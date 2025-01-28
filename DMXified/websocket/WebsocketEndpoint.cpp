#include "WebsocketEndpoint.h"
#include "ConnectionMetadata.h"

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>


WebsocketEndpoint::WebsocketEndpoint() : m_next_id(0) {
	m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
	m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

	m_endpoint.init_asio();
	m_endpoint.start_perpetual();

	m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
}

WebsocketEndpoint::~WebsocketEndpoint() {
	m_endpoint.stop_perpetual();

	for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
		if (it->second->get_status() != "Open") {
			// Only close open connections
			continue;
		}

		std::cout << "> Closing connection " << it->second->get_id() << std::endl;

		websocketpp::lib::error_code ec;
		m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
		if (ec) {
			std::cout << "> Error closing connection " << it->second->get_id() << ": "
					  << ec.message() << std::endl;
		}
	}

	m_thread->join();
}

int WebsocketEndpoint::connect(std::string const & uri) {
	websocketpp::lib::error_code ec;

	client::connection_ptr con = m_endpoint.get_connection(uri, ec);

	if (ec) {
		std::cout << "> Connect initialization error: " << ec.message() << std::endl;
		return -1;
	}

	int new_id = m_next_id++;
	ConnectionMetadata::ptr metadata_ptr = websocketpp::lib::make_shared<ConnectionMetadata>(new_id, con->get_handle(), uri);
	m_connection_list[new_id] = metadata_ptr;

	con->set_open_handler(websocketpp::lib::bind(
		&ConnectionMetadata::on_open,
		metadata_ptr,
		&m_endpoint,
		websocketpp::lib::placeholders::_1
	));
	con->set_fail_handler(websocketpp::lib::bind(
		&ConnectionMetadata::on_fail,
		metadata_ptr,
		&m_endpoint,
		websocketpp::lib::placeholders::_1
	));
	con->set_close_handler(websocketpp::lib::bind(
		&ConnectionMetadata::on_close,
		metadata_ptr,
		&m_endpoint,
		websocketpp::lib::placeholders::_1
	));
	con->set_message_handler(websocketpp::lib::bind(
		&ConnectionMetadata::on_message,
		metadata_ptr,
		websocketpp::lib::placeholders::_1,
		websocketpp::lib::placeholders::_2
	));

	m_endpoint.connect(con);

	return new_id;
}

void WebsocketEndpoint::close(int id, websocketpp::close::status::value code, std::string reason) {
	websocketpp::lib::error_code ec;

	con_list::iterator metadata_it = m_connection_list.find(id);
	if (metadata_it == m_connection_list.end()) {
		std::cout << "> No connection found with id " << id << std::endl;
		return;
	}

	m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
	if (ec) {
		std::cout << "> Error initiating close: " << ec.message() << std::endl;
	}
}

void WebsocketEndpoint::send(int id, std::string message) {
	websocketpp::lib::error_code ec;

	con_list::iterator metadata_it = m_connection_list.find(id);
	if (metadata_it == m_connection_list.end()) {
		std::cout << "> No connection found with id " << id << std::endl;
		return;
	}

	m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
	if (ec) {
		std::cout << "> Error sending message: " << ec.message() << std::endl;
		return;
	}

	metadata_it->second->record_sent_message(message);
}

ConnectionMetadata::ptr WebsocketEndpoint::get_metadata(int id) const {
	con_list::const_iterator metadata_it = m_connection_list.find(id);
	if (metadata_it == m_connection_list.end()) {
		return ConnectionMetadata::ptr();
	} else {
		return metadata_it->second;
	}
}


client m_endpoint;
websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

con_list m_connection_list;
int m_next_id;
