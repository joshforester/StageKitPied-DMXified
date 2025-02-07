#ifndef QLCPLUSOUTPUTPROCESSOR_H
#define QLCPLUSOUTPUTPROCESSOR_H

#include <thread>
#include <string>
#include <iostream>
#include <mutex>                             // For std::mutex
#include "../config/Output.h"                // the Output class
#include "../config/MappingConfigConsts.h"   // For translating config
#include "../websocket/WebsocketEndpoint.h"  // Include your WebsocketEndpoint header
#include "../websocket/ConnectionMetadata.h" // Include ConnectionMetadata

#ifdef DEBUG
  #define MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|QlcplusOutputProcessor : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_QLCPLUSOUTPUTPROCESSOR_DEBUG( str ) do { } while ( false )
#endif

#define MSG_QLCPLUSOUTPUTPROCESSOR_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|QlcplusOutputProcessor : ERROR : " << str << std::endl; } while( false )
#define MSG_QLCPLUSOUTPUTPROCESSOR_INFO( str ) do { std::cout << std::this_thread::get_id() << "|QlcplusOutputProcessor : INFO : " << str << std::endl; } while( false )


class QlcplusOutputProcessor {
public:

	static std::string defaultQlcplusWebsocketUrl;

	static unsigned int defaultQlcplusConnectSleepTimeMs;
	static unsigned int defaultQlcplusSendSleepTimeMs;

	// Constructor that initializes WebSocket endpoint with a URL
    QlcplusOutputProcessor(
    	const std::string& url = defaultQlcplusWebsocketUrl,
		const long qlcplusConnectSleepTimeMs = defaultQlcplusConnectSleepTimeMs,
		const long qlcplusSendSleepTimeMs = defaultQlcplusSendSleepTimeMs
	);

#ifndef EESD
    ~QlcplusOutputProcessor();
#endif // EESD

    // Process the Output and ensure WebSocket is open
    void process(const Output output);

private:
    std::string url;                       // WebSocket URL
	const long qlcplusConnectSleepTimeMs;
	const long qlcplusSendSleepTimeMs;

#ifndef EESD
    WebsocketEndpoint websocketEndpoint;  // WebSocket endpoint object
    ConnectionMetadata::ptr connectionMetadata;  // Metadata for the WebSocket connection

    // Ensure WebSocket connection is open before sending data
    void ensureConnection();
    const std::string qlcplusOutputToQlcplusWsCommand(const Output output);
#endif // EESD
};

#endif // QLCPLUSOUTPUTPROCESSOR_H
