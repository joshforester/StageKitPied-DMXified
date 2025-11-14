#ifndef CONFIG_MAPPINGCONFIGCONSTS_H_
#define CONFIG_MAPPINGCONFIGCONSTS_H_

#include <string>
#include <stdexcept>

// Mapping output types
enum OUTPUT_TYPE {
	stageKitOutput     = 1,
	qlcplusOutput      = 2,
};

// Mapping subtypes for stageKitOutput type
enum STAGEKIT_OUTPUT_SUBTYPE {
	Handle_LEDUpdate    = 1,
	Handle_StrobeUpdate = 2,
	Handle_FogUpdate    = 3,
};

// Mapping subtypes for qlcplusOutput type
enum QLCPLUS_OUTPUT_SUBTYPE {
	simpleDeskChannelSet = 1,
	setFunctionStatus    = 2,
	basicWidgetValueSet  = 3,
};

enum QLCPLUS_OUTPUT_PARAMETER {
	widgetId             = 1,
	value                = 2,
	functionId           = 3,
	status               = 4,
	absoluteDmxAddress   = 5,
};

inline const std::string outputTypeToString(OUTPUT_TYPE type) {
    switch (type) {
        case OUTPUT_TYPE::stageKitOutput:
            return "stageKitOutput";
        case OUTPUT_TYPE::qlcplusOutput:
            return "qlcplusOutput";
        default:
            return "Unknown";
    }
}

inline const OUTPUT_TYPE stringToOutputType(std::string str) {
	if (str == "stageKitOutput") {
		return OUTPUT_TYPE::stageKitOutput;
	} else if (str == "qlcplusOutput") {
		return OUTPUT_TYPE::qlcplusOutput;
	} else {
		throw std::runtime_error("Attempting to convert string " + str + " to output type, but it is not a valid type.");
	}
}

inline const std::string stagekitOutputSubtypeToString(STAGEKIT_OUTPUT_SUBTYPE subtype) {
    switch (subtype) {
        case STAGEKIT_OUTPUT_SUBTYPE::Handle_LEDUpdate:
            return "Handle_LEDUpdate";
        case STAGEKIT_OUTPUT_SUBTYPE::Handle_StrobeUpdate:
            return "Handle_StrobeUpdate";
        case STAGEKIT_OUTPUT_SUBTYPE::Handle_FogUpdate:
            return "Handle_FogUpdate";
        default:
            return "Unknown";
    }
}

inline const STAGEKIT_OUTPUT_SUBTYPE stringToStagekitOutputSubtype(std::string str) {
	if (str == "Handle_LEDUpdate") {
		return STAGEKIT_OUTPUT_SUBTYPE::Handle_LEDUpdate;
	} else if (str == "Handle_StrobeUpdate") {
		return STAGEKIT_OUTPUT_SUBTYPE::Handle_StrobeUpdate;
	} else if (str == "Handle_FogUpdate") {
		return STAGEKIT_OUTPUT_SUBTYPE::Handle_FogUpdate;
	} else {
		throw std::runtime_error("Attempting to convert string " + str + " to stageKitOutput subtype, but it is not a valid subtype.");
	}
}

inline const std::string qlcplusOutputSubtypeToString(QLCPLUS_OUTPUT_SUBTYPE subtype) {
    switch (subtype) {
        case QLCPLUS_OUTPUT_SUBTYPE::simpleDeskChannelSet:
            return "simpleDeskChannelSet";
        case QLCPLUS_OUTPUT_SUBTYPE::setFunctionStatus:
            return "setFunctionStatus";
        case QLCPLUS_OUTPUT_SUBTYPE::basicWidgetValueSet:
            return "basicWidgetValueSet";
        default:
            return "Unknown";
    }
}

inline const QLCPLUS_OUTPUT_SUBTYPE stringToQlcplusOutputSubtype(std::string str) {
	if (str == "simpleDeskChannelSet") {
		return QLCPLUS_OUTPUT_SUBTYPE::simpleDeskChannelSet;
	} else if (str == "setFunctionStatus") {
		return QLCPLUS_OUTPUT_SUBTYPE::setFunctionStatus;
	} else if (str == "basicWidgetValueSet") {
		return QLCPLUS_OUTPUT_SUBTYPE::basicWidgetValueSet;
	} else {
		throw std::runtime_error("Attempting to convert string " + str + " to qlcplusOutput subtype, but it is not a valid subtype.");
	}
}

inline const std::string qlcplusOutputParameterToString(QLCPLUS_OUTPUT_PARAMETER parameter) {
    switch (parameter) {
        case QLCPLUS_OUTPUT_PARAMETER::widgetId:
            return "widgetId";
        case QLCPLUS_OUTPUT_PARAMETER::value:
            return "value";
        case QLCPLUS_OUTPUT_PARAMETER::functionId:
            return "functionId";
        case QLCPLUS_OUTPUT_PARAMETER::status:
            return "status";
        case QLCPLUS_OUTPUT_PARAMETER::absoluteDmxAddress:
            return "absoluteDmxAddress";
        default:
            return "Unknown";
    }
}

#endif /* CONFIG_MAPPINGCONFIGCONSTS_H_ */
