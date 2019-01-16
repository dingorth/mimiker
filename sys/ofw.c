#include <ofw.h>
#include <stdc.h>
#include <klog.h> // do we want klog here?

/* Checks header magic number correctness */
int fdt_check_header(const void *fdt) {
  if (fdt_magic(fdt) != FDT_MAGIC)
    panic("Wrong FDT_MAGIC!");

  return 0;
}

/* prints basic info from fdt_header */
void fdt_print_header_info(const void *fdt) {
  klog("dtb_base_address: 0x%lx", fdt);
  klog("fdt_magic: 0x%lx", fdt_magic(fdt));
  klog("fdt_totalsize: %ld", fdt_totalsize(fdt));
  klog("fdt_version: %ld", fdt_version(fdt));
  klog("fdt_off_dt_struct: 0x%lx", fdt_off_dt_struct(fdt));
  klog("fdt_size_dt_struct: %ld", fdt_size_dt_struct(fdt));
  klog("fdt_off_dt_strings: 0x%lx", fdt_off_dt_strings(fdt));
  klog("fdt_size_dt_strings: %ld", fdt_size_dt_strings(fdt));
  klog("fdt_off_mem_rsvmap: 0x%lx", fdt_off_mem_rsvmap(fdt));
}

/* gets phandle from offset in dt_struct */
phandle_t fdt_offset_phandle(const void *fdt, int offset) {
  if (offset < 0)
    return (0);
  return ((phandle_t)offset + fdt_off_dt_struct(fdt));
}

/* gets offset in dt_struct from phandle */
int fdt_phandle_offset(const void *fdt, phandle_t p) {
  int pint = (int)p;
  int dtoff = fdt_off_dt_struct(fdt);

  if (pint < dtoff)
    return (-1);
  return (pint - dtoff);
}

/* gets pointer to fdt + offset + dt_struct */
const void *fdt_offset_ptr_(const void *fdt, int offset) {
  return (const char *)fdt + fdt_off_dt_struct(fdt) + offset;
}

/* gets node name from offset in dt_struct */
const char *fdt_get_name(const void *fdt, int nodeoffset, int *len) {
  const struct fdt_node_header *nh = fdt_offset_ptr_(fdt, nodeoffset);
  const char *nameptr;
  nameptr = nh->name;

  if (len)
    *len = strlen(nameptr);

  return nameptr;
}

const void *fdt_offset_ptr(const void *fdt, int offset, unsigned int len) {
// jakies checki
#if 0
  unsigned absoffset = offset + fdt_off_dt_struct(fdt);

  if ((absoffset < (unsigned)offset) || ((absoffset + len) < absoffset) ||
      (absoffset + len) > fdt_totalsize(fdt))
    return NULL;

  if (fdt_version(fdt) >= 0x11)
    if (((offset + len) < (unsigned)offset) ||
        ((offset + len) > fdt_size_dt_struct(fdt)))
      return NULL;
#endif

  return fdt_offset_ptr_(fdt, offset);
}

uint32_t fdt_next_tag(const void *fdt, int startoffset, int *nextoffset) {
  const fdt32_t *tagp, *lenp;
  uint32_t tag;
  int offset = startoffset;
  const char *p;

  *nextoffset = -FDT_ERR_TRUNCATED;
  tagp = fdt_offset_ptr(fdt, offset, FDT_TAGSIZE);
  if (!tagp)
    return FDT_END; /* premature end */
  tag = fdt32_to_cpu(*tagp);
  offset += FDT_TAGSIZE;

  *nextoffset = -FDT_ERR_BADSTRUCTURE;
  switch (tag) {
    case FDT_BEGIN_NODE:
      klog("got tag FDT_BEGIN_NODE");
      /* skip name */
      do {
        p = fdt_offset_ptr(fdt, offset++, 1);
      } while (p && (*p != '\0'));
      if (!p)
        return FDT_END; /* premature end */
      break;

    case FDT_PROP:
      lenp = fdt_offset_ptr(fdt, offset, sizeof(*lenp));
      if (!lenp)
        return FDT_END; /* premature end */
      /* skip-name offset, length and value */
      offset += sizeof(struct fdt_property) - FDT_TAGSIZE + fdt32_to_cpu(*lenp);
      if (fdt_version(fdt) < 0x10 && fdt32_to_cpu(*lenp) >= 8 &&
          ((offset - fdt32_to_cpu(*lenp)) % 8) != 0)
        offset += 4;
      break;

    case FDT_END:
    case FDT_END_NODE:
    case FDT_NOP:
      break;

    default:
      return FDT_END;
  }

  if (!fdt_offset_ptr(fdt, startoffset, offset - startoffset))
    return FDT_END; /* premature end */

  *nextoffset = FDT_TAGALIGN(offset);
  return tag;
}
