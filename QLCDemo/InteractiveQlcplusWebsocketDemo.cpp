#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "websocket/ConnectionMetadata.h"
#include "websocket/WebsocketEndpoint.h"

typedef std::string string;

int main() {
    bool done = false;
    string input;
    WebsocketEndpoint endpoint;

    while (!done) {
        std::cout << "Enter Command: ";
        std::getline(std::cin, input);

        if (input == "quit") {
            done = true;
        } else if (input == "help") {
            std::cout
                << "\nCommand List:\n"
                << "connect <ws uri>\n"
                << "send <connection id> <message>\n"
                << "close <connection id> [<close code:default=1000>] [<close reason>]\n"
                << "show <connection id>\n"
                << "help: Display this help text\n"
                << "quit: Exit the program\n"
                << std::endl;
        } else if (input.substr(0,7) == "connect") {
            int id = endpoint.connect(input.substr(8));
            if (id != -1) {
                std::cout << "> Created connection with id " << id << std::endl;
            }
        } else if (input.substr(0,4) == "send") {
            std::stringstream ss(input);

            string cmd;
            int id;
            string message;

            ss >> cmd >> id;
            std::getline(ss,message);

            endpoint.send(id, message);
        } else if (input.substr(0,5) == "close") {
            std::stringstream ss(input);

            string cmd;
            int id;
            int close_code = websocketpp::close::status::normal;
            string reason;

            ss >> cmd >> id >> close_code;
            std::getline(ss,reason);

            endpoint.close(id, close_code, reason);
        } else if (input.substr(0,4) == "show") {
            int id = atoi(input.substr(5).c_str());

            ConnectionMetadata::ptr metadata = endpoint.get_metadata(id);
            if (metadata) {
                std::cout << *metadata << std::endl;
            } else {
                std::cout << "> Unknown connection id " << id << std::endl;
            }
        } else {
            std::cout << "> Unrecognized Command" << std::endl;
        }
    }

    return 0;
}
