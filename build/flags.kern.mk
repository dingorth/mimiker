include $(TOPDIR)/build/flags.mk

CFLAGS   += -fno-builtin -nostdinc -nostdlib -ffreestanding
CPPFLAGS += -I$(TOPDIR)/include -D_KERNELSPACE 
LDFLAGS  += -nostdlib
