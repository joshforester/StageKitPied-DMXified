#include "QlcplusOutputProcessor.h"


std::string QlcplusOutputProcessor::defaultQlcplusWebsocketUrl = "ws://127.0.0.1:9999/qlcplusWS";
unsigned int QlcplusOutputProcessor::defaultQlcplusConnectSleepTimeMs = 100;
unsigned int QlcplusOutputProcessor::defaultQlcplusSendSleepTimeMs = 0;

QlcplusOutputProcessor::QlcplusOutputProcessor(
		const std::string& url,
		const long qlcplusConnectSleepTimeMs,
		const long qlcplusSendSleepTimeMs
)
    : url(url),
      qlcplusConnectSleepTimeMs(qlcplusConnectSleepTimeMs),
      qlcplusSendSleepTimeMs(qlcplusSendSleepTimeMs),
	  connectionMetadata(nullptr) {
}

QlcplusOutputProcessor::~QlcplusOutputProcessor() {
    try {
    	MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG("Cleaning up QlcplusOutputProcessor");

        // Ensure the WebSocket connection is closed gracefully
        if (connectionMetadata != nullptr && connectionMetadata->get_status() == "Open") {
            websocketEndpoint.close(websocketpp::close::status::normal, "Cleanup");
            MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG("WebSocket connection closed.");
        }

        connectionMetadata.reset();  // This will automatically delete the shared_ptr
    } catch (const std::exception& e) {
    	MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("Exception during QlcplusOutputProcessor cleanup: " + std::string(e.what()));
    } catch (...) {
    	MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("Unknown exception during QlcplusOutputProcessor cleanup.");
    }
}

void QlcplusOutputProcessor::process(const Output output) {

	// Ensure WebSocket connection is open before processing the output
    ensureConnection();

	std::string message = "process called for output " + output.getType() + ":" + output.getSubtype() + ".";
	MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG(message);


	// Send the message over the WebSocket connection
	if (connectionMetadata != nullptr && connectionMetadata->get_status() == "Open") {
		std::string data = qlcplusOutputToQlcplusWsCommand(output);
		websocketEndpoint.send(data);
		MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG("Sent message: " + data);
        std::this_thread::sleep_for(std::chrono::milliseconds(qlcplusSendSleepTimeMs));
	} else {
		MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("WebSocket connection failed, unable to send message.");
	}

}

void QlcplusOutputProcessor::ensureConnection() {

    if (connectionMetadata == nullptr || connectionMetadata->get_status() != "Open") {
        MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG("No connection to Qlcplus Websocket.  Establishing.");

        // Attempt to establish a new connection
        bool isConnected = false;
        try {
            isConnected = websocketEndpoint.connect(url);
            std::this_thread::sleep_for(std::chrono::milliseconds(qlcplusConnectSleepTimeMs));
        } catch (const std::exception& e) {
        	MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("Exception while connecting to WebSocket: " + std::string(e.what()));
        } catch (...) {
        	MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("Unknown error while connecting to WebSocket.");
        }

        if (isConnected) {
            try {
                connectionMetadata = websocketEndpoint.get_metadata();
                if (connectionMetadata == nullptr) {
                	MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("Failed to retrieve metadata for connection.");
                }
            } catch (const std::exception& e) {
            	MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("Error retrieving metadata: " + std::string(e.what()));
            }
        }
    }
}

const std::string QlcplusOutputProcessor::qlcplusOutputToQlcplusWsCommand(const Output output) {
	std::string type = output.getType();
	if (type != outputTypeToString(OUTPUT_TYPE::qlcplusOutput)) {
		throw std::invalid_argument("Invalid outputType " + type);
	}

	QLCPLUS_OUTPUT_SUBTYPE subtype = stringToQlcplusOutputSubtype(output.getSubtype());
	if (subtype == QLCPLUS_OUTPUT_SUBTYPE::basicWidgetValueSet) {
	    std::string widgetId = output.getParameter(qlcplusOutputParameterToString(QLCPLUS_OUTPUT_PARAMETER::widgetId));
		std::string value = output.getParameter(qlcplusOutputParameterToString(QLCPLUS_OUTPUT_PARAMETER::value));
		return widgetId + "|" + value;
	} else if (subtype == QLCPLUS_OUTPUT_SUBTYPE::setFunctionStatus) {
		std::string functionId = output.getParameter(qlcplusOutputParameterToString(QLCPLUS_OUTPUT_PARAMETER::functionId));
		std::string status = output.getParameter(qlcplusOutputParameterToString(QLCPLUS_OUTPUT_PARAMETER::status));
		return "QLC+API|setFunctionStatus|" + functionId + "|" + status;
	} else if (subtype == QLCPLUS_OUTPUT_SUBTYPE::simpleDeskChannelSet) {
		std::string absoluteDmxAddress = output.getParameter(qlcplusOutputParameterToString(QLCPLUS_OUTPUT_PARAMETER::absoluteDmxAddress));
		std::string value = output.getParameter(qlcplusOutputParameterToString(QLCPLUS_OUTPUT_PARAMETER::value));
		return "CH|" + absoluteDmxAddress + "|" + value;
	}

	MSG_QLCPLUSOUTPUTPROCESSOR_ERROR("Unknown output subtype specified for " + type + "!");
	return "NOOP";
}
