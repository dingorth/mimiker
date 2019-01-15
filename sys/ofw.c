#include <ofw.h>

int fdt_check_header(const void *fdt) {
  if (fdt_magic(fdt) != FDT_MAGIC)
    panic("Wrong FDT_MAGIC!");

  return 0;
}
