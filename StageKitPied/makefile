COMPILER_G++          := g++
COMPILER_ARM          := arm-linux-gnueabihf-gcc

COMPILER              := g++
OBJ_DIR               := objects
CONFIG_SRC_DIR        := dmxified/config
CONFIG_SRC_FILES      := $(wildcard $(CONFIG_SRC_DIR)/*.cpp)
CONFIG_OBJ_FILES      := $(patsubst $(CONFIG_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CONFIG_SRC_FILES))
CONTROLLER_SRC_DIR    := controller
CONTROLLER_SRC_FILES  := $(wildcard $(CONTROLLER_SRC_DIR)/*.cpp)
CONTROLLER_OBJ_FILES  := $(patsubst $(CONTROLLER_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CONTROLLER_SRC_FILES))
ENGINE_SRC_DIR        := dmxified/engine
ENGINE_SRC_FILES      := $(wildcard $(ENGINE_SRC_DIR)/*.cpp)
ENGINE_OBJ_FILES      := $(patsubst $(ENGINE_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(ENGINE_SRC_FILES))
HELPERS_SRC_DIR       := helpers
HELPERS_SRC_FILES     := $(wildcard $(HELPERS_SRC_DIR)/*.cpp)
HELPERS_OBJ_FILES     := $(patsubst $(HELPERS_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(HELPERS_SRC_FILES))
LEDS_SRC_DIR          := leds
LEDS_SRC_FILES        := $(wildcard $(LEDS_SRC_DIR)/*.cpp)
LEDS_OBJ_FILES        := $(patsubst $(LEDS_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(LEDS_SRC_FILES))
NETWORK_SRC_DIR       := network
NETWORK_SRC_FILES     := $(wildcard $(NETWORK_SRC_DIR)/*.cpp)
NETWORK_OBJ_FILES     := $(patsubst $(NETWORK_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(NETWORK_SRC_FILES))
OUTPUT_SRC_DIR        := dmxified/output
OUTPUT_SRC_FILES      := $(wildcard $(OUTPUT_SRC_DIR)/*.cpp)
OUTPUT_OBJ_FILES      := $(patsubst $(OUTPUT_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(OUTPUT_SRC_FILES))
SERIAL_SRC_DIR        := serial
SERIAL_SRC_FILES      := $(wildcard $(SERIAL_SRC_DIR)/*.cpp)
SERIAL_OBJ_FILES      := $(patsubst $(SERIAL_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SERIAL_SRC_FILES))
STAGEKIT_SRC_DIR      := stagekit
STAGEKIT_SRC_FILES    := $(wildcard $(STAGEKIT_SRC_DIR)/*.cpp)
STAGEKIT_OBJ_FILES    := $(patsubst $(STAGEKIT_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(STAGEKIT_SRC_FILES))
WEBSOCKET_SRC_DIR     := dmxified/websocket
WEBSOCKET_SRC_FILES   := $(wildcard $(WEBSOCKET_SRC_DIR)/*.cpp)
WEBSOCKET_OBJ_FILES   := $(patsubst $(WEBSOCKET_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(WEBSOCKET_SRC_FILES))
SKP_SRC               := stagekitpied.cpp
SKP_OBJ_FILES         := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SKP_SRC))
SKP_OUT               := skp
FLAGS                 := -g -Wall -std=c++11 #-DDEBUG
LRAPIDXML_PATH        := -I/usr/include/rapidxml/
LWEBSOCKETPP_PATH     := -I/usr/include/websocketpp/
LUSB_PATH             := -I/usr/include/libusb-1.0/
LUSB_FLAG             := -lusb-1.0
LACL_FLAG			  := -lacl
LPTHREAD_FLAG         := -lpthread
INC_PATHS             := -I./

all: skp

skp: $(CONFIG_OBJ_FILES) $(WEBSOCKET_OBJ_FILES) $(HELPERS_OBJ_FILES) $(STAGEKIT_OBJ_FILES) $(LEDS_OBJ_FILES) $(NETWORK_OBJ_FILES) $(SERIAL_OBJ_FILES) $(CONTROLLER_OBJ_FILES) $(OUTPUT_OBJ_FILES) $(ENGINE_OBJ_FILES) $(SKP_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(SKP_OUT) $(LUSB_FLAG) $(LACL_FLAG) 

config: $(CONFIG_OBJ_FILES)

engine: $(ENGINE_OBJ_FILES)

output: $(OUTPUT_OBJ_FILES)

websocket: $(WEBSOCKET_OBJ_FILES)

helpers: $(HELPERS_OBJ_FILES)

leds: $(LEDS_OBJ_FILES)

network: $(NETWORK_OBJ_FILES)

serial: $(SERIAL_OBJ_FILES)

stagekit: $(STAGEKIT_OBJ_FILES)

controller: $(CONTROLLER_OBJ_FILES)

$(OBJ_DIR)/%.o: %.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LRAPIDXML_PATH) $(LUSB_PATH) -c -o $@ $< 

$(OBJ_DIR)/%.o: $(CONFIG_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LRAPIDXML_PATH) -c -o $@ $<

$(OBJ_DIR)/%.o: $(ENGINE_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LUSB_PATH) $(LRAPIDXML_PATH) $(LWEBSOCKETPP_PATH) -c -o $@ $<

$(OBJ_DIR)/%.o: $(OUTPUT_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LUSB_PATH) $(LRAPIDXML_PATH) $(LWEBSOCKETPP_PATH) -c -o $@ $<

$(OBJ_DIR)/%.o: $(WEBSOCKET_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LWEBSOCKETPP_PATH) -c -o $@ $<

$(OBJ_DIR)/%.o: $(HELPERS_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(LEDS_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(NETWORK_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<
  
$(OBJ_DIR)/%.o: $(SERIAL_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(STAGEKIT_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LUSB_PATH) -c -o $@ $<

$(OBJ_DIR)/%.o: $(CONTROLLER_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) $(LUSB_PATH) $(LRAPIDXML_PATH) -c -o $@ $<

print-%  : ; @echo $* = $($*)

clean:
	rm -f $(OBJ_DIR)/*