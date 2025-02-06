#include "../output/QlcplusOutputProcessor.h"

std::string QlcplusOutputProcessor::defaultQlcplusWebsocketUrl = "";

QlcplusOutputProcessor::QlcplusOutputProcessor(
		const std::string& url,
		const long qlcplusConnectSleepTimeMs,
		const long qlcplusSendSleepTimeMs
)
    : url(url),
	  qlcplusConnectSleepTimeMs(qlcplusConnectSleepTimeMs),
	  qlcplusSendSleepTimeMs(qlcplusSendSleepTimeMs) {
}

void QlcplusOutputProcessor::process(const Output output) {
    // Convert the Output data to a message or a format that needs to be sent
    std::string message = "MockQlcpplusOutputProcessor.process called for output: " + output.getType() + ":" + output.getSubtype() + "\n";

    std::cout << message << std::endl;
}
