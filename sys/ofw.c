#include <ofw.h>
#include <stdc.h>
#include <klog.h> // do we want klog here?

int fdt_check_header(const void *fdt) {
  if (fdt_magic(fdt) == FDT_MAGIC) {
    /* Complete tree */
    if (fdt_version(fdt) < FDT_FIRST_SUPPORTED_VERSION)
      return -FDT_ERR_BADVERSION;
    if (fdt_last_comp_version(fdt) > FDT_LAST_SUPPORTED_VERSION)
      return -FDT_ERR_BADVERSION;
  } else if (fdt_magic(fdt) == FDT_SW_MAGIC) {
    /* Unfinished sequential-write blob */
    if (fdt_size_dt_struct(fdt) == 0)
      return -FDT_ERR_BADSTATE;
  } else {
    return -FDT_ERR_BADMAGIC;
  }

  return 0;
}

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

void fdt_print_all_tags(const void *fdt){

  char *name;

  int nextoffset;
  int offset = 0;
  while (true) {
    uint32_t tag = fdt_next_tag(fdt, offset, &nextoffset);
    klog("tag = %ld at offset 0x%lx", tag, offset);
    klog("nextoffset = %lx", nextoffset);
    switch (tag) {
      case FDT_BEGIN_NODE:
        klog("got tag FDT_BEGIN_NODE");
        name = (char *)fdt_get_name(fdt, offset, NULL);
        klog("name = %s", name);
        break;
      case FDT_PROP:
        klog("got tag FDT_PROP");
        int len;
        fdt_property_t *prop =
          (fdt_property_t *)fdt_get_property_by_offset(fdt, offset, &len);
        klog("prop tag = %lx", prop->tag);
        klog("prop len = %lx", prop->len);
        klog("prop data as str = %s", prop->data);
        break;
      case FDT_END_NODE:
        klog("got tag FDT_END_NODE");
        break;
      case FDT_NOP:
        klog("got tag FDT_NOP");
        break;
      case FDT_END:
        klog("got tag FDT_END");
        break;
    }
    klog("-----");
    offset = nextoffset;
    if (tag == FDT_END)
      break;
  }


}

static void print_node_recursive(const void* fdt, int nodeoffset){
  int propertyoffset;
  int namelen;
  klog("BEGIN NODE");
  klog("node name: %s, len: %d",
       fdt_get_name(fdt, nodeoffset, &namelen),
       namelen);

  #define PATHBUF_LEN 100
  char path[PATHBUF_LEN];
  fdt_get_path(fdt, nodeoffset, path, PATHBUF_LEN);
  klog("full path: %s", path);

  klog("BEGIN NODE PROPS");

  const fdt_property_t* prop_ptr;
  int proplen;
  const char *prop_name;
  const void *prop_value_ptr;


  fdt_for_each_property_offset(propertyoffset, fdt, nodeoffset){
    prop_ptr = fdt_get_property_by_offset(fdt, propertyoffset, &proplen);
    prop_value_ptr = prop_ptr->data;
    prop_name = fdt_string(fdt, prop_ptr->nameoff);
    klog("%s = 0x%lx", prop_name, (int)prop_value_ptr);
  }

  int child_nodeoffset;
  fdt_for_each_subnode(child_nodeoffset, fdt, nodeoffset){
    print_node_recursive(fdt, child_nodeoffset);
  }

  klog("END NODE PROPS");
  klog("END NODE");
}

inline void print_whole_fdt(const void* fdt){
  print_node_recursive(fdt, 0);
}

static inline const void *fdt_offset_ptr_(const void *fdt, int offset) {
  return (const char *)fdt + fdt_off_dt_struct(fdt) + offset;
}

int fdt_check_prop_offset_(const void *fdt, int offset) {
  if ((offset < 0) || (offset % FDT_TAGSIZE) ||
      (fdt_next_tag(fdt, offset, &offset) != FDT_PROP))
    return -FDT_ERR_BADOFFSET;

  return offset;
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

const void *fdt_offset_ptr(const void *fdt, int offset, unsigned int len) {
  unsigned absoffset = offset + fdt_off_dt_struct(fdt);

  if (((int)absoffset < offset) || ((absoffset + len) < absoffset) ||
      (absoffset + len) > fdt_totalsize(fdt))
    return NULL;

  if (fdt_version(fdt) >= 0x11)
    if (((int)(offset + len) < offset) ||
        ((offset + len) > fdt_size_dt_struct(fdt)))
      return NULL;

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

int fdt_check_node_offset_(const void *fdt, int offset) {
  if ((offset < 0) || (offset % FDT_TAGSIZE) ||
      (fdt_next_tag(fdt, offset, &offset) != FDT_BEGIN_NODE))
    return -FDT_ERR_BADOFFSET;

  return offset;
}

int fdt_next_node(const void *fdt, int offset, int *depth) {
  int nextoffset = 0;
  uint32_t tag;

  if (offset >= 0)
    if ((nextoffset = fdt_check_node_offset_(fdt, offset)) < 0)
      return nextoffset;

  do {
    offset = nextoffset;
    tag = fdt_next_tag(fdt, offset, &nextoffset);

    switch (tag) {
      case FDT_PROP:
      case FDT_NOP:
        break;

      case FDT_BEGIN_NODE:
        if (depth)
          (*depth)++;
        break;

      case FDT_END_NODE:
        if (depth && ((--(*depth)) < 0))
          return nextoffset;
        break;

      case FDT_END:
        if ((nextoffset >= 0) || ((nextoffset == -FDT_ERR_TRUNCATED) && !depth))
          return -FDT_ERR_NOTFOUND;
        else
          return nextoffset;
    }
  } while (tag != FDT_BEGIN_NODE);

  return offset;
}

int fdt_first_subnode(const void *fdt, int offset) {
  int depth = 0;

  offset = fdt_next_node(fdt, offset, &depth);
  if (offset < 0 || depth != 1)
    return -FDT_ERR_NOTFOUND;

  return offset;
}

int fdt_next_subnode(const void *fdt, int offset) {
  int depth = 1;

  /*
   * With respect to the parent, the depth of the next subnode will be
   * the same as the last.
   */
  do {
    offset = fdt_next_node(fdt, offset, &depth);
    if (offset < 0 || depth < 1)
      return -FDT_ERR_NOTFOUND;
  } while (depth > 1);

  return offset;
}

static int fdt_string_eq_(const void *fdt, int stroffset, const char *s,
                          int len) {
  const char *p = fdt_string(fdt, stroffset);

  return ((int)strlen(p) == len) && (memcmp(p, s, len) == 0);
}

const char *fdt_string(const void *fdt, int stroffset) {
  return (const char *)fdt + fdt_off_dt_strings(fdt) + stroffset;
}

const char *fdt_get_name(const void *fdt, int nodeoffset, int *len) {
  const struct fdt_node_header *nh = fdt_offset_ptr_(fdt, nodeoffset);
  const char *nameptr;
  int err;

  if (((err = fdt_check_header(fdt)) != 0) ||
      ((err = fdt_check_node_offset_(fdt, nodeoffset)) < 0))
    goto fail;

  nameptr = nh->name;

  if (fdt_version(fdt) < 0x10) {
    /*
     * For old FDT versions, match the naming conventions of V16:
     * give only the leaf name (after all /). The actual tree
     * contents are loosely checked.
     */
    const char *leaf;
    leaf = strrchr(nameptr, '/');
    if (leaf == NULL) {
      err = -FDT_ERR_BADSTRUCTURE;
      goto fail;
    }
    nameptr = leaf + 1;
  }

  if (len)
    *len = strlen(nameptr);

  return nameptr;

fail:
  if (len)
    *len = err;
  return NULL;
}

int fdt_first_property_offset(const void *fdt, int nodeoffset) {
  int offset;

  if ((offset = fdt_check_node_offset_(fdt, nodeoffset)) < 0)
    return offset;

  return nextprop_(fdt, offset);
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

const struct fdt_property *fdt_get_property_by_offset(const void *fdt,
                                                      int offset,
                                                      int *lenp)
{
	/* Prior to version 16, properties may need realignment
	 * and this API does not work. fdt_getprop_*() will, however. */

	if (fdt_version(fdt) < 0x10) {
		if (lenp)
			*lenp = -FDT_ERR_BADVERSION;
		return NULL;
	}

	return fdt_get_property_by_offset_(fdt, offset, lenp);
}

static const struct fdt_property *
fdt_get_property_namelen_(const void *fdt, int offset, const char *name,
                          int namelen, int *lenp, int *poffset) {
  for (offset = fdt_first_property_offset(fdt, offset); (offset >= 0);
       (offset = fdt_next_property_offset(fdt, offset))) {
    const struct fdt_property *prop;

    if (!(prop = fdt_get_property_by_offset_(fdt, offset, lenp))) {
      offset = -FDT_ERR_INTERNAL;
      break;
    }
    if (fdt_string_eq_(fdt, fdt32_to_cpu(prop->nameoff), name, namelen)) {
      if (poffset)
        *poffset = offset;
      return prop;
    }
  }

  if (lenp)
    *lenp = offset;
  return NULL;
}

const void *fdt_getprop_namelen(const void *fdt, int nodeoffset,
                                const char *name, int namelen, int *lenp) {
  int poffset;
  const struct fdt_property *prop;

  prop =
    fdt_get_property_namelen_(fdt, nodeoffset, name, namelen, lenp, &poffset);
  if (!prop)
    return NULL;

  /* Handle realignment */
  if (fdt_version(fdt) < 0x10 && (poffset + sizeof(*prop)) % 8 &&
      fdt32_to_cpu(prop->len) >= 8)
    return prop->data + 4;
  return prop->data;
}

const void *fdt_getprop(const void *fdt, int nodeoffset, const char *name,
                        int *lenp) {
  return fdt_getprop_namelen(fdt, nodeoffset, name, strlen(name), lenp);
}

int fdt_get_path(const void *fdt, int nodeoffset, char *buf, int buflen) {
  int pdepth = 0, p = 0;
  int offset, depth, namelen;
  const char *name;

  FDT_CHECK_HEADER(fdt);

  if (buflen < 2)
    return -FDT_ERR_NOSPACE;

  for (offset = 0, depth = 0; (offset >= 0) && (offset <= nodeoffset);
       offset = fdt_next_node(fdt, offset, &depth)) {
    while (pdepth > depth) {
      do {
        p--;
      } while (buf[p - 1] != '/');
      pdepth--;
    }

    if (pdepth >= depth) {
      name = fdt_get_name(fdt, offset, &namelen);
      if (!name)
        return namelen;
      if ((p + namelen + 1) <= buflen) {
        memcpy(buf + p, name, namelen);
        p += namelen;
        buf[p++] = '/';
        pdepth++;
      }
    }

    if (offset == nodeoffset) {
      if (pdepth < (depth + 1))
        return -FDT_ERR_NOSPACE;

      if (p > 1) /* special case so that root path is "/", not "" */
        p--;
      buf[p] = '\0';
      return 0;
    }
  }

  if ((offset == -FDT_ERR_NOTFOUND) || (offset >= 0))
    return -FDT_ERR_BADOFFSET;
  else if (offset == -FDT_ERR_BADOFFSET)
    return -FDT_ERR_BADSTRUCTURE;

  return offset; /* error from fdt_next_node() */
}
