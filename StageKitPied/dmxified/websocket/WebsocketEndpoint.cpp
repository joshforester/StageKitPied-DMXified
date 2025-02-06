// WebsocketEndpoint.cpp
#include "WebsocketEndpoint.h"
#include "ConnectionMetadata.h"


// Define the static members of WebsocketEndpoint here
websocketpp::client<websocketpp::config::asio_client> WebsocketEndpoint::m_endpoint;
websocketpp::lib::shared_ptr<websocketpp::lib::thread> WebsocketEndpoint::m_thread;
ConnectionMetadata::ptr WebsocketEndpoint::m_metadata;  // Only one connection metadata

WebsocketEndpoint::WebsocketEndpoint() {
  // Static members are already initialized, so no need to do it in the constructor
  m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
  m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

  m_endpoint.init_asio();
  m_endpoint.start_perpetual();

  m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
}

static std::atomic<bool> is_cleaning_up{false};  // Add this static flag

WebsocketEndpoint::~WebsocketEndpoint() {
	if (is_cleaning_up.exchange(true)) {
	  // Cleanup already in progress, no need to do it again
	  return;
	}

    if (m_thread) {
        // Stop the perpetual running loop
        m_endpoint.stop_perpetual();

        // Join the thread to wait for it to finish
        m_thread->join();
    }

    // Close the connection if it exists
    if (m_metadata) {
        websocketpp::lib::error_code ec;
        m_endpoint.close(m_metadata->get_hdl(), websocketpp::close::status::normal, "", ec);
        if (ec) {
        	MSG_WEBSOCKET_WEBSOCKETENDPOINT_ERROR("Error closing connection: " + ec.message());
        }
    }
}

bool WebsocketEndpoint::connect(std::string const& uri) {
  websocketpp::lib::error_code ec;

  client::connection_ptr con = m_endpoint.get_connection(uri, ec);

  if (ec) {
	MSG_WEBSOCKET_WEBSOCKETENDPOINT_ERROR("Connect initialization error: " + ec.message());
	return false;
  }

  m_metadata = websocketpp::lib::make_shared<ConnectionMetadata>(con->get_handle(), uri);

  con->set_open_handler(websocketpp::lib::bind(
      &ConnectionMetadata::on_open,
      m_metadata,
      &m_endpoint,
      websocketpp::lib::placeholders::_1
  ));
  con->set_fail_handler(websocketpp::lib::bind(
      &ConnectionMetadata::on_fail,
      m_metadata,
      &m_endpoint,
      websocketpp::lib::placeholders::_1
  ));
  con->set_close_handler(websocketpp::lib::bind(
      &ConnectionMetadata::on_close,
      m_metadata,
      &m_endpoint,
      websocketpp::lib::placeholders::_1
  ));
  con->set_message_handler(websocketpp::lib::bind(
      &ConnectionMetadata::on_message,
      m_metadata,
      websocketpp::lib::placeholders::_1,
      websocketpp::lib::placeholders::_2
  ));

  m_endpoint.connect(con);

  return true;  // Single connection, so no need for an ID
}

void WebsocketEndpoint::close(websocketpp::close::status::value code, std::string reason) {
  if (!m_metadata) {
	MSG_WEBSOCKET_WEBSOCKETENDPOINT_DEBUG("No active connection to close.");
    return;
  }

  websocketpp::lib::error_code ec;
  m_endpoint.close(m_metadata->get_hdl(), code, reason, ec);
  if (ec) {
	  MSG_WEBSOCKET_WEBSOCKETENDPOINT_ERROR("Error initiating close: " + ec.message());
  }
}

void WebsocketEndpoint::send(std::string message) {
  if (!m_metadata) {
	MSG_WEBSOCKET_WEBSOCKETENDPOINT_DEBUG("No active connection to send message.");
    return;
  }

  websocketpp::lib::error_code ec;
  m_endpoint.send(m_metadata->get_hdl(), message, websocketpp::frame::opcode::text, ec);
  if (ec) {
	MSG_WEBSOCKET_WEBSOCKETENDPOINT_ERROR("Error sending message: " + ec.message());
    return;
  }

  m_metadata->record_sent_message(message);
}

ConnectionMetadata::ptr WebsocketEndpoint::get_metadata() const {
  return m_metadata;
}
