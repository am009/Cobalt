LINKER= arch/$(ARCH)/linker.ld
CFLAG= -I./include -ffreestanding -Wall -Wextra
LDFLAG= -nostdlib -T $(LINKER) -o $(KERNEL) $(OBJS) -lgcc