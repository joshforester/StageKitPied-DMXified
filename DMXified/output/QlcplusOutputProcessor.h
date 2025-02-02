#ifndef QLCPLUSOUTPUTPROCESSOR_H
#define QLCPLUSOUTPUTPROCESSOR_H

#ifdef DEBUG
  #define MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG( str ) do { std::cout << "QlcplusOutputProcessor : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG( str ) do { } while ( false )
#endif

#define MSG_QLCPLUSOUTPUTPROCESSOR_ERROR( str ) do { std::cout << "QlcplusOutputProcessor : ERROR : " << str << std::endl; } while( false )
#define MSG_QLCPLUSOUTPUTPROCESSOR_INFO( str ) do { std::cout << "QlcplusOutputProcessor : INFO : " << str << std::endl; } while( false )

#include <string>
#include <iostream>
#include <mutex>                             // For std::mutex
#include "../config/Output.h"                // the Output class
#include "../config/MappingConfigConsts.h"   // For translating config
#include "../websocket/WebsocketEndpoint.h"  // Include your WebsocketEndpoint header
#include "../websocket/ConnectionMetadata.h" // Include ConnectionMetadata

class QlcplusOutputProcessor {
public:
    // Constructor that initializes WebSocket endpoint with a URL
    QlcplusOutputProcessor(const std::string& url = "ws://127.0.0.1:9999/qlcplusWS");

#ifndef EESD
    ~QlcplusOutputProcessor();
#endif // EESD

    // Process the Output and ensure WebSocket is open
    void process(const Output output);

private:
    std::string url;                       // WebSocket URL

#ifndef EESD
    WebsocketEndpoint websocketEndpoint;  // WebSocket endpoint object
    ConnectionMetadata::ptr connectionMetadata;  // Metadata for the WebSocket connection

    // Ensure WebSocket connection is open before sending data
    void ensureConnection();
    const std::string qlcplusOutputToQlcplusWsCommand(const Output output);
#endif // EESD
};

#endif // QLCPLUSOUTPUTPROCESSOR_H
