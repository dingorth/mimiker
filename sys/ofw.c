#include <ofw.h>
#include <klog.h> // do we want klog here?

int fdt_check_header(const void *fdt) {
  if (fdt_magic(fdt) != FDT_MAGIC)
    panic("Wrong FDT_MAGIC!");

  return 0;
}

void fdt_print_header_info(const void *fdt){
  klog("fdt_magic: 0x%lx", fdt_magic(fdt));
  klog("fdt_totalsize: %ld", fdt_totalsize(fdt));
  klog("fdt_version: %ld", fdt_version(fdt));
  klog("fdt_off_dt_struct: 0x%lx", fdt_off_dt_struct(fdt));
  klog("fdt_size_dt_struct: %ld", fdt_size_dt_struct(fdt));
  klog("fdt_off_dt_strings: 0x%lx", fdt_off_dt_strings(fdt));
  klog("fdt_size_dt_strings: %ld", fdt_size_dt_strings(fdt));
  klog("fdt_off_mem_rsvmap: 0x%lx", fdt_off_mem_rsvmap(fdt));
}
