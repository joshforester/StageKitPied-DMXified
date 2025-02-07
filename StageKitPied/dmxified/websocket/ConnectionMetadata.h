#ifndef WEBSOCKET_CONNECTIONMETADATA_H_
#define WEBSOCKET_CONNECTIONMETADATA_H_

#include <thread>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>


#ifdef DEBUG
  #define MSG_WEBSOCKET_CONNECTIONMETADATA_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|ConnectionMetadata : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_WEBSOCKET_CONNECTIONMETADATA_DEBUG( str ) do { } while ( false )
#endif

#define MSG_WEBSOCKET_CONNECTIONMETADATA_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|ConnectionMetadata : ERROR : " << str << std::endl; } while( false )
#define MSG_WEBSOCKET_CONNECTIONMETADATA_INFO( str ) do { std::cout << std::this_thread::get_id() << "|ConnectionMetadata : INFO : " << str << std::endl; } while( false )


typedef websocketpp::client<websocketpp::config::asio_client> client;

class ConnectionMetadata {
public:
  typedef websocketpp::lib::shared_ptr<ConnectionMetadata> ptr;

  ConnectionMetadata(websocketpp::connection_hdl hdl, std::string uri);

  void on_open(client * c, websocketpp::connection_hdl hdl);

  void on_fail(client * c, websocketpp::connection_hdl hdl);

  void on_close(client * c, websocketpp::connection_hdl hdl);

  void on_message(websocketpp::connection_hdl, client::message_ptr msg);

  websocketpp::connection_hdl get_hdl() const;

  std::string get_status() const;

  void record_sent_message(std::string message);

  friend std::ostream & operator<< (std::ostream & out, ConnectionMetadata const & data);

private:
  websocketpp::connection_hdl m_hdl;
  std::string m_status;
  std::string m_uri;
  std::string m_server;
  std::string m_error_reason;
  std::vector<std::string> m_messages;
};

#endif /* WEBSOCKET_CONNECTIONMETADATA_H_ */
