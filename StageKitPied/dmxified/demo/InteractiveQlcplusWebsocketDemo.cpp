#include "../websocket/WebsocketEndpoint.h"  // Ensure the path is correct
#include "../websocket/ConnectionMetadata.h" // This includes the metadata class

// Include other necessary standard libraries
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <cctype>
#include <locale>

// Trim function to remove leading and trailing whitespace
std::string trim(const std::string& str) {
    // Find the first non-whitespace character
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return ""; // String contains only whitespace
    }

    // Find the last non-whitespace character
    auto end = str.find_last_not_of(" \t\n\r\f\v");

    // Create a substring from the start to end
    return str.substr(start, end - start + 1);
}

int main() {
    bool done = false;
    std::string input;
    WebsocketEndpoint endpoint;  // Create the WebSocket endpoint

    while (!done) {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);

        if (input == "quit") {
            done = true;  // Exit the loop immediately
            endpoint.close(websocketpp::close::status::normal, "quit");
        } else if (input == "help") {
            std::cout
                << "\nCommand List:\n"
                << "connect <ws uri>\n"
                << "send <message>\n"
                << "close [<close code:default=1000>] [<close reason>]\n"
                << "show\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << std::endl;
        } else if (input.substr(0, 7) == "connect") {
            bool isConnected = endpoint.connect(input.substr(8));
            if (isConnected) {
                std::cout << "> Created connection" << std::endl;
            }
        } else if (input.substr(0, 4) == "send") {
            std::stringstream ss(input);

            std::string cmd;
            std::string message;

            ss >> cmd;
            std::getline(ss, message);

            std::cout << "Sending message:" + trim(message) << std::endl;

            endpoint.send(trim(message));
        } else if (input.substr(0, 5) == "close") {
            std::stringstream ss(input);

            std::string cmd;
            int close_code = websocketpp::close::status::normal;
            std::string reason;

            ss >> cmd >> close_code;
            std::getline(ss, reason);

            // Only close if there's an active connection
            ConnectionMetadata::ptr metadata = endpoint.get_metadata();
            if (metadata) {
                endpoint.close(close_code, reason);
            } else {
                std::cout << "> No active connection to close." << std::endl;
            }
        } else if (input.substr(0, 4) == "show") {
            ConnectionMetadata::ptr metadata = endpoint.get_metadata();
            if (metadata) {
                std::cout << *metadata << std::endl;
            } else {
                std::cout << "> Unknown connection" << std::endl;
            }
        } else {
            std::cout << "> Unrecognized Command" << std::endl;
        }
    }

    return 0;
}
