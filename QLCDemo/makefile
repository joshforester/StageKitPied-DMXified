PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

COMPILER_G++          := g++
COMPILER_ARM          := arm-linux-gnueabihf-gcc

COMPILER              := g++
OBJ_DIR               := objects
CONFIG_SRC_DIR        := config
CONFIG_SRC_FILES      := $(wildcard $(CONFIG_SRC_DIR)/*.cpp)
CONFIG_OBJ_FILES      := $(patsubst $(CONFIG_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CONFIG_SRC_FILES))
WEBSOCKET_SRC_DIR     := websocket
WEBSOCKET_SRC_FILES   := $(wildcard $(WEBSOCKET_SRC_DIR)/*.cpp)
WEBSOCKET_OBJ_FILES   := $(patsubst $(WEBSOCKET_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(WEBSOCKET_SRC_FILES))
IQWD_SRC              := InteractiveQlcplusWebsocketDemo.cpp
IQWD_OBJ_FILES        := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(IQWD_SRC))
IQWD_OUT              := iqwd
FLAGS                 := -g -Wall -std=c++11
LWEBSOCKETPP_PATH     := -I/usr/include/websocketpp/
LPTHREAD_FLAG         := -lpthread
LSSL_FLAG             := -lssl -lcrypto
INC_PATHS             := -I./ $(LWEBSOCKETPP_PATH)

all: iqwd

iqwd: $(IQWD_OBJ_FILES) $(CONFIG_OBJ_FILES) $(WEBSOCKET_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(IQWD_OUT) $(LSSL_FLAG)

config: $(CONFIG_OBJ_FILES)

websocket: $(WEBSOCKET_OBJ_FILES)

$(OBJ_DIR)/%.o: %.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $< 

$(OBJ_DIR)/%.o: $(CONFIG_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: $(WEBSOCKET_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

print-%  : ; @echo $* = $($*)

clean:
	rm -f $(IQWD_OBJ_FILES) $(CONFIG_OBJ_FILES) $(WEBSOCKET_OBJ_FILES)
