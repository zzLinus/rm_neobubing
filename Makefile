UNAME_S = $(shell uname -s)
WORK_DIR  = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build
MAKE = make
MV = mv

SERIAL_DIR= $(shell find ./3rdparty -name "src-serial")

CC = g++
CPPFLAGS = -std=c++20 -O0 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CPPFLAGS += -Wno-pointer-arith -Wno-newline-eof -Wno-unused-parameter -Wno-gnu-statement-expression
CPPFLAGS += -Wno-gnu-compound-literal-initializer -Wno-gnu-zero-variadic-macro-arguments
CPPFLAGS += -I$(WORK_DIR)/include

# NOTE: turn on debug here
CPPFLAGS += -D__DEBUG__

# FIXME: imtui dependency linking
CPPFLAGS += -I "./3rdparty/include"
CPPFLAGS += -L "./3rdparty/lib"

LDFLAGS += -lm -lpthread -ldl -lrt -lserial

#LDFLAGS = `pkg-config sdl --libs`

SRC = $(wildcard src/*.cc) $(wildcard src/**/*.cc)
OBJ = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SRC))))
BIN = rx78-2

.PHONY: all clean

all: dirs $(BIN)

dirs:
	mkdir -p $(BUILD_DIR)

run: all
	$(BUILD_DIR)/$(BIN)

serial: $(SERIAL_DIR)
	$(MAKE) -C $< -j8
	$(MV) $(SERIAL_DIR)/build/libserial.a 3rdparty/lib

$(BIN): $(OBJ) serial
	$(CC) -o $(BUILD_DIR)/$(BIN) $(OBJ) $(CPPFLAGS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(dir $@) && echo + CC $<
	$(CC) -o $@ -c $< $(CPPFLAGS)

clean-serial: $(SERIAL_DIR)
	$(MAKE) -C $< clean

clean: clean-serial
	rm 3rdparty/lib/libserial.a
	rm -rf $(BUILD_DIR)/$(BIN) $(OBJ)
