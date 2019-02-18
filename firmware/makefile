DEVICE_PORT=ttyACM1
F_CPU = 16000000
F_USB = 8000000
ARCH = AVR8
MCU = atmega32u4
LUFA_PATH = lufa/LUFA

# Compiler
GCC = avr-gcc

# Directories used for input and output
SRCDIR = .
BUILDDIR = build
EXEDIR = bin
INCLUDEDIR = .

# Name to output the built file to
OUTPUT_NAME = haptyk

# Where the sources are located
SRCS = $(wildcard $(SRCDIR)/*.c)
SRCS += $(wildcard $(SRCDIR)/*/*.c)

# Where the compiled objects are located
OBJS = $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(SRCS))

# Flags for the compiler
GCC_FLAGS += -Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -mmcu=atmega32u4 -c -Ilufa -DF_CPU=$(F_CPU) -DF_USB=$(F_USB) -DUSE_LUFA_CONFIG_HEADER -I.

# Minimake files used to dependency rebuilding
DEPS := $(OBJS:.o=.d)
-include $(DEPS)

# Linking all the .o files and with the libs
build: $(OBJS)
	$(GCC) $^ -mmcu=atmega32u4 -o ./bin/$(OUTPUT_NAME)
	avr-objcopy -O ihex -R .eeprom ./bin/$(OUTPUT_NAME) ./bin/$(OUTPUT_NAME).hex

# Compiling all the .c files into .o files
$(OBJS): $(BUILDDIR)/%.o : $(SRCDIR)/%.c
	$(GCC) $(GCC_FLAGS) -o "$@" "$<"

# Running the created exe
.PHONY: run
run:
	avrdude -F -V -c avr109 -p m32U4 -P /dev/$(DEVICE_PORT) -b 115200 -U flash:w:./bin/$(OUTPUT_NAME).hex
	
# Cleaning everything up
.PHONY: clean
clean:
	rm -rf $(EXEDIR)/*
	rm -rf $(BUILDDIR)/*
	
.PHONY: port
port:
	dterm /dev/${DEVICE_PORT}
