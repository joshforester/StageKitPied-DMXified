#include "../../controller/RpiLightsController.h"

#include "../config/MappingConfigConsts.h"

RpiLightsController::RpiLightsController( const char* ini_file ) {
};

RpiLightsController::~RpiLightsController() {
};

void RpiLightsController::Do_Handle_RumbleData( const uint8_t left_weight, const uint8_t right_weight ) {
  switch( right_weight ) {
    case SKRUMBLEDATA::SK_LED_RED:
      this->Handle_LEDUpdate( left_weight, SKRUMBLEDATA::SK_LED_RED);
      break;
   case SKRUMBLEDATA::SK_LED_GREEN:
      this->Handle_LEDUpdate( left_weight, SKRUMBLEDATA::SK_LED_GREEN);
      break;
    case SKRUMBLEDATA::SK_LED_BLUE:
      this->Handle_LEDUpdate( left_weight, SKRUMBLEDATA::SK_LED_BLUE);
      break;
    case SKRUMBLEDATA::SK_LED_YELLOW:
      this->Handle_LEDUpdate( left_weight, SKRUMBLEDATA::SK_LED_YELLOW);
      break;
    case SKRUMBLEDATA::SK_FOG_ON:
      this->Handle_FogUpdate( true );
      break;
    case SKRUMBLEDATA::SK_FOG_OFF:
      this->Handle_FogUpdate( false );
      break;
    case SKRUMBLEDATA::SK_STROBE_OFF:
      this->Handle_StrobeUpdate( 0 );
      break;
    case SKRUMBLEDATA::SK_STROBE_SPEED_1:
      this->Handle_StrobeUpdate( 1 );
      break;
    case SKRUMBLEDATA::SK_STROBE_SPEED_2:
      this->Handle_StrobeUpdate( 2 );
      break;
    case SKRUMBLEDATA::SK_STROBE_SPEED_3:
      this->Handle_StrobeUpdate( 3 );
      break;
    case SKRUMBLEDATA::SK_STROBE_SPEED_4:
      this->Handle_StrobeUpdate( 4 );
      break;
    case SKRUMBLEDATA::SK_ALL_OFF:
      this->Handle_LEDUpdate( SKRUMBLEDATA::SK_NONE, SKRUMBLEDATA::SK_ALL_OFF );
      this->Handle_StrobeUpdate( 0 );
      this->Handle_FogUpdate( false );
      break;
    default:
      MSG_RPLC_INFO( "Unhandled stagekit data received : " << right_weight );
      break;
  }

}

void RpiLightsController::Handle_LEDUpdate( const uint8_t leds, const uint8_t colour ) {
	const std::string message = "Mock processing output: " +
			stagekitOutputSubtypeToString(STAGEKIT_OUTPUT_SUBTYPE::Handle_LEDUpdate) + ":" +
			skRumbleDataTypeToString(static_cast<SKRUMBLEDATA>(colour));
	MSG_RPLC_DEBUG( message );
};

void RpiLightsController::Handle_FogUpdate( const bool fog_on_state ) {
	const std::string message = "Mock processing output: " +
			stagekitOutputSubtypeToString(STAGEKIT_OUTPUT_SUBTYPE::Handle_FogUpdate) + ":" +
			skRumbleDataTypeToString(boolToSkFogRumbleData(fog_on_state));
	MSG_RPLC_DEBUG( message );
};

void RpiLightsController::Handle_StrobeUpdate( const uint8_t strobe_speed ) {
	const std::string message = "Mock processing output: " +
			stagekitOutputSubtypeToString(STAGEKIT_OUTPUT_SUBTYPE::Handle_StrobeUpdate) + ":" +
			skRumbleDataTypeToString(intToSkStrobeSpeedRumbleData(strobe_speed));
	MSG_RPLC_DEBUG( message );
};
