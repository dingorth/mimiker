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

static int nextprop_(const void *fdt, int offset) {
  uint32_t tag;
  int nextoffset;

  do {
    tag = fdt_next_tag(fdt, offset, &nextoffset);

    switch (tag) {
      case FDT_END:
        if (nextoffset >= 0)
          return -FDT_ERR_BADSTRUCTURE;
        else
          return nextoffset;

      case FDT_PROP:
        return offset;
    }
    offset = nextoffset;
  } while (tag == FDT_NOP);

  return -FDT_ERR_NOTFOUND;
}

int fdt_check_prop_offset_(const void *fdt, int offset) {
  if ((offset < 0) || (offset % FDT_TAGSIZE) ||
      (fdt_next_tag(fdt, offset, &offset) != FDT_PROP))
    return -FDT_ERR_BADOFFSET;

  return offset;
}

int fdt_first_property_offset(const void *fdt, int nodeoffset) {
  int offset;
  if ((offset = fdt_check_node_offset_(fdt, nodeoffset)) < 0)
    return offset;

  return nextprop_(fdt, offset);
}

int fdt_check_node_offset_(const void *fdt, int offset) {
  if ((offset < 0) || (offset % FDT_TAGSIZE) ||
      (fdt_next_tag(fdt, offset, &offset) != FDT_BEGIN_NODE))
    return -FDT_ERR_BADOFFSET;

  return offset;
}

int fdt_next_property_offset(const void *fdt, int offset) {
  if ((offset = fdt_check_prop_offset_(fdt, offset)) < 0)
    return offset;

  return nextprop_(fdt, offset);
}

static const struct fdt_property *
fdt_get_property_by_offset_(const void *fdt, int offset, int *lenp) {
  int err;
  const struct fdt_property *prop;

  if ((err = fdt_check_prop_offset_(fdt, offset)) < 0) {
    if (lenp)
      *lenp = err;
    return NULL;
  }

  prop = fdt_offset_ptr_(fdt, offset);

  if (lenp)
    *lenp = fdt32_to_cpu(prop->len);

  return prop;
}

const char *fdt_string(const void *fdt, int stroffset)
{
	return (const char *)fdt + fdt_off_dt_strings(fdt) + stroffset;
}

static int fdt_string_eq_(const void *fdt, int stroffset,
                          const char *s, int len)
{
	const char *p = fdt_string(fdt, stroffset);

	return ((int)strlen(p) == len) && (memcmp(p, s, len) == 0);
}

const struct fdt_property *fdt_get_property_by_offset(const void *fdt,
                                                      int offset, int *lenp) {
  /* Prior to version 16, properties may need realignment
   * and this API does not work. fdt_getprop_*() will, however. */

  if (fdt_version(fdt) < 0x10) {
    if (lenp)
      *lenp = -FDT_ERR_BADVERSION;
    return NULL;
  }

  return fdt_get_property_by_offset_(fdt, offset, lenp);
}
static const struct fdt_property *fdt_get_property_namelen_(const void *fdt,
						            int offset,
						            const char *name,
						            int namelen,
							    int *lenp,
							    int *poffset)
{
	for (offset = fdt_first_property_offset(fdt, offset);
	     (offset >= 0);
	     (offset = fdt_next_property_offset(fdt, offset))) {
		const struct fdt_property *prop;

		if (!(prop = fdt_get_property_by_offset_(fdt, offset, lenp))) {
			offset = -FDT_ERR_INTERNAL;
			break;
		}
		if (fdt_string_eq_(fdt, fdt32_to_cpu(prop->nameoff),
				   name, namelen)) {
			if (poffset)
				*poffset = offset;
			return prop;
		}
	}

	if (lenp)
		*lenp = offset;
	return NULL;
}


const struct fdt_property *fdt_get_property_namelen(const void *fdt,
						    int offset,
						    const char *name,
						    int namelen, int *lenp)
{
	/* Prior to version 16, properties may need realignment
	 * and this API does not work. fdt_getprop_*() will, however. */
	if (fdt_version(fdt) < 0x10) {
		if (lenp)
			*lenp = -FDT_ERR_BADVERSION;
		return NULL;
	}

	return fdt_get_property_namelen_(fdt, offset, name, namelen, lenp,
					 NULL);
}


const struct fdt_property *fdt_get_property(const void *fdt,
					    int nodeoffset,
					    const char *name, int *lenp)
{
	return fdt_get_property_namelen(fdt, nodeoffset, name,
					strlen(name), lenp);
}
