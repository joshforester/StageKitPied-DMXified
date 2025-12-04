# Currently this makefile lives in StageKitPied/dmxified/demo, so we need to go 2 parent directories up to get project
# root.
PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))../../

$(info PROJECT_ROOT = $(PROJECT_ROOT))

COMPILER              := g++
OBJ_DIR               := objects
CONFIG_SRC_DIR        := ../config
CONFIG_SRC_FILES      := $(wildcard $(CONFIG_SRC_DIR)/*.cpp)
CONFIG_OBJ_FILES      := $(patsubst $(CONFIG_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CONFIG_SRC_FILES))
ENGINE_SRC_DIR        := ../engine
ENGINE_SRC_FILES      := $(wildcard $(ENGINE_SRC_DIR)/*.cpp)
ENGINE_OBJ_FILES      := $(patsubst $(ENGINE_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(ENGINE_SRC_FILES))
OUTPUT_SRC_DIR        := ../output
WEBSOCKET_SRC_DIR     := ../websocket
WEBSOCKET_SRC_FILES   := $(wildcard $(WEBSOCKET_SRC_DIR)/*.cpp)
WEBSOCKET_OBJ_FILES   := $(patsubst $(WEBSOCKET_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(WEBSOCKET_SRC_FILES))
XXSD_SOP_SRC          := ../output/StageKitOutputProcessor.cpp
XXSD_RLC_SRC          := MockRpiLightsController.cpp
FLAGS                 := -g -Wall -std=c++11
LWEBSOCKETPP_PATH     := /usr/include/websocketpp/
LRAPIDXML_PATH        := /usr/include/rapidxml/

all:
	make BUILD_TYPE=IQWD_BUILD iqwd
	make BUILD_TYPE=XCRD_BUILD xcrd
	make BUILD_TYPE=EESD_BUILD eesd
	make BUILD_TYPE=QOSD_BUILD qosd


ifeq ($(BUILD_TYPE), IQWD_BUILD)
IQWD_SRC              := InteractiveQlcplusWebsocketDemo.cpp
IQWD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(IQWD_SRC))
IQWD_OUT              := iqwd
FLAGS                 := $(FLAGS)
INC_PATHS             := -I./ -I$(LWEBSOCKETPP_PATH)
endif

ifeq ($(BUILD_TYPE), XCRD_BUILD)
XCRD_SRC              := XmlConfigReaderDemo.cpp
XCRD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(XCRD_SRC))
XCRD_OUT              := xcrd
FLAGS                 := $(FLAGS)
INC_PATHS             := -I./ -I$(LRAPIDXML_PATH)
endif

ifeq ($(BUILD_TYPE), EESD_BUILD)
EESD_QOP_SRC          := MockQlcplusOutputProcessor.cpp
EESD_SRC              := EventEngineSimulatorDemo.cpp
EESD_OUT              := eesd
FLAGS                 := $(FLAGS) -DDEBUG -DXXSD -DEESD
INC_PATHS             := -I./ -I$(LWEBSOCKETPP_PATH) -I$(LRAPIDXML_PATH) -I$(PROJECT_ROOT)
OUTPUT_SRC_FILES      := $(OUTPUT_SRC_DIR)/StageKitOutputProcessor.cpp
EESD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(EESD_SRC)) \
                         $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(XXSD_RLC_SRC)) \
                         $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(EESD_QOP_SRC)) \
                         $(patsubst $(XXSD_SOP_SRC), objects/StageKitOutputProcessor.o, $(XXSD_SOP_SRC))
EESD_OBJ_FILES        := $(filter-out $(OBJ_DIR)/QlcplusOutputProcessor.o, $(EESD_OBJ_FILES))
endif

ifeq ($(BUILD_TYPE), QOSD_BUILD)
QOSD_QOP_SRC          := ../output/QlcplusOutputProcessor.cpp
QOSD_SRC              := QlcplusOutputSimulatorDemo.cpp
QOSD_OUT              := qosd
FLAGS                 := $(FLAGS) -DDEBUG -DXXSD -DQOSD
INC_PATHS             := -I./ -I$(LWEBSOCKETPP_PATH) -I$(LRAPIDXML_PATH) -I$(PROJECT_ROOT)
OUTPUT_SRC_FILES      := $(OUTPUT_SRC_DIR)/StageKitOutputProcessor.cpp $(OUTPUT_SRC_DIR)/QlcplusOutputProcessor.cpp
QOSD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(QOSD_SRC)) \
                         $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(XXSD_RLC_SRC)) \
                         $(patsubst $(QOSD_QOP_SRC), objects/QlcplusOutputProcessor.o, $(QOSD_QOP_SRC)) \
                         $(patsubst $(XXSD_SOP_SRC), objects/StageKitOutputProcessor.o, $(XXSD_SOP_SRC))
endif


iqwd: $(IQWD_OBJ_FILES) $(WEBSOCKET_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(IQWD_OUT)

xcrd: $(XCRD_OBJ_FILES) $(CONFIG_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(XCRD_OUT)

eesd: $(EESD_OBJ_FILES) $(CONFIG_OBJ_FILES) $(ENGINE_OBJ_FILES) $(OUTPUT_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(EESD_OUT)

qosd: $(QOSD_OBJ_FILES) $(CONFIG_OBJ_FILES) $(ENGINE_OBJ_FILES) $(OUTPUT_OBJ_FILES) $(WEBSOCKET_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(QOSD_OUT)

$(OBJ_DIR)/%.o: %.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $< 

$(OBJ_DIR)/EventEngineSimulatorDemo.o: EventEngineSimulatorDemo.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/QlcplusOutputSimulatorDemo.o: QlcplusOutputSimulatorDemo.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/MockRpiLightsController.o: MockRpiLightsController.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/MockQlcplusOutputProcessor.o: MockQlcplusOutputProcessor.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(CONFIG_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(ENGINE_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(OUTPUT_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(WEBSOCKET_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

print-%  : ; @echo $* = $($*)

clean:
	rm -f $(OBJ_DIR)/*
