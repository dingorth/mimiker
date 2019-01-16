#ifndef _FDT_H_
#define _FDT_H_

#include <common.h>
#include <stdc.h>

typedef uint16_t fdt16_t;
typedef uint32_t fdt32_t;
typedef uint64_t fdt64_t;

typedef uint32_t ihandle_t;
typedef uint32_t phandle_t;
typedef uint32_t pcell_t;

typedef struct fdt_header {
  fdt32_t magic;             /* magic word FDT_MAGIC */
  fdt32_t totalsize;         /* total size of DT block */
  fdt32_t off_dt_struct;     /* offset to structure */
  fdt32_t off_dt_strings;    /* offset to strings */
  fdt32_t off_mem_rsvmap;    /* offset to memory reserve map */
  fdt32_t version;           /* format version */
  fdt32_t last_comp_version; /* last compatible version */

  /* version 2 fields below */
  fdt32_t boot_cpuid_phys; /* Which physical CPU id we're
                                                      booting on */
  /* version 3 fields below */
  fdt32_t size_dt_strings; /* size of the strings block */

  /* version 17 fields below */
  fdt32_t size_dt_struct; /* size of the structure block */
} fdt_header_t;

typedef struct fdt_reserve_entry {
  fdt64_t address;
  fdt64_t size;
} fdt_reserve_entry_t;

typedef struct fdt_node_header {
  fdt32_t tag;
  char name[0];
} fdt_node_header_t;

typedef struct fdt_property {
  fdt32_t tag;
  fdt32_t len;
  fdt32_t nameoff;
  char data[0];
} fdt_property_t;

#define EXTRACT_BYTE(x, n) ((unsigned long long)((uint8_t *)&x)[n])
#define CPU_TO_FDT16(x) ((EXTRACT_BYTE(x, 0) << 8) | EXTRACT_BYTE(x, 1))
#define CPU_TO_FDT32(x)                                                        \
  ((EXTRACT_BYTE(x, 0) << 24) | (EXTRACT_BYTE(x, 1) << 16) |                   \
   (EXTRACT_BYTE(x, 2) << 8) | EXTRACT_BYTE(x, 3))
#define CPU_TO_FDT64(x)                                                        \
  ((EXTRACT_BYTE(x, 0) << 56) | (EXTRACT_BYTE(x, 1) << 48) |                   \
   (EXTRACT_BYTE(x, 2) << 40) | (EXTRACT_BYTE(x, 3) << 32) |                   \
   (EXTRACT_BYTE(x, 4) << 24) | (EXTRACT_BYTE(x, 5) << 16) |                   \
   (EXTRACT_BYTE(x, 6) << 8) | EXTRACT_BYTE(x, 7))

#define FDT_TAGSiZE sizeof(fdt32_t)
#define FDT_ALIGN(x, a) (((x) + (a)-1) & ~((a)-1))
#define FDT_TAGALIGN(x) (FDT_ALIGN((x), FDT_TAGSIZE))

/* #define fdt_for_each_property_offset(property, fdt, node) \ */
/*   for (property = fdt_first_property_offset(fdt, node); property >= 0; \ */
/*        property = fdt_next_property_offset(fdt, property)) */

static inline uint16_t fdt16_to_cpu(fdt16_t x) {
  return (uint16_t)CPU_TO_FDT16(x);
}
static inline fdt16_t cpu_to_fdt16(uint16_t x) {
  return (fdt16_t)CPU_TO_FDT16(x);
}

static inline uint32_t fdt32_to_cpu(fdt32_t x) {
  return (uint32_t)CPU_TO_FDT32(x);
}

static inline fdt32_t cpu_to_fdt32(uint32_t x) {
  return (fdt32_t)CPU_TO_FDT32(x);
}

/* fdt_header_t getters */
#define fdt_get_header(fdt, field)                                             \
  (fdt32_to_cpu(((const struct fdt_header *)(fdt))->field))
#define fdt_magic(fdt) (fdt_get_header(fdt, magic))
#define fdt_totalsize(fdt) (fdt_get_header(fdt, totalsize))
#define fdt_off_dt_struct(fdt) (fdt_get_header(fdt, off_dt_struct))
#define fdt_off_dt_strings(fdt) (fdt_get_header(fdt, off_dt_strings))
#define fdt_off_mem_rsvmap(fdt) (fdt_get_header(fdt, off_mem_rsvmap))
#define fdt_version(fdt) (fdt_get_header(fdt, version))
#define fdt_last_comp_version(fdt) (fdt_get_header(fdt, last_comp_version))
#define fdt_boot_cpuid_phys(fdt) (fdt_get_header(fdt, boot_cpuid_phys))
#define fdt_size_dt_strings(fdt) (fdt_get_header(fdt, size_dt_strings))
#define fdt_size_dt_struct(fdt) (fdt_get_header(fdt, size_dt_struct))

#define FDT_MAGIC 0xd00dfeed /* 4: version, 4: total size */
#define FDT_TAGSIZE sizeof(fdt32_t)

#define FDT_BEGIN_NODE 0x1 /* Start node: full name */
#define FDT_END_NODE 0x2   /* End node */
#define FDT_PROP 0x3       /* Property: name off, size, content */
#define FDT_NOP 0x4        /* nop */
#define FDT_END 0x9

#define FDT_V1_SIZE (7 * sizeof(fdt32_t))
#define FDT_V2_SIZE (FDT_V1_SIZE + sizeof(fdt32_t))
#define FDT_V3_SIZE (FDT_V2_SIZE + sizeof(fdt32_t))
#define FDT_V16_SIZE FDT_V3_SIZE
#define FDT_V17_SIZE (FDT_V16_SIZE + sizeof(fdt32_t))

/* Error codes: informative error codes */
#define FDT_ERR_NOTFOUND 1
/* FDT_ERR_NOTFOUND: The requested node or property does not exist */
#define FDT_ERR_EXISTS 2
/* FDT_ERR_EXISTS: Attempted to create a node or property which
 * already exists */
#define FDT_ERR_NOSPACE 3
/* FDT_ERR_NOSPACE: Operation needed to expand the device
 * tree, but its buffer did not have sufficient space to
 * contain the expanded tree. Use fdt_open_into() to move the
 * device tree to a buffer with more space. */

/* Error codes: codes for bad parameters */
#define FDT_ERR_BADOFFSET 4
/* FDT_ERR_BADOFFSET: Function was passed a structure block
 * offset which is out-of-bounds, or which points to an
 * unsuitable part of the structure for the operation. */
#define FDT_ERR_BADPATH 5
/* FDT_ERR_BADPATH: Function was passed a badly formatted path
 * (e.g. missing a leading / for a function which requires an
 * absolute path) */
#define FDT_ERR_BADPHANDLE 6
/* FDT_ERR_BADPHANDLE: Function was passed an invalid phandle.
 * This can be caused either by an invalid phandle property
 * length, or the phandle value was either 0 or -1, which are
 * not permitted. */
#define FDT_ERR_BADSTATE 7
/* FDT_ERR_BADSTATE: Function was passed an incomplete device
 * tree created by the sequential-write functions, which is
 * not sufficiently complete for the requested operation. */

/* Error codes: codes for bad device tree blobs */
#define FDT_ERR_TRUNCATED 8
/* FDT_ERR_TRUNCATED: Structure block of the given device tree
 * ends without an FDT_END tag. */
#define FDT_ERR_BADMAGIC 9
/* FDT_ERR_BADMAGIC: Given "device tree" appears not to be a
 * device tree at all - it is missing the flattened device
 * tree magic number. */
#define FDT_ERR_BADVERSION 10
/* FDT_ERR_BADVERSION: Given device tree has a version which
 * can't be handled by the requested operation.  For
 * read-write functions, this may mean that fdt_open_into() is
 * required to convert the tree to the expected version. */
#define FDT_ERR_BADSTRUCTURE 11
/* FDT_ERR_BADSTRUCTURE: Given device tree has a corrupt
 * structure block or other serious error (e.g. misnested
 * nodes, or subnodes preceding properties). */
#define FDT_ERR_BADLAYOUT 12
/* FDT_ERR_BADLAYOUT: For read-write functions, the given
 * device tree has it's sub-blocks in an order that the
 * function can't handle (memory reserve map, then structure,
 * then strings).  Use fdt_open_into() to reorganize the tree
 * into a form suitable for the read-write operations. */

/* "Can't happen" error indicating a bug in libfdt */
#define FDT_ERR_INTERNAL 13
/* FDT_ERR_INTERNAL: libfdt has failed an internal assertion.
 * Should never be returned, if it is, it indicates a bug in
 * libfdt itself. */

/* Errors in device tree content */
#define FDT_ERR_BADNCELLS 14
/* FDT_ERR_BADNCELLS: Device tree has a #address-cells, #size-cells
 * or similar property with a bad format or value */

#define FDT_ERR_BADVALUE 15
/* FDT_ERR_BADVALUE: Device tree has a property with an unexpected
 * value. For example: a property expected to contain a string list
 * is not NUL-terminated within the length of its value. */

#define FDT_ERR_BADOVERLAY 16
/* FDT_ERR_BADOVERLAY: The device tree overlay, while
 * correctly structured, cannot be applied due to some
 * unexpected or missing value, property or node. */

#define FDT_ERR_NOPHANDLES 17
/* FDT_ERR_NOPHANDLES: The device tree doesn't have any
 * phandle available anymore without causing an overflow */

#define FDT_ERR_MAX 17

/* Fdt functions */
int fdt_check_header(const void *fdt);
void fdt_print_header_info(const void *fdt);
phandle_t fdt_offset_phandle(const void *fdt, int offset);
int fdt_phandle_offset(const void *fdt, phandle_t p);
const char *fdt_get_name(const void *fdt, int nodeoffset, int *len);
const void *fdt_offset_ptr_(const void *fdt, int offset);
uint32_t fdt_next_tag(const void *fdt, int startoffset, int *nextoffset);
const void *fdt_offset_ptr(const void *fdt, int offset, unsigned int len);
int fdt_first_property_offset(const void *fdt, int nodeoffset);
int fdt_check_node_offset_(const void *fdt, int offset);
int fdt_next_property_offset(const void *fdt, int offset);
int fdt_check_prop_offset_(const void *fdt, int offset);
const struct fdt_property *fdt_get_property_by_offset(const void *fdt,
                                                      int offset, int *lenp);

const char *fdt_string(const void *fdt, int stroffset);
const struct fdt_property *fdt_get_property_by_offset(const void *fdt,
                                                      int offset, int *lenp);
const struct fdt_property *fdt_get_property_namelen(const void *fdt, int offset,
                                                    const char *name,
                                                    int namelen, int *lenp);
const struct fdt_property *fdt_get_property(const void *fdt, int nodeoffset,
                                            const char *name, int *lenp);

#endif
