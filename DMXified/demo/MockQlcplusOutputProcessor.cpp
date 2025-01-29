#include "../engine/QlcplusOutputProcessor.h"

#include <iostream>

QlcplusOutputProcessor::QlcplusOutputProcessor(const std::string& url)
    : url(url) {
}

void QlcplusOutputProcessor::process(const Output output) {
    // Convert the Output data to a message or a format that needs to be sent
    std::string message = "MockQlcpplusOutputProcessor.process called for output: " + output.getType() + ":" + output.getSubtype() + "\n";

    std::cout << message << std::endl;
}
