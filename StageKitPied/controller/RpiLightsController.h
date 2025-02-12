#ifndef _RPILIGHTSCONTROLLER_H_
#define _RPILIGHTSCONTROLLER_H_

#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <atomic>   // for running global variable
#include <cstdlib>  // system
#include <cstring>  // memcpy
#include <unistd.h> // readlink
#include <libgen.h> // dirname

#include "stagekit/StageKitConsts.h"

// Only include StageKitPied dependencies if we're not in the EESD (demo) build
#if !defined(XXSD)
#include "helpers/INI_Handler.h"
#include "helpers/SleepTimer.h"
#include "serial/SerialAdapter.h"
#include "stagekit/USB_ControlRequest.h"
#include "stagekit/StageKitManager.h"
#include "leds/LEDArray.h"
#include "network/RB3E_Network.h"
#include "../dmxified/engine/EventEngine.h"
#include "../dmxified/config/MappingConfig.h"
#include "../dmxified/config/XmlLoader.h"


#endif // !XXSD
#ifdef DEBUG
  #if defined(XXSD)
    #define MSG_RPLC_DEBUG( str ) do { std::cout << "MockRpiLightsController : DEBUG : " << str << std::endl; } while( false )
  #else
    #define MSG_RPLC_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|RpiLightsController : DEBUG : " << str << std::endl; } while( false )
  #endif
#else
  #define MSG_RPLC_DEBUG( str ) do { } while ( false )
#endif

#if defined(XXSD)
  #define MSG_RPLC_ERROR( str ) do { std::cerr << "MockRpiLightsController : ERROR : " << str << std::endl; } while( false )
  #define MSG_RPLC_INFO( str ) do { std::cout << "MockRpiLightsController : INFO : " << str << std::endl; } while( false )
#else
  #define MSG_RPLC_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|RpiLightsController : ERROR : " << str << std::endl; } while( false )
  #define MSG_RPLC_INFO( str ) do { std::cout << std::this_thread::get_id() << "|RpiLightsController : INFO : " << str << std::endl; } while( false )
#endif

#define USB_DIRECTION_IN 0x80
#define ALIVE_CHECK_ITR 1                // Check clients
#define ALIVE_CLEAR_ITR 20               // Remove clients


extern std::atomic<bool> running;


class RpiLightsController {
public:
  RpiLightsController( const char* ini_file );

  ~RpiLightsController();

  void Do_Handle_RumbleData( const uint8_t left_weight, const uint8_t right_weight );

#if !defined(XXSD)
  bool Start();

  long Update( const long time_passed_ms ); // Returns time to sleep in ms

  void Stop();

  bool Restart();
#endif // !XXSD

private:

#if !defined(XXSD)
  void SerialAdapter_Poll();

  void SerialAdapter_HandleControlData();

  void SerialAdapter_HandleOutReport();
  
  void RB3ENetwork_Poll();

  void Stagekit_ResetVariables();

  void StageKit_PollButtons( const long time_passed_ms );

  long Handle_TimeUpdate( const long time_passed_ms );

  bool Handle_StagekitConnect();

  void Handle_StagekitDisconnect();

  bool Handle_SerialConnect();

  void Handle_SerialDisconnect();

  void Handle_RumbleData( const uint8_t left_weight, const uint8_t right_weight );
#endif // !EESD && !QOSD

  void Handle_LEDUpdate( const uint8_t colour, const uint8_t leds );

  void Handle_FogUpdate( bool fog_on_state );

  void Handle_StrobeUpdate( const uint8_t strobe_speed );

  long               m_sleep_time;

#if !defined(XXSD)
  bool               m_dmxified_enabled;
  std::string        m_dmxified_mapping_file;
  std::string        m_qlcplus_websocket_url;
  long               m_qlcplus_connect_sleep_time_ms;
  long               m_qlcplus_send_sleep_time_ms;
  long               m_file_exists_input_watcher_sleep_time_ms;
  MappingConfig	     mDmxifiedMappingConfig;

  SerialAdapter      mSerialAdapter;
  StageKitManager    mStageKitManager;
  LEDArray           mLEDS;
  INI_Handler        mINI_Handler;
  RB3E_Network       mRB3E_Network;
  
  bool               m_rb3e_listener_enabled;
  bool               m_rb3e_sender_enabled;
  std::string        m_rb3e_source_ip;
  uint16_t           m_rb3e_listening_port;  
  std::string        m_rb3e_target_ip;
  uint16_t           m_rb3e_target_port;  

  USB_ControlRequest m_control_request;
  unsigned char*     m_ptr_control_request_data;
  bool               m_serial_connected_to_x360;

  uint8_t            m_stagekit_default_config;
  bool               m_stagekit_blink_on_start;

  uint8_t            m_stagekit_colour_red;
  uint8_t            m_stagekit_colour_green;
  uint8_t            m_stagekit_colour_blue;
  uint8_t            m_stagekit_colour_yellow;
  uint8_t            m_stagekit_strobe_speed;
  long               m_stagekit_strobe_next_on_ms;

  uint8_t            m_colour_red;
  uint8_t            m_colour_green;
  uint8_t            m_colour_blue;
  uint8_t            m_colour_brightness;

  // LED array
  bool               m_leds_enabled;
  std::string*       m_leds_ini;
  uint16_t           m_leds_ini_amount;
  uint8_t            m_leds_ini_number;
  
  bool               m_leds_strobe_enabled;
  uint16_t           m_leds_strobe_rate[ 4 ];
  uint8_t            m_leds_strobe_speed_current;
  long               m_leds_strobe_next_on_ms;

  uint16_t           m_sleeptime_idle;
  uint16_t           m_sleeptime_stagekit;
  uint16_t           m_sleeptime_strobe;

  // NO DATA
  long               m_noserialdata_ms;
  long               m_noserialdata_ms_count;
  long               m_idletime_ms;
  long               m_nodata_ms;
  long               m_nodata_ms_count;
  uint8_t            m_nodata_red;
  uint8_t            m_nodata_green;
  uint8_t            m_nodata_blue;
  uint8_t            m_nodata_brightness;
  
  long               m_button_check_delay;
#endif // !XXSD
};

#endif

