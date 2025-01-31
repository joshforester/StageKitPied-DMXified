#ifndef EESD

#include "QlcplusOutputProcessor.h"
#include <iostream>

QlcplusOutputProcessor::QlcplusOutputProcessor(const std::string& url)
    : url(url), connectionMetadata(nullptr) {
}

void QlcplusOutputProcessor::process(const Output output) {
	std::lock_guard<std::mutex> lock(mtx);  // Lock for shared resource protection

	// Ensure WebSocket connection is open before processing the output
    ensureConnection();

	std::string message = "process called for output " + output.getType() + ":" + output.getSubtype() + ".";
	MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG( message );


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
		MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG("No connection to Qlcplus Websocket.  Establishing.");

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
