AS			= /home/yahong/Binutils/install/bin/amo-linux-as

CC			= /home/yahong/GCC/install/bin/amo-linux-gcc
CFLAGS		= -fno-builtin -fno-exceptions -fno-stack-protector \
			  -nostdlib -nodefaultlibs -ffreestanding 

LD			= /home/yahong/Binutils/install/bin/amo-linux-ld

RM			= rm -rf

BOOT		= boot/boot.s
INIT		= init/main.c
LIB			= lib/string.c

SRCS		= $(BOOT) $(INIT) $(LIB)
INCS		= -Iinclude

ASMS		= $(patsubst %.c,%.s,$(SRCS))

OBJS		= $(patsubst %.s,%.o,$(ASMS))

LINKER_CONF	= boot/setup.ld

BIN			= lamune

all: $(ASMS) $(OBJS) $(BIN)
	@echo $(OBJS)

%.o: %.s
	@$(AS) $< -o $@
	@echo "AS\t" $@

%.s: %.c
	@$(CC) $(CFLAGS) $(INCS) -S $< -o $@
	@echo "CC\t" $@

$(BIN):
	@$(LD) -T $(LINKER_CONF) $(OBJS) -o $(BIN)
	@echo "LD\t" $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(BIN)

