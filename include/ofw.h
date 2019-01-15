#ifndef _FDT_H_
#define _FDT_H_

#include <common.h>

typedef uint16_t fdt16_t;
typedef uint32_t fdt32_t;
typedef uint64_t fdt64_t;

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

typedef uint32_t ihandle_t;
typedef uint32_t phandle_t;
typedef uint32_t pcell_t;

typedef struct fdt_header {
  uint32_t magic;
  uint32_t totalsize;
  uint32_t off_dt_struct;
  uint32_t off_dt_strings;
  uint32_t off_mem_rsvmap;
  uint32_t version;
  uint32_t last_comp_version;
  uint32_t boot_cpuid_phys;
  uint32_t size_dt_strings;
  uint32_t size_dt_struct;
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

int fdt_check_header(const void *fdt);
#define FDT_MAGIC	0xd00dfeed	/* 4: version, 4: total size */
#define FDT_TAGSIZE	sizeof(fdt32_t)

#define FDT_BEGIN_NODE	0x1		/* Start node: full name */
#define FDT_END_NODE	0x2		/* End node */
#define FDT_PROP	0x3		/* Property: name off,
                           101  					   size, content */
#define FDT_NOP		0x4		/* nop */
#define FDT_END		0x9

#define FDT_V1_SIZE	(7*sizeof(fdt32_t))
#define FDT_V2_SIZE	(FDT_V1_SIZE + sizeof(fdt32_t))
#define FDT_V3_SIZE	(FDT_V2_SIZE + sizeof(fdt32_t))
#define FDT_V16_SIZE	FDT_V3_SIZE
#define FDT_V17_SIZE	(FDT_V16_SIZE + sizeof(fdt32_t))


/* Device tree functions */
/*
phandle_t	OF_peer(phandle_t node);
phandle_t	OF_child(phandle_t node);
phandle_t	OF_parent(phandle_t node);
ssize_t		OF_getproplen(phandle_t node, const char *propname);
ssize_t		OF_getprop(phandle_t node, const char *propname, void *buf,
                                    size_t len);
ssize_t		OF_getencprop(phandle_t node, const char *prop, pcell_t *buf,
                                    size_t len);
int		OF_hasprop(phandle_t node, const char *propname);
ssize_t		OF_searchprop(phandle_t node, const char *propname, void *buf,
                                    size_t len);
ssize_t		OF_searchencprop(phandle_t node, const char *propname,
                                    pcell_t *buf, size_t len);
ssize_t		OF_getprop_alloc(phandle_t node, const char *propname,
                                    void **buf);
ssize_t		OF_getprop_alloc_multi(phandle_t node, const char *propname,
                                    int elsz, void **buf);
ssize_t		OF_getencprop_alloc(phandle_t node, const char *propname,
                                    void **buf);
ssize_t		OF_getencprop_alloc_multi(phandle_t node, const char *propname,
                                    int elsz, void **buf);
void		OF_prop_free(void *buf);
int		OF_nextprop(phandle_t node, const char *propname, char *buf,
                                    size_t len);
int		OF_setprop(phandle_t node, const char *name, const void *buf,
                                    size_t len);
ssize_t		OF_canon(const char *path, char *buf, size_t len);
phandle_t	OF_finddevice(const char *path);
ssize_t		OF_package_to_path(phandle_t node, char *buf, size_t len);
*/

#endif
