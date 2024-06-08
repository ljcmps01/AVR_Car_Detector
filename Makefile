# Directories
TOOLS_DIR = ${TOOLS_PATH}
AVR_GCC_ROOT_DIR = $(TOOLS_DIR)/avr8-gnu-toolchain-linux_x86_64
AVR_GCC_BIN_DIR = $(AVR_GCC_ROOT_DIR)/bin
AVR_GCC_INCLUDE_DIR = $(AVR_GCC_ROOT_DIR)/include
INCLUDE_DIRS = $(AVR_GCC_INCLUDE_DIR)\
				./src \
				./external
LIB_DIRS = $(AVR_GCC_INCLUDE_DIR)

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Toolchain
CC = $(AVR_GCC_BIN_DIR)/avr-gcc
RM = rm
CPPCHECK = cppcheck

# Files
TARGET = $(BIN_DIR)/main.ELF

##source files
SOURCES_WITH_HEADERS = \
	src/drivers/myUSART.c\

MAIN_FILE = src/main.c

SOURCES =\
	$(MAIN_FILE)\
	$(SOURCES_WITH_HEADERS)

# blink.c

HEADERS = \
	$(SOURCES_WITH_HEADERS:.c=.h) \

OBJECT_NAMES = $(SOURCES:.c=.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)/%,$(OBJECT_NAMES))

# Flags
MCU = atmega328p

WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=$(MCU) $(WFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) -Og -g
LDFLAGS = -mmcu=$(MCU) $(addprefix -L, $(LIB_DIRS)) -O


# Build
## Linking
$(TARGET): $(OBJECTS) $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ -o $@

## Compile
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

##Phonies
.PHONY: all clean flash cppcheck

all: $(TARGET)

clean:
	$(RM) -r build

flash: $(TARGET)
	sudo avrdude -c arduino -P ch340 -b 115200 -p $(MCU) -D -U flash:w:$(TARGET):e

cppcheck: 
	  @$(CPPCHECK) --quiet --enable=all --error-exitcode=1\
	  --inline-suppr \
	  --suppress=missingIncludeSystem \
	  -i $(INCLUDE_DIRS) \
		$(SOURCES) \
	  --check-config