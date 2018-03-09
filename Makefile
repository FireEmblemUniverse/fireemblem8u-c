#### Tools ####

CC1      := tools/agbcc/bin/agbcc
CC1_OLD  := tools/agbcc/bin/old_agbcc
CPP      := $(DEVKITARM)/bin/arm-none-eabi-cpp
AS       := $(DEVKITARM)/bin/arm-none-eabi-as
LD       := $(DEVKITARM)/bin/arm-none-eabi-ld
OBJCOPY  := $(DEVKITARM)/bin/arm-none-eabi-objcopy
TEXTENCODE := tools/textencode/textencode

CC1FLAGS := -mthumb-interwork -Wimplicit -Wparentheses -Werror -O2 -fhex-asm
CPPFLAGS := -I tools/agbcc/include -iquote include -nostdinc -undef
ASFLAGS  := -mcpu=arm7tdmi -mthumb-interwork -I include


#### Files ####

ROM       := fireemblem8.gba
ELF       := $(ROM:.gba=.elf)
MAP       := $(ROM:.gba=.map)
LDSCRIPT  := ldscript.txt
SYM_FILES := sym_iwram.txt sym_ewram.txt
CFILES_GENERATED := src/msg_data.c
CFILES    := $(wildcard src/*.c) $(CFILES_GENERATED)
SFILES    := $(wildcard asm/*.s) $(wildcard asm/libc/*.s) $(wildcard data/*.s)
OFILES    := $(SFILES:.s=.o) $(CFILES:.c=.o)

# Use the older compiler to build the m4a library
src/agb_sram.o: CC1FLAGS := -mthumb-interwork -Wimplicit -Wparentheses -Werror -O1
src/m4a_2.o: CC1 := tools/agbcc/bin/old_agbcc
src/m4a_4.o: CC1 := tools/agbcc/bin/old_agbcc


#### Main Targets ####

compare: $(ROM)
	sha1sum -c checksum.sha1

clean:
	$(RM) $(ROM) $(ELF) $(MAP) $(OFILES) src/*.s $(CFILES_GENERATED)


#### Recipes ####

$(ELF): $(OFILES) $(LDSCRIPT) $(SYM_FILES)
	$(LD) -T $(LDSCRIPT) -Map $(MAP) $(OFILES) tools/agbcc/lib/libgcc.a tools/agbcc/lib/libc.a -o $@

%.gba: %.elf
	$(OBJCOPY) -O binary --pad-to 0x9000000 $< $@

%.o: %.c
	$(CPP) $(CPPFLAGS) $< | $(CC1) $(CC1FLAGS) -o $*.s
	echo '.ALIGN 2, 0' >> $*.s
	$(AS) $(ASFLAGS) $*.s -o $*.o

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Generate msg_data.c
src/msg_data.c include/msg_data.h: msg_list.txt
	$(TEXTENCODE) $< $@ include/msg_data.h
