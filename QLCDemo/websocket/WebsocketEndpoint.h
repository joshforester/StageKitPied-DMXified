#ifndef WEBSOCKET_WEBSOCKETENDPOINT_H_
#define WEBSOCKET_WEBSOCKETENDPOINT_H_

#ifdef DEBUG
  #define MSG_WEBSOCKET_WEBSOCKETENDPOINT_DEBUG( str ) do { std::cout << "WebsocketEndpoint : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_WEBSOCKET_WEBSOCKETENDPOINT_DEBUG( str ) do { } while ( false )
#endif

#define MSG_WEBSOCKET_WEBSOCKETENDPOINT_ERROR( str ) do { std::cout << "WebsocketEndpoint : ERROR : " << str << std::endl; } while( false )
#define MSG_WEBSOCKET_WEBSOCKETENDPOINT_INFO( str ) do { std::cout << "WebsocketEndpoint : INFO : " << str << std::endl; } while( false )

#include "ConnectionMetadata.h"

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

typedef std::map<int,ConnectionMetadata::ptr> con_list;

class WebsocketEndpoint {
public:

  WebsocketEndpoint();

  ~WebsocketEndpoint();

  int connect(std::string const & uri);

  void close(int id, websocketpp::close::status::value code, std::string reason);

  void send(int id, std::string message);

  ConnectionMetadata::ptr get_metadata(int id) const;

private:

  client 	m_endpoint;
  websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
  con_list 	m_connection_list;
  int 		m_next_id;

};

#endif /* WEBSOCKET_WEBSOCKETENDPOINT_H_ */
