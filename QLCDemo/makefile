PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

#OBJS = QLCDemo.o
#
#ifeq ($(BUILD_MODE),debug)
#	CFLAGS += -g
#else ifeq ($(BUILD_MODE),run)
#	CFLAGS += -O2
#else ifeq ($(BUILD_MODE),linuxtools)
#	CFLAGS += -g -pg -fprofile-arcs -ftest-coverage
#	LDFLAGS += -pg -fprofile-arcs -ftest-coverage
#	EXTRA_CLEAN += QLCDemo.gcda QLCDemo.gcno $(PROJECT_ROOT)gmon.out
#	EXTRA_CMDS = rm -rf QLCDemo.gcda
#else
#    $(error Build mode $(BUILD_MODE) not supported by this Makefile)
#endif
#
#all:	QLCDemo
#
#QLCDemo:	$(OBJS)
#	$(CXX) $(LDFLAGS) -o $@ $^
#	$(EXTRA_CMDS)
#
#%.o:	$(PROJECT_ROOT)%.cpp
#	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<
#
#%.o:	$(PROJECT_ROOT)%.c
#	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
#
#clean:
#	rm -fr QLCDemo $(OBJS) $(EXTRA_CLEAN)
	
	
COMPILER_G++          := g++
COMPILER_ARM          := arm-linux-gnueabihf-gcc

COMPILER              := g++
OBJ_DIR               := objects
CONFIG_SRC_DIR        := config
CONFIG_SRC_FILES      := $(wildcard $(CONFIG_SRC_DIR)/*.cpp)
CONFIG_OBJ_FILES      := $(patsubst $(CONFIG_SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CONFIG_SRC_FILES))
QLCDEMO_SRC           := QLCDemo.cpp
QLCDEMO_OBJ_FILES     := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(QLCDEMO_SRC))
QLCDEMO_OUT           := qlcdemo
FLAGS                 := -g -Wall -std=c++11
LWEBSOCKETPP_PATH     := -I/usr/include/websocketpp/
LPTHREAD_FLAG         := -lpthread
LSSL_FLAG             := -lssl -lcrypto
INC_PATHS             := -I./ $(LWEBSOCKETPP_PATH)

all: qlcdemo

qlcdemo: $(CONFIG_OBJ_FILES) $(QLCDEMO_OBJ_FILES)
	$(COMPILER) $(FLAGS) $(INC_PATHS) $^ -o $(QLCDEMO_OUT) $(LSSL_FLAG)

config: $(CONFIG_OBJ_FILES)

$(OBJ_DIR)/%.o: %.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $< 

$(OBJ_DIR)/%.o: $(CONFIG_SRC_DIR)/%.cpp
	$(COMPILER) $(FLAGS) $(INC_PATHS) -c -o $@ $<

print-%  : ; @echo $* = $($*)

clean:
	rm -f $(CONFIG_OBJ_FILES) $(QLCDEMO_OBJ_FILES)
