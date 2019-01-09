# vim: tabstop=8 shiftwidth=8 noexpandtab:
TOPDIR = $(realpath ../..)

include Makefile.libfdt

CFLAGS   = -ffreestanding
CPPFLAGS = -I$(TOPDIR)/include

SOURCES = $(LIBFDT_SRCS)

LIBFILE = libfdt.a

all: $(LIBFILE)

include $(TOPDIR)/build/flags.mk
include $(TOPDIR)/build/compile.mk
include $(TOPDIR)/build/common.mk

$(LIBFILE): $(OBJECTS)


# Installation targets
LIBFILE_INST = $(SYSROOT)/usr/lib/$(LIBFILE)

install-here: $(SYSROOT)/usr/lib/$(LIBFILE) $(EXTRAFILES_INST)

$(LIBFILE_INST): $(LIBFILE)
	@echo "[INSTALL] $(DIR)$(LIBFILE) -> /usr/lib/$(LIBFILE)"
	$(INSTALL) -m 644 $(LIBFILE) $(LIBFILE_INST)

clean-here:
	$(RM) *.o *.a
