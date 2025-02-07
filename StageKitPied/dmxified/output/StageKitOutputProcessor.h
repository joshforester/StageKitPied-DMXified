#ifndef STAGEKITOUTPUTPROCESSOR_H
#define STAGEKITOUTPUTPROCESSOR_H

#include <thread>
#include <cstdint> // For uint8_t
#include <mutex>   // For std::mutex
#include "../config/Output.h"


#ifdef DEBUG
  #define MSG_STAGEKITOUTPUTPROCESSOR_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|StageKitOutputProcessor : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_STAGEKITOUTPUTPROCESSOR_DEBUG( str ) do { } while ( false )
#endif

#define MSG_STAGEKITOUTPUTPROCESSOR_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|StageKitOutputProcessor : ERROR : " << str << std::endl; } while( false )
#define MSG_STAGEKITOUTPUTPROCESSOR_INFO( str ) do { std::cout << std::this_thread::get_id() << "|StageKitOutputProcessor : INFO : " << str << std::endl; } while( false )

// Forward declare RpiLightsController
class RpiLightsController;


class StageKitOutputProcessor {
public:
    // Constructor (initialize the RpiLightsController object)
    StageKitOutputProcessor(RpiLightsController& rpiLightsController);

    // The process function that processes the output based on weights
    void process(Output output, uint8_t left_weight, uint8_t right_weight) const;

private:
    RpiLightsController& rpiLightsController; // Lights controller (reference)
//    mutable std::mutex mtx;                   // Mutex to protect shared data
};

#endif // STAGEKITOUTPUTPROCESSOR_H
