#include "ConnectionMetadata.h"


ConnectionMetadata::ConnectionMetadata(websocketpp::connection_hdl hdl, std::string uri)
  : m_hdl(hdl)
  , m_status("Connecting")
  , m_uri(uri)
  , m_server("N/A")
{}

void ConnectionMetadata::on_open(client * c, websocketpp::connection_hdl hdl) {
    m_status = "Open";
    MSG_WEBSOCKET_CONNECTIONMETADATA_DEBUG("Connection opened, status set to 'Open'.");

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
}

void ConnectionMetadata::on_fail(client * c, websocketpp::connection_hdl hdl) {
    m_status = "Failed";
    MSG_WEBSOCKET_CONNECTIONMETADATA_DEBUG("Connection failed, status set to 'Failed'.");

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
    m_error_reason = con->get_ec().message();
}

void ConnectionMetadata::on_close(client * c, websocketpp::connection_hdl hdl) {
    m_status = "Closed";
    MSG_WEBSOCKET_CONNECTIONMETADATA_DEBUG("Connection closed, status set to 'Closed'.");

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    std::stringstream s;
    s << "close code: " << con->get_remote_close_code() << " ("
      << websocketpp::close::status::get_string(con->get_remote_close_code())
      << "), close reason: " << con->get_remote_close_reason();
    m_error_reason = s.str();
}

void ConnectionMetadata::on_message(websocketpp::connection_hdl, client::message_ptr msg) {
    if (msg->get_opcode() == websocketpp::frame::opcode::text) {
        m_messages.push_back("<< " + msg->get_payload());
    } else {
        m_messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
    }
}

websocketpp::connection_hdl ConnectionMetadata::get_hdl() const {
    return m_hdl;
}

std::string ConnectionMetadata::get_status() const {
    if (m_status.empty()) {
        MSG_WEBSOCKET_CONNECTIONMETADATA_ERROR("Status is empty for connection.");
        return "Unknown";  // Fallback if status is empty
    }
    return m_status;
}

void ConnectionMetadata::record_sent_message(std::string message) {
    m_messages.push_back(">> " + message);
}

std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data) {
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";
    out << "> Messages Processed: (" << data.m_messages.size() << ") \n";

    for (const auto& msg : data.m_messages) {
        out << msg << "\n";
    }

    return out;
}
