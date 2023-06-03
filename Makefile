SOURCE_DIR := ./src
BUILD_DIR := ./build
INCLUDE_DIR := ./include

TARGET := secret

CFLAGS += -Wall -std=c11 -pedantic -pedantic-errors -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -I $(INCLUDE_DIR)
C_SOURCES := $(shell find $(SOURCE_DIR)/ -type f -name "*.c")
OBJ := $(C_SOURCES:%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)/$(SOURCE_DIR)
	$(CC) -c -o $@ $< $(CFLAGS)

all: secret

secret: $(OBJ)
	$(CC) -o $(BUILD_DIR)/$(TARGET) $^ $(CFLAGS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY:
	clean
