#
#             LUFA Library
#     Copyright (C) Dean Camera, 2017.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU				= atmega32u4
ARCH         	= AVR8
BOARD        	= USBKEY
F_CPU        	= 8000000
F_USB        	= $(F_CPU)
OPTIMIZATION 	= s
TARGET       	= haptyk
SRC          	= $(TARGET).c $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS) \
				  Descriptors.c usb_driver.c bt.c spi.c sdep.c print.c

LUFA_PATH    	= ./lufa/LUFA
CC_FLAGS     	= -DUSE_LUFA_CONFIG_HEADER -IConfig/
LD_FLAGS     	=

DEVICE_PORT		= ttyACM1

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
include $(DMBS_PATH)/cppcheck.mk
include $(DMBS_PATH)/doxygen.mk
include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
include $(DMBS_PATH)/hid.mk
include $(DMBS_PATH)/avrdude.mk
include $(DMBS_PATH)/atprogram.mk

.PHONY: run
run:
	avrdude -F -V -c avr109 -p m32u4 -P /dev/$(DEVICE_PORT) -b 115200 -U flash:w:./$(TARGET).hex

.PHONY: term
term:
	while true; do cat /dev/$(DEVICE_PORT) 2>/dev/null; done
