#ifndef EESD
#include "QlcplusOutputProcessor.h"
#include <iostream>

QlcplusOutputProcessor::QlcplusOutputProcessor(const std::string& url)
    : url(url), connectionMetadata(nullptr) {
    // Initialize the connectionMetadata pointer as null
}

void QlcplusOutputProcessor::process(const Output output) {
    // Ensure WebSocket connection is open before processing the output
    ensureConnection();

    // Convert the Output data to a message or a format that needs to be sent
    std::string message = "Processing output...";  // Placeholder message

    // Send the message over the WebSocket connection
    if (connectionMetadata != nullptr && connectionMetadata->get_status() == "open") {
        websocketEndpoint.send(connectionMetadata->get_id(), message);
        std::cout << "Sent message: " << message << std::endl;
    } else {
        std::cerr << "WebSocket connection failed, unable to send message." << std::endl;
    }
}

void QlcplusOutputProcessor::ensureConnection() {
    if (connectionMetadata == nullptr || connectionMetadata->get_status() != "open") {
        // If no valid connection exists or it's not open, attempt to establish a new connection
        int connectionId = websocketEndpoint.connect(url);

        if (connectionId != -1) {
            connectionMetadata = websocketEndpoint.get_metadata(connectionId);
            std::cout << "WebSocket connection established with ID: " << connectionMetadata->get_id() << std::endl;
        } else {
            std::cerr << "Failed to establish WebSocket connection." << std::endl;
        }
    }
}
#endif // EESD
