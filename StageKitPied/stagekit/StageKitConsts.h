#ifndef _STAGEKITCONSTS_H_
#define _STAGEKITCONSTS_H_

#include <string>
#include <stdexcept>

// Interfaces
// const uint16_t
enum SKINTERFACE {
  SK_INTERFACE_DATA     = 0x01,
  SK_INTERFACE_AUDIO    = 0x02,
  SK_INTERFACE_SECURITY = 0x03,
};

// Buttons : Same as X360 controller
// const uint16_t
enum SKBUTTON {
  SK_BUTTON_SHOULDER_LEFT  = 0x0001,  // Not available on SK Pod
  SK_BUTTON_SHOULDER_RIGHT = 0x0002,  // Not available on SK Pod
  SK_BUTTON_XBOX           = 0x0004,
  SK_BUTTON_UNKNOWN_1      = 0x0008,  // ??
  SK_BUTTON_A              = 0x0010,
  SK_BUTTON_B              = 0x0020,
  SK_BUTTON_X              = 0x0040,
  SK_BUTTON_Y              = 0x0080,
  SK_BUTTON_UP             = 0x0100,
  SK_BUTTON_DOWN           = 0x0200,
  SK_BUTTON_LEFT           = 0x0400,
  SK_BUTTON_RIGHT          = 0x0800,
  SK_BUTTON_START          = 0x1000,
  SK_BUTTON_BACK           = 0x2000,
  SK_BUTTON_HAT_LEFT       = 0x4000,  // Not available on SK Pod
  SK_BUTTON_HAT_RIGHT      = 0x8000,  // Not available on SK Pod
};

// Status LEDs
// const uint8_t
enum SKSTATUSLEDS {
  SK_STATUS_OFF        = 0x00, // OFF
  SK_STATUS_BLINK_ALL  = 0x01, // ALL - Used to blink all LEDs
  SK_STATUS_BLINK_1    = 0x02, // LED1
  SK_STATUS_BLINK_2    = 0x03, // LED2
  SK_STATUS_BLINK_3    = 0x04, // LED3
  SK_STATUS_BLINK_4    = 0x05, // LED4
  SK_STATUS_ON_1       = 0x06, // LED1
  SK_STATUS_ON_2       = 0x07, // LED2
  SK_STATUS_ON_3       = 0x08, // LED3
  SK_STATUS_ON_4       = 0x09, // LED4
  SK_STATUS_ROTATE     = 0x0A,
  SK_STATUS_BLINK_FAST = 0x0B,
  SK_STATUS_BLINK_SLOW = 0x0C,
  SK_STATUS_ALTERNATE  = 0x0D,
};

// Rumble Data (Small Weight: Left 8 bit + Large Weight: Right 8 bit)
// Stage kit has 4 colours and 8 leds for each colour.
// It contains no pre-determined patterns, each led is switched on & off with the bit-packed rumble data.
enum SKRUMBLEDATA {
  SK_NONE           = 0x00,  // 0000 0000 - Left weight
  SK_LED_1          = 0x01,  // 0000 0001 - Left weight
  SK_LED_2          = 0x02,  // 0000 0010 - Left weight
  SK_LED_3          = 0x04,  // 0000 0100 - Left weight
  SK_LED_4          = 0x08,  // 0000 1000 - Left weight
  SK_LED_5          = 0x10,  // 0001 0000 - Left weight
  SK_LED_6          = 0x20,  // 0010 0000 - Left weight
  SK_LED_7          = 0x40,  // 0100 0000 - Left weight
  SK_LED_8          = 0x80,  // 1000 0000 - Left weight
  SK_FOG_ON         = 0x01,  // 0000 0001 - Right weight
  SK_FOG_OFF        = 0x02,  // 0000 0010 - Right weight
  SK_STROBE_SPEED_1 = 0x03,  // 0000 0011 - Right weight
  SK_STROBE_SPEED_2 = 0x04,  // 0000 0100 - Right weight
  SK_STROBE_SPEED_3 = 0x05,  // 0000 0101 - Right weight
  SK_STROBE_SPEED_4 = 0x06,  // 0000 0110 - Right weight
  SK_STROBE_OFF     = 0x07,  // 0000 0111 - Right weight
  SK_LED_BLUE       = 0x20,  // 0010 0000 - Right weight
  SK_LED_GREEN      = 0x40,  // 0100 0000 - Right weight
  SK_LED_YELLOW     = 0x60,  // 0110 0000 - Right weight
  SK_LED_RED        = 0x80,  // 1000 0000 - Right weight
  SK_LED_MASK       = 0xE0,  // 1110 0000 - Right weight
  SK_ALL_OFF        = 0xFF,  // 1111 1111 - Right weight
  SERIAL_RESTART    = 0xFE,  // made up; not an actual SK event
  IDLE_ON           = 0xFD,  // made up; not an actual SK event
  IDLE_OFF          = 0xFC,  // made up; not an actual SK event
};


inline const std::string skRumbleDataTypeToString(const SKRUMBLEDATA type) {
    switch (type) {
        case SKRUMBLEDATA::SK_FOG_ON:
            return "SK_FOG_ON";
        case SKRUMBLEDATA::SK_FOG_OFF:
            return "SK_FOG_OFF";
        case SKRUMBLEDATA::SK_STROBE_SPEED_1:
            return "SK_STROBE_SPEED_1";
        case SKRUMBLEDATA::SK_STROBE_SPEED_2:
            return "SK_STROBE_SPEED_2";
        case SKRUMBLEDATA::SK_STROBE_SPEED_3:
            return "SK_STROBE_SPEED_3";
        case SKRUMBLEDATA::SK_STROBE_SPEED_4:
            return "SK_STROBE_SPEED_4";
        case SKRUMBLEDATA::SK_STROBE_OFF:
            return "SK_STROBE_OFF";
        case SKRUMBLEDATA::SK_LED_BLUE:
            return "SK_LED_BLUE";
        case SKRUMBLEDATA::SK_LED_GREEN:
            return "SK_LED_GREEN";
        case SKRUMBLEDATA::SK_LED_YELLOW:
            return "SK_LED_YELLOW";
        case SKRUMBLEDATA::SK_LED_RED:
            return "SK_LED_RED";
        case SKRUMBLEDATA::SK_LED_MASK:
            return "SK_LED_MASK";
        case SKRUMBLEDATA::SK_ALL_OFF:
            return "SK_ALL_OFF";
        case SKRUMBLEDATA::SERIAL_RESTART:
            return "SERIAL_RESTART";
        case SKRUMBLEDATA::IDLE_ON:
            return "IDLE_ON";
        case SKRUMBLEDATA::IDLE_OFF:
            return "IDLE_OFF";
        default:
            return "Unknown";
    }
}

inline const SKRUMBLEDATA stringToSkRumbleData(std::string str) {
	if (str == "SK_FOG_ON") {
		return SKRUMBLEDATA::SK_FOG_ON;
	} else if (str == "SK_FOG_OFF") {
		return SKRUMBLEDATA::SK_FOG_OFF;
	} else if (str == "SK_STROBE_SPEED_1") {
		return SKRUMBLEDATA::SK_STROBE_SPEED_1;
	} else if (str == "SK_STROBE_SPEED_2") {
		return SKRUMBLEDATA::SK_STROBE_SPEED_2;
	} else if (str == "SK_STROBE_SPEED_3") {
		return SKRUMBLEDATA::SK_STROBE_SPEED_3;
	} else if (str == "SK_STROBE_SPEED_4") {
		return SKRUMBLEDATA::SK_STROBE_SPEED_4;
	} else if (str == "SK_STROBE_OFF") {
		return SKRUMBLEDATA::SK_STROBE_OFF;
	} else if (str == "SK_LED_BLUE") {
		return SKRUMBLEDATA::SK_LED_BLUE;
	} else if (str == "SK_LED_GREEN") {
		return SKRUMBLEDATA::SK_LED_GREEN;
	} else if (str == "SK_LED_YELLOW") {
		return SKRUMBLEDATA::SK_LED_YELLOW;
	} else if (str == "SK_LED_RED") {
		return SKRUMBLEDATA::SK_LED_RED;
	} else if (str == "SK_LED_MASK") {
		return SKRUMBLEDATA::SK_LED_MASK;
	} else if (str == "SK_ALL_OFF") {
		return SKRUMBLEDATA::SK_ALL_OFF;
	} else if (str == "SERIAL_RESTART") {
		return SKRUMBLEDATA::SERIAL_RESTART;
	} else if (str == "IDLE_ON") {
		return SKRUMBLEDATA::IDLE_ON;
	} else if (str == "IDLE_OFF") {
		return SKRUMBLEDATA::IDLE_OFF;
	} else {
		throw std::runtime_error("Attempting to convert string " + str + " to SKRUMBLEDATA, but it is not a valid type.");
	}
}

inline const SKRUMBLEDATA boolToSkFogRumbleData(bool fog_on_status) {
    if (fog_on_status) {
    	return SKRUMBLEDATA::SK_FOG_ON;
    } else {
    	return SKRUMBLEDATA::SK_FOG_OFF;
    }
}

inline const SKRUMBLEDATA intToSkStrobeSpeedRumbleData(const uint8_t strobe_speed) {
	switch (strobe_speed) {
		case 0:
			return SKRUMBLEDATA::SK_STROBE_OFF;
		case 1:
			return SKRUMBLEDATA::SK_STROBE_SPEED_1;
		case 2:
			return SKRUMBLEDATA::SK_STROBE_SPEED_2;
		case 3:
			return SKRUMBLEDATA::SK_STROBE_SPEED_3;
		case 4:
			return SKRUMBLEDATA::SK_STROBE_SPEED_4;
		default:
			throw std::runtime_error("Attempting to convert uint8_t " + std::to_string(strobe_speed) + " to SKRUMBLEDATA::SK_STROBE speed, but it is not a valid speed.");
			return SKRUMBLEDATA::SK_NONE;
	}
}

#endif
