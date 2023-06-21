SOURCE_DIR := ./src
BUILD_DIR := ./build
INCLUDE_DIR := ./include

TARGET := ss

CFLAGS += -Wall -std=gnu11 -pedantic -pedantic-errors -Wextra -Werror -Wno-unused-parameter
CFLAGS += -fno-omit-frame-pointer -Wno-implicit-fallthrough -MMD -I $(INCLUDE_DIR)
C_SOURCES := $(shell find $(SOURCE_DIR)/ -type f -name "*.c")
OBJ := $(C_SOURCES:%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: %.c Makefile
	mkdir -p $(BUILD_DIR)/$(SOURCE_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(TARGET)

debug:
	CFLAGS=-g make clean all

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: clean all debug

-include $(OBJ:%.o=%.d)
