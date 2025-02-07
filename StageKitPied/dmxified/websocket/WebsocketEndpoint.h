// WebsocketEndpoint.h
#ifndef WEBSOCKET_WEBSOCKETENDPOINT_H_
#define WEBSOCKET_WEBSOCKETENDPOINT_H_

#include <thread>
#include "ConnectionMetadata.h"
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

#ifdef DEBUG
  #define MSG_WEBSOCKET_WEBSOCKETENDPOINT_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|WebsocketEndpoint : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_WEBSOCKET_WEBSOCKETENDPOINT_DEBUG( str ) do { } while ( false )
#endif

#define MSG_WEBSOCKET_WEBSOCKETENDPOINT_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|WebsocketEndpoint : ERROR : " << str << std::endl; } while( false )
#define MSG_WEBSOCKET_WEBSOCKETENDPOINT_INFO( str ) do { std::cout << std::this_thread::get_id() << "|WebsocketEndpoint : INFO : " << str << std::endl; } while( false )


// NOTE: This class is not thread-safe; please ensure calling classes (such as QlcplusOutputProcessor) are thread-safe.
class WebsocketEndpoint {
public:
  WebsocketEndpoint();
  ~WebsocketEndpoint();

  bool connect(std::string const& uri);
  void close(websocketpp::close::status::value code, std::string reason);
  void send(std::string message);
  ConnectionMetadata::ptr get_metadata() const;

private:
  typedef websocketpp::client<websocketpp::config::asio_client> client;

  static client m_endpoint;  // Declare static member
  static websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

  // Only need one connection now
  static ConnectionMetadata::ptr m_metadata;

};

#endif /* WEBSOCKET_WEBSOCKETENDPOINT_H_ */
