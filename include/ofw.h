#ifndef _FDT_H_
#define _FDT_H_

#include <common.h>

typedef uint16_t fdt16_t;
typedef uint32_t fdt32_t;
typedef uint64_t fdt64_t;

typedef uint32_t ihandle_t;
typedef uint32_t phandle_t;
typedef uint32_t pcell_t;

typedef struct fdt_header {
  fdt32_t magic;			 /* magic word FDT_MAGIC */
  fdt32_t totalsize;		 /* total size of DT block */
  fdt32_t off_dt_struct;		 /* offset to structure */
  fdt32_t off_dt_strings;		 /* offset to strings */
  fdt32_t off_mem_rsvmap;		 /* offset to memory reserve map */
  fdt32_t version;		 /* format version */
  fdt32_t last_comp_version;	 /* last compatible version */

  /* version 2 fields below */
  fdt32_t boot_cpuid_phys;	 /* Which physical CPU id we're
                  					    booting on */
  /* version 3 fields below */
  fdt32_t size_dt_strings;	 /* size of the strings block */

  /* version 17 fields below */
  fdt32_t size_dt_struct;		 /* size of the structure block */
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

#define FDT_MAGIC	0xd00dfeed	/* 4: version, 4: total size */
#define FDT_TAGSIZE	sizeof(fdt32_t)

#define FDT_BEGIN_NODE	0x1		/* Start node: full name */
#define FDT_END_NODE	0x2		/* End node */
#define FDT_PROP	0x3		/* Property: name off, size, content */
#define FDT_NOP		0x4		/* nop */
#define FDT_END		0x9

int fdt_check_header(const void *fdt);
void fdt_print_header_info(const void *fdt);


#endif
