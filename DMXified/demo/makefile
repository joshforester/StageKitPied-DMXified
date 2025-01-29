# Currently this makefile lives in DMXified/demo, so we need to go 2 parent directories up to get project root.
PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))../..

COMPILER              := g++
OBJ_DIR               := objects
CONFIG_SRC_DIR        := ../config
CONFIG_SRC_FILES      := $(wildcard $(CONFIG_SRC_DIR)/*.cpp)
CONFIG_OBJ_FILES      := $(patsubst $(CONFIG_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CONFIG_SRC_FILES))
ENGINE_SRC_DIR        := ../engine
ENGINE_SRC_FILES      := $(wildcard $(ENGINE_SRC_DIR)/*.cpp)
ENGINE_OBJ_FILES      := $(patsubst $(ENGINE_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(ENGINE_SRC_FILES))
WEBSOCKET_SRC_DIR     := ../websocket
WEBSOCKET_SRC_FILES   := $(wildcard $(WEBSOCKET_SRC_DIR)/*.cpp)
WEBSOCKET_OBJ_FILES   := $(patsubst $(WEBSOCKET_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(WEBSOCKET_SRC_FILES))
IQWD_SRC              := InteractiveQlcplusWebsocketDemo.cpp
IQWD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(IQWD_SRC))
IQWD_OUT              := iqwd
XCRD_SRC              := XmlConfigReaderDemo.cpp
XCRD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(XCRD_SRC))
XCRD_OUT              := xcrd
EESD_RLC_SRC          := MockRpiLightsController.cpp
EESD_QOP_SRC          := MockQlcplusOutputProcessor.cpp
EESD_SRC              := EventEngineSimulatorDemo.cpp
EESD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(EESD_SRC)) $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(EESD_RLC_SRC))  $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(EESD_QOP_SRC))
EESD_OUT              := eesd
FLAGS                 := -g -Wall -std=c++11
EESD_FLAGS            := $(FLAGS) -DDEBUG
LWEBSOCKETPP_PATH     := -I/usr/include/websocketpp/
LRAPIDXML_PATH        := -I/usr/include/rapidxml/
LPTHREAD_FLAG         := -lpthread
LSSL_FLAG             := -lssl -lcrypto
INC_PATHS             := -I./ $(LWEBSOCKETPP_PATH) $(LRAPIDXML_PATH)
EESD_INC_PATHS        := -I$(PROJECT_ROOT)/StageKitPied

all: iqwd xcrd eesd

iqwd: $(IQWD_OBJ_FILES) $(WEBSOCKET_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LWEBSOCKETPP_PATH) $^ -o $(IQWD_OUT) $(LSSL_FLAG)

xcrd: $(XCRD_OBJ_FILES) $(CONFIG_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(XCRD_OUT)

eesd: $(EESD_OBJ_FILES) $(CONFIG_OBJ_FILES) $(ENGINE_OBJ_FILES)
	$(COMPILER) $(EESD_FLAGS) -DEESD $(EESD_INC_PATHS) $^ -o $(EESD_OUT)

config: $(CONFIG_OBJ_FILES)

engine: $(ENGINE_OBJ_FILES) $(CONFIG_OBJ_FILES)

websocket: $(WEBSOCKET_OBJ_FILES)

$(OBJ_DIR)/%.o: %.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $< 

$(OBJ_DIR)/EventEngineSimulatorDemo.o: EventEngineSimulatorDemo.cpp
	$(COMPILER) $(FLAGS) -DEESD $(INC_PATHS) $(EESD_INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/MockRpiLightsController.o: MockRpiLightsController.cpp
	$(COMPILER) $(EESD_FLAGS) -DEESD $(EESD_INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/MockQlcplusOutputProcessor.o: MockQlcplusOutputProcessor.cpp
	$(COMPILER) $(EESD_FLAGS) -DEESD $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(CONFIG_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LRAPIDXML_PATH) -c -o $@ $<

$(OBJ_DIR)/%.o: $(ENGINE_SRC_DIR)/%.cpp
	$(COMPILER) $(EESD_FLAGS) -DEESD $(EESD_INC_PATHS) $(LRAPIDXML_PATH) -c -o $@ $<

$(OBJ_DIR)/%.o: $(WEBSOCKET_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LWEBSOCKETPP_PATH) -c -o $@ $<

print-%  : ; @echo $* = $($*)

clean:
	rm -f $(IQWD_OBJ_FILES) $(XCRD_OBJ_FILES) $(EESD_OBJ_FILES) $(CONFIG_OBJ_FILES) $(ENGINE_OBJ_FILES) $(WEBSOCKET_OBJ_FILES)
