#ifndef _LEDARRAY_H_
#define _LEDARRAY_H_

#include <thread>
#include <cstdint>
#include <iostream>
#include <string>
#include <sstream>

#include "helpers/INI_Handler.h"
#include "leds/LEDGroup.h"
#include "leds/SK9822.h"
#include "stagekit/StageKitConsts.h"


#ifdef DEBUG
  #define MSG_LEDARRAY_DEBUG( str ) do { std::cout << std::this_thread::get_id() << "|LEDArray : DEBUG : " << str << std::endl; } while( false )
#else
  #define MSG_LEDARRAY_DEBUG( str ) do { } while ( false )
#endif

#define MSG_LEDARRAY_ERROR( str ) do { std::cerr << std::this_thread::get_id() << "|LEDArray : ERROR : " << str << std::endl; } while( false )
#define MSG_LEDARRAY_INFO( str ) do { std::cout << std::this_thread::get_id() << "|LEDArray : INFO : " << str << std::endl; } while( false )


class LEDArray {
public:
  LEDArray();

  ~LEDArray();

  bool SetEnabled( const bool enabled );
  
  void TurnOff();

  bool Init( const std::string& device_name, const int led_amount );

  bool LoadSettingsSK( const std::string& ini_file );

  void SetLights( const uint8_t colour, const uint8_t leds );

  void Strobe( const bool on );

  void SetLED( const int led_number, const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t brightness );

  void SetAllLED( const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t brightness );

private:
  void LoadLEDData( INI_Handler* ptrINI_Handler, const std::string& section_name, LEDGroup* ptrLEDGroup, const uint8_t red, const uint8_t green, const uint8_t blue );

  void SetLEDS( const uint8_t leds, LEDGroup theLEDGroups[] );

  SK9822 mSK9822;

  bool m_is_init;

  LEDGroup m_LEDGroups_Red[ 8 ];
  LEDGroup m_LEDGroups_Green[ 8 ];
  LEDGroup m_LEDGroups_Blue[ 8 ];
  LEDGroup m_LEDGroups_Yellow[ 8 ];
  LEDGroup m_LEDGroup_Strobe;

  uint8_t m_SK_LED_Number[ 8 ];

};

#endif
