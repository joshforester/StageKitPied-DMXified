#ifndef QLCPLUSOUTPUTPROCESSOR_H
#define QLCPLUSOUTPUTPROCESSOR_H

#include <string>
#include "../config/Output.h"  // the Output class
#include "../websocket/WebsocketEndpoint.h"  // Include your WebsocketEndpoint header
#include "../websocket/ConnectionMetadata.h"  // Include ConnectionMetadata

class QlcplusOutputProcessor {
public:
    // Constructor that initializes WebSocket endpoint with a URL
    QlcplusOutputProcessor(const std::string& url);

    // Process the Output and ensure WebSocket is open
    void process(const Output output);

private:
    std::string url;                       // WebSocket URL

#ifndef EESD
    WebsocketEndpoint websocketEndpoint;  // WebSocket endpoint object
    ConnectionMetadata::ptr connectionMetadata;  // Metadata for the WebSocket connection

    // Ensure WebSocket connection is open before sending data
    void ensureConnection();
#endif // EESD
};

#endif // QLCPLUSOUTPUTPROCESSOR_H
