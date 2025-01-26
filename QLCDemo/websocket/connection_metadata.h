#ifndef WEBSOCKET_CONNECTION_METADATA_H_
#define WEBSOCKET_CONNECTION_METADATA_H_

#ifdef DEBUG
  #define MSG_WEBSOCKET_CONNECTION_METADATA_DEBUG( str ) do { std::cout << "websocket::connection_metadata : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_WEBSOCKET_CONNECTION_METADATA_DEBUG( str ) do { } while ( false )
#endif

#define MSG_WEBSOCKET_CONNECTION_METADATA_ERROR( str ) do { std::cout << "websocket::connection_metadata : ERROR : " << str << std::endl; } while( false )
#define MSG_WEBSOCKET_CONNECTION_METADATA_INFO( str ) do { std::cout << "websocket::connection_metadata : INFO : " << str << std::endl; } while( false )

//
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

//
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

//
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

class connection_metadata {
public:

  typedef websocketpp::lib::shared_ptr<connection_metadata> ptr;

  connection_metadata(int id, websocketpp::connection_hdl hdl, std::string uri);

  void on_open(client * c, websocketpp::connection_hdl hdl);

  void on_fail(client * c, websocketpp::connection_hdl hdl);

  void on_close(client * c, websocketpp::connection_hdl hdl);

  void on_message(websocketpp::connection_hdl, client::message_ptr msg);

  websocketpp::connection_hdl get_hdl() const;

  int get_id() const;

  std::string get_status() const;

  void record_sent_message(std::string message);

  friend std::ostream & operator<< (std::ostream & out, connection_metadata const & data);

private:

  int 						  	m_id;
  websocketpp::connection_hdl 	m_hdl;
  std::string 					m_status;
  std::string 					m_uri;
  std::string 					m_server;
  std::string 					m_error_reason;
  std::vector<std::string> 		m_messages;

};

#endif /* WEBSOCKET_CONNECTION_METADATA_H_ */
