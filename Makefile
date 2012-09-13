# Configuracion del proyecto 

APP_NAME = Raven_EndDev
CONFIG_NAME = EndDevice_Raven
LIST_PATH = $(CONFIG_NAME)/List
EXE_PATH = $(CONFIG_NAME)/Exe
OBJ_PATH = $(CONFIG_NAME)/Obj

# Configuracion avrdude 

DEVICE_NO = m1284p
PROGRAMMER = dragon_jtag
PORT = usb

# Ruta a BitCloud 
BITCLOUD_PATH = /home/erich/avr/BitCloud_Raven

include $(BITCLOUD_PATH)/lib/Makerules_Atmega1284_Gcc

DEFINES = \
  -DBOARD_RAVEN \
  -DATMEGA1284 \
  -DAT86RF230B \
  -DHAL_4MHz \
  -DNO_SECURITY_MODE \
  -DSTACK_TYPE_ENDDEVICE 

INCLUDES = \
-I./ \
-I./include \
-I$(BITCLOUD_PATH)/lib \
-I$(BITCLOUD_PATH)/Components/BSP/RAVEN/include \
-I$(BITCLOUD_PATH)/Components/HAL/include \
-I$(BITCLOUD_PATH)/Components/BSP \
-I$(BITCLOUD_PATH)/Components/BSP/include \
-I$(BITCLOUD_PATH)/Components/NWK/include \
-I$(BITCLOUD_PATH)/Components/NWK/include/private \
-I$(BITCLOUD_PATH)/Components/ZDO/include \
-I$(BITCLOUD_PATH)/Components/ZDO/include/private \
-I$(BITCLOUD_PATH)/Components/APS/include \
-I$(BITCLOUD_PATH)/Components/APS/include/private \
-I$(BITCLOUD_PATH)/Components/SystemEnvironment/include \
-I$(BITCLOUD_PATH)/Components/ConfigServer/include \
-I$(BITCLOUD_PATH)/Components/ConfigServer/include/private \
-I$(BITCLOUD_PATH)/Components/PersistDataServer/include \
-I$(BITCLOUD_PATH)/Components/PersistDataServer/include/private \
-I$(BITCLOUD_PATH)/Components/Security/TrustCentre/include \
-I$(BITCLOUD_PATH)/Components/Security/ServiceProvider/include \
-I$(BITCLOUD_PATH)/Components/HAL/avr/atmega1284/common/include \
-I$(BITCLOUD_PATH)/Components/HAL/avr/atmega1284/raven/include \
-I$(BITCLOUD_PATH)/Components/MAC_PHY/include \
-I$(BITCLOUD_PATH)/Components/MAC_PHY/MAC_ENV/include \
-I$(BITCLOUD_PATH)/Components/MAC_PHY/MAC_HWI/include \
-I$(BITCLOUD_PATH)/Components/MAC_PHY/MAC_HWD_PHY/include \
-I$(BITCLOUD_PATH)/Components/MAC_PHY/MAC_HWD_PHY/RF230/PHY/include \
-I$(BITCLOUD_PATH)/Components/MAC_PHY/MAC_HWD_PHY/RF230/MAC_HWD/include 


LIBS = \
$(BITCLOUD_PATH)/lib/libHAL_Raven_Atmega1284_4Mhz_Gcc.a \
$(BITCLOUD_PATH)/lib/libBc_EndDevice_Atmega1284_Rf230B_Gcc.a \
$(BITCLOUD_PATH)/lib/WdtInitatmega1284_Gcc.o

SRCS = \
$(BITCLOUD_PATH)/Components/PersistDataServer/src/pdsWriteData.c \
$(BITCLOUD_PATH)/Components/PersistDataServer/src/pdsDataServer.c \
$(BITCLOUD_PATH)/Components/PersistDataServer/src/pdsCrcService.c \
$(BITCLOUD_PATH)/Components/BSP/RAVEN/src/sipc.c \
$(BITCLOUD_PATH)/Components/BSP/RAVEN/src/joystik.c \
$(BITCLOUD_PATH)/Components/BSP/RAVEN/src/bspTaskManager.c \
$(BITCLOUD_PATH)/Components/BSP/RAVEN/src/lcd.c \
$(BITCLOUD_PATH)/Components/BSP/RAVEN/src/sensors.c \
$(BITCLOUD_PATH)/Components/ConfigServer/src/csPersistentMem.c \
$(BITCLOUD_PATH)/Components/ConfigServer/src/csMem.c \
$(BITCLOUD_PATH)/Components/ConfigServer/src/configServer.c 

PREINCLUDE = MakerulesBc_EndDevice_Atmega1284_Rf230B_Gcc.h

SRCS += $(wildcard src/*.c)

CSRCS = $(filter %.c, $(SRCS))
OBJS = $(addprefix $(OBJ_PATH)/, $(notdir %$(subst .c,.o,$(CSRCS))))

ASM_FILE_EXT = s

ifneq (, $(findstring .$(ASM_FILE_EXT), $(SRCS)))
  ASRCS = $(filter %.$(ASM_FILE_EXT), $(SRCS))
  OBJS += $(addprefix $(OBJ_PATH)/, $(notdir %$(subst .$(ASM_FILE_EXT),.o,$(ASRCS))))
endif

CFLAGS += $(DEFINES)
CFLAGS += $(INCLUDES)
CFLAGS += -include $(PREINCLUDE)
CFLAGS += -g

ASFLAGS = $(INCLUDES)
ASFLAGS += $(filter -mmcu%,$(CFLAGS))

LD = $(CC)
LINKER_FLAGS = -Wl,-Map=$(LIST_PATH)/$(APP_NAME).map -Wl,--gc-sections
LINKER_FLAGS += -Wl,--script=linkerScr/atmega1284p.ld
LINKER_FLAGS += $(filter -mmcu%,$(CFLAGS))

.PHONY: all directories clean size root_files

all: directories $(EXE_PATH)/$(APP_NAME).elf $(EXE_PATH)/$(APP_NAME).hex $(EXE_PATH)/$(APP_NAME).srec $(EXE_PATH)/$(APP_NAME).bin root_files size

$(OBJ_PATH)/%.o: $(SRCS)
	$(CC) $(CFLAGS) $(filter %$(subst .o,.c,$(notdir $@)), $(SRCS)) -o $@

$(EXE_PATH)/$(APP_NAME).elf: $(OBJS)
	$(LD) $(LINKER_FLAGS) $(OBJS) -Wl,-\( $(LIBS) -Wl,-\) -o $@

$(EXE_PATH)/$(APP_NAME).srec: $(EXE_PATH)/$(APP_NAME).elf
	$(OBJCOPY) -O srec --srec-len 128 $^ $@

$(EXE_PATH)/$(APP_NAME).hex: $(EXE_PATH)/$(APP_NAME).elf
	$(OBJCOPY) -O ihex -R .eeprom $^ $@

$(EXE_PATH)/$(APP_NAME).bin: $(EXE_PATH)/$(APP_NAME).elf
	$(OBJCOPY) -O binary --strip-debug --strip-unneeded -R .eeprom $^ $@

root_files:
	cp -f $(EXE_PATH)/*.* ./

clean:
	rm -rf $(CONFIG_NAME) $(APP_NAME).elf $(APP_NAME).hex $(APP_NAME).srec $(APP_NAME).bin

install:
	avrdude -p $(DEVICE_NO) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$(APP_NAME).hex

prog_all: #Compila y programa
	make
	avrdude -p $(DEVICE_NO) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$(APP_NAME).hex

directories:
	@mkdir -p $(LIST_PATH)
	@mkdir -p $(EXE_PATH)
	@mkdir -p $(OBJ_PATH)

size: $(EXE_PATH)/$(APP_NAME).elf
	@echo
	@$(SIZE) -td $(EXE_PATH)/$(APP_NAME).elf
