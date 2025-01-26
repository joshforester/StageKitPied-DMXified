#ifndef WEBSOCKET_WEBSOCKET_ENDPOINT_H_
#define WEBSOCKET_WEBSOCKET_ENDPOINT_H_

#ifdef DEBUG
  #define MSG_WEBSOCKET_WEBSOCKET_ENDPOINT_DEBUG( str ) do { std::cout << "websocket::websocket_endpoint : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_WEBSOCKET_WEBSOCKET_ENDPOINT_DEBUG( str ) do { } while ( false )
#endif

#define MSG_WEBSOCKET_WEBSOCKET_ENDPOINT_ERROR( str ) do { std::cout << "websocket::websocket_endpoint : ERROR : " << str << std::endl; } while( false )
#define MSG_WEBSOCKET_WEBSOCKET_ENDPOINT_INFO( str ) do { std::cout << "websocket::websocket_endpoint : INFO : " << str << std::endl; } while( false )

#include "connection_metadata.h"

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

typedef std::map<int,connection_metadata::ptr> con_list;

class websocket_endpoint {
public:

  websocket_endpoint();

  ~websocket_endpoint();

  int connect(std::string const & uri);

  void close(int id, websocketpp::close::status::value code, std::string reason);

  void send(int id, std::string message);

  connection_metadata::ptr get_metadata(int id) const;

private:

  client 	m_endpoint;
  websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
  con_list 	m_connection_list;
  int 		m_next_id;

};

#endif /* WEBSOCKET_WEBSOCKET_ENDPOINT_H_ */
