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
	src/drivers/avr_w5100.c\
	external/w5100/w5100.c

ifndef TEST
SOURCES =\
	src/main.c\
	$(SOURCES_WITH_HEADERS)

else
SOURCES =\
	src/test/$(TEST).c\
	$(SOURCES_WITH_HEADERS)

endif

HEADERS = \
	$(SOURCES_WITH_HEADERS:.c=.h) \

OBJECT_NAMES = $(SOURCES:.c=.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)/%,$(OBJECT_NAMES))

# Defines
DEFINES = -DF_CPU=16000000UL

# Flags
MCU = atmega328p

WFLAGS = -Wall -Wextra -Werror -Wshadow
CFLAGS = -mmcu=$(MCU) $(WFLAGS) $(addprefix -I, $(INCLUDE_DIRS)) -Og -g $(DEFINES)
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
	sudo avrdude -c arduino -P /dev/ttyACM0 -b 115200 -p $(MCU) -D -U flash:w:$(TARGET):e

cppcheck: 
	  @$(CPPCHECK) --quiet --enable=all --error-exitcode=1\
	  --inline-suppr \
	  --suppress=missingIncludeSystem \
	  -i $(INCLUDE_DIRS) \
		$(SOURCES) \
	  --check-config