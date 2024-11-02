AS			= /home/yahong/Binutils/install/bin/amo-linux-as
OBJCOPY		= /home/yahong/Binutils/install/bin/amo-linux-objcopy

CC			= /home/yahong/GCC/install/bin/amo-linux-gcc
CFLAGS		= -fno-builtin -fno-exceptions -fno-stack-protector \
			  -nostdlib -nodefaultlibs -ffreestanding

LD			= /home/yahong/Binutils/install/bin/amo-linux-ld

RM			= rm -rf

ARCH		= arch/IDT.s arch/interrupt.c arch/VGA.c arch/keyboard.c arch/timer.c arch/atomic.c
BOOT		= boot/boot.s
DRIVERS		= drivers/stdin.c drivers/stdout.c
FS			= fs/vfs.c
INIT		= init/main.c init/init_task.c
KERNEL		= kernel/schedule.c kernel/RR.c
LIBSOFT		= udivmodsi4.c udivmod.c divmod.c mulsi3.c
LIB			= lib/string.c lib/unistd.c lib/printk.c lib/assert.c
TOOLS		= tools/shell/shell.c tools/hexapawn.c

SRCS		= $(ARCH) $(BOOT) $(DRIVERS) $(FS) $(INIT) $(KERNEL) $(LIB) $(TOOLS) $(addprefix lib/soft/, $(LIBSOFT))
INCS		= -Iinclude

OBJS		= $(patsubst %.s,%.o,$(SRCS)) $(patsubst %.c,%.o,$(SRCS))
OBJS		:= $(filter %.o,$(OBJS))

LINKER_CONF	= boot/setup.ld

BIN			= lamune
COE			= lamune.coe

all: $(OBJS) $(BIN) $(COE)

%.o: %.s
	@$(AS) $< -o $@
	@echo "AS\t" $@

%.o: %.c
	@$(CC) $(CFLAGS) $(INCS) -S $< -o $(subst .o,.s,$@)
	@echo "CC\t" $(subst .o,.s,$@)
	@$(AS) $(subst .o,.s,$@) -o $@
	@echo "AS\t" $@
	@$(RM) $(subst .o,.s,$@)

$(BIN):
	@$(LD) -T $(LINKER_CONF) $(OBJS) -o $(BIN)
	@echo "LD\t" $(BIN)

$(COE):
	@$(OBJCOPY) -O binary $(BIN) _temp_coe
	@xxd -g4 -c32 _temp_coe | cut --characters=11-81 > $(COE)
	@$(RM) _temp_coe
	@echo "COE\t" $(COE)

clean:
	@$(RM) $(OBJS)
	@echo "CLEAN\t" $(OBJS)

fclean: clean
	@$(RM) $(BIN) $(COE)
	@echo "CLEAN\t" $(BIN) $(COE)

