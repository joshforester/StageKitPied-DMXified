#include "QlcplusOutputProcessor.h"


QlcplusOutputProcessor::QlcplusOutputProcessor(const std::string& url)
    : url(url), connectionMetadata(nullptr) {
}

QlcplusOutputProcessor::~QlcplusOutputProcessor() {
    try {
        std::cout << "Cleaning up QlcplusOutputProcessor\n";

        // Ensure the WebSocket connection is closed gracefully
        if (connectionMetadata != nullptr && connectionMetadata->get_status() == "Open") {
            websocketEndpoint.close(websocketpp::close::status::normal, "Cleanup");
            std::cout << "WebSocket connection closed." << std::endl;
        }

        connectionMetadata.reset();  // This will automatically delete the shared_ptr
    } catch (const std::exception& e) {
        std::cerr << "Exception during QlcplusOutputProcessor cleanup: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception during QlcplusOutputProcessor cleanup." << std::endl;
    }
}

void QlcplusOutputProcessor::process(const Output output) {

	// Ensure WebSocket connection is open before processing the output
    ensureConnection();

	std::string message = "process called for output " + output.getType() + ":" + output.getSubtype() + ".";
	MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG( message );


	// Send the message over the WebSocket connection
	if (connectionMetadata != nullptr && connectionMetadata->get_status() == "Open") {
		websocketEndpoint.send(message);
		std::cout << "Sent message: " << message << std::endl;
	} else {
		std::cerr << "WebSocket connection failed, unable to send message." << std::endl;
	}

}

void QlcplusOutputProcessor::ensureConnection() {

    if (connectionMetadata == nullptr || connectionMetadata->get_status() != "Open") {
        MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG("No connection to Qlcplus Websocket.  Establishing.");

        // Attempt to establish a new connection
        bool isConnected = false;
        try {
            isConnected = websocketEndpoint.connect(url);
        } catch (const std::exception& e) {
            std::cerr << "Exception while connecting to WebSocket: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error while connecting to WebSocket." << std::endl;
        }

        if (isConnected) {
            try {
                connectionMetadata = websocketEndpoint.get_metadata();
                if (connectionMetadata != nullptr) {
                    std::cout << "WebSocket connection established." << std::endl;
                } else {
                    std::cerr << "Failed to retrieve metadata for connection." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error retrieving metadata: " << e.what() << std::endl;
            }
        }
    }
}
