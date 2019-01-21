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


/* FDT values are BIG-ENDIAN, so we need some conversion. */
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

#undef CPU_TO_FDT64
#undef CPU_TO_FDT32
#undef CPU_TO_FDT16
#undef EXTRACT_BYTE

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

#define FDT_FIRST_SUPPORTED_VERSION	0x02
#define FDT_LAST_SUPPORTED_VERSION	0x11
#define FDT_SW_MAGIC		(~FDT_MAGIC)

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

/* maybe not needed */
void fdt_print_header_info(const void *fdt);


/**********************************************************************/
/* Low-level functions (you probably don't need these)                */
/**********************************************************************/

uint32_t fdt_next_tag(const void *fdt, int startoffset, int *nextoffset);

/**********************************************************************/
/* Traversal functions                                                */
/**********************************************************************/

/**
 * fdt_first_subnode() - get offset of first direct subnode
 *
 * @fdt:	FDT blob
 * @offset:	Offset of node to check
 * @return offset of first subnode, or -FDT_ERR_NOTFOUND if there is none
 */
int fdt_first_subnode(const void *fdt, int offset);

/**
 * fdt_next_subnode() - get offset of next direct subnode
 *
 * After first calling fdt_first_subnode(), call this function repeatedly to
 * get direct subnodes of a parent node.
 *
 * @fdt:	FDT blob
 * @offset:	Offset of previous subnode
 * @return offset of next subnode, or -FDT_ERR_NOTFOUND if there are no more
 * subnodes
 */
int fdt_next_subnode(const void *fdt, int offset);

/**
 * fdt_for_each_subnode - iterate over all subnodes of a parent
 *
 * @node:	child node (int, lvalue)
 * @fdt:	FDT blob (const void *)
 * @parent:	parent node (int)
 *
 * This is actually a wrapper around a for loop and would be used like so:
 *
 *	fdt_for_each_subnode(node, fdt, parent) {
 *		Use node
 *		...
 *	}
 *
 *	if ((node < 0) && (node != -FDT_ERR_NOT_FOUND)) {
 *		Error handling
 *	}
 *
 * Note that this is implemented as a macro and @node is used as
 * iterator in the loop. The parent variable be constant or even a
 * literal.
 *
 */
#define fdt_for_each_subnode(node, fdt, parent)		\
	for (node = fdt_first_subnode(fdt, parent);	\
	     node >= 0;					\
	     node = fdt_next_subnode(fdt, node))

/**********************************************************************/
/* Read-only functions                                                */
/**********************************************************************/

/**
 * fdt_string - retrieve a string from the strings block of a device tree
 * @fdt: pointer to the device tree blob
 * @stroffset: offset of the string within the strings block (native endian)
 *
 * fdt_string() retrieves a pointer to a single string from the
 * strings block of the device tree blob at fdt.
 *
 * returns:
 *     a pointer to the string, on success
 *     NULL, if stroffset is out of bounds
 */
const char *fdt_string(const void *fdt, int stroffset);

/**
 * fdt_get_name - retrieve the name of a given node
 * @fdt: pointer to the device tree blob
 * @nodeoffset: structure block offset of the starting node
 * @lenp: pointer to an integer variable (will be overwritten) or NULL
 *
 * fdt_get_name() retrieves the name (including unit address) of the
 * device tree node at structure block offset nodeoffset.  If lenp is
 * non-NULL, the length of this name is also returned, in the integer
 * pointed to by lenp.
 *
 * returns:
 *	pointer to the node's name, on success
 *		If lenp is non-NULL, *lenp contains the length of that name
 *			(>=0)
 *	NULL, on error
 *		if lenp is non-NULL *lenp contains an error code (<0):
 *		-FDT_ERR_BADOFFSET, nodeoffset did not point to FDT_BEGIN_NODE
 *			tag
 *		-FDT_ERR_BADMAGIC,
 *		-FDT_ERR_BADVERSION,
 *		-FDT_ERR_BADSTATE, standard meanings
 */
const char *fdt_get_name(const void *fdt, int nodeoffset, int *lenp);


/**
 * fdt_first_property_offset - find the offset of a node's first property
 * @fdt: pointer to the device tree blob
 * @nodeoffset: structure block offset of a node
 *
 * fdt_first_property_offset() finds the first property of the node at
 * the given structure block offset.
 *
 * returns:
 *	structure block offset of the property (>=0), on success
 *	-FDT_ERR_NOTFOUND, if the requested node has no properties
 *	-FDT_ERR_BADOFFSET, if nodeoffset did not point to an FDT_BEGIN_NODE tag
 *      -FDT_ERR_BADMAGIC,
 *	-FDT_ERR_BADVERSION,
 *	-FDT_ERR_BADSTATE,
 *	-FDT_ERR_BADSTRUCTURE,
 *	-FDT_ERR_TRUNCATED, standard meanings.
 */
int fdt_first_property_offset(const void *fdt, int nodeoffset);

/**
 * fdt_next_property_offset - step through a node's properties
 * @fdt: pointer to the device tree blob
 * @offset: structure block offset of a property
 *
 * fdt_next_property_offset() finds the property immediately after the
 * one at the given structure block offset.  This will be a property
 * of the same node as the given property.
 *
 * returns:
 *	structure block offset of the next property (>=0), on success
 *	-FDT_ERR_NOTFOUND, if the given property is the last in its node
 *	-FDT_ERR_BADOFFSET, if nodeoffset did not point to an FDT_PROP tag
 *      -FDT_ERR_BADMAGIC,
 *	-FDT_ERR_BADVERSION,
 *	-FDT_ERR_BADSTATE,
 *	-FDT_ERR_BADSTRUCTURE,
 *	-FDT_ERR_TRUNCATED, standard meanings.
 */
int fdt_next_property_offset(const void *fdt, int offset);

/**
 * fdt_for_each_property_offset - iterate over all properties of a node
 *
 * @property_offset:	property offset (int, lvalue)
 * @fdt:		FDT blob (const void *)
 * @node:		node offset (int)
 *
 * This is actually a wrapper around a for loop and would be used like so:
 *
 *	fdt_for_each_property_offset(property, fdt, node) {
 *		Use property
 *		...
 *	}
 *
 *	if ((property < 0) && (property != -FDT_ERR_NOT_FOUND)) {
 *		Error handling
 *	}
 *
 * Note that this is implemented as a macro and property is used as
 * iterator in the loop. The node variable can be constant or even a
 * literal.
 */
#define fdt_for_each_property_offset(property, fdt, node)	\
	for (property = fdt_first_property_offset(fdt, node);	\
	     property >= 0;					\
	     property = fdt_next_property_offset(fdt, property))



/**
* fdt_getprop_namelen - get property value based on substring
* @fdt: pointer to the device tree blob
* @nodeoffset: offset of the node whose property to find
* @name: name of the property to find
* @namelen: number of characters of name to consider
* @lenp: pointer to an integer variable (will be overwritten) or NULL
*
* Identical to fdt_getprop(), but only examine the first namelen
* characters of name for matching the property name.
*/
const void *fdt_getprop_namelen(const void *fdt, int nodeoffset,
const char *name, int namelen, int *lenp);


/**
 * fdt_getprop - retrieve the value of a given property
 * @fdt: pointer to the device tree blob
 * @nodeoffset: offset of the node whose property to find
 * @name: name of the property to find
 * @lenp: pointer to an integer variable (will be overwritten) or NULL
 *
 * fdt_getprop() retrieves a pointer to the value of the property
 * named 'name' of the node at offset nodeoffset (this will be a
 * pointer to within the device blob itself, not a copy of the value).
 * If lenp is non-NULL, the length of the property value is also
 * returned, in the integer pointed to by lenp.
 *
 * returns:
 *	pointer to the property's value
 *		if lenp is non-NULL, *lenp contains the length of the property
 *		value (>=0)
 *	NULL, on error
 *		if lenp is non-NULL, *lenp contains an error code (<0):
 *		-FDT_ERR_NOTFOUND, node does not have named property
 *		-FDT_ERR_BADOFFSET, nodeoffset did not point to FDT_BEGIN_NODE
 *			tag
 *		-FDT_ERR_BADMAGIC,
 *		-FDT_ERR_BADVERSION,
 *		-FDT_ERR_BADSTATE,
 *		-FDT_ERR_BADSTRUCTURE,
 *		-FDT_ERR_TRUNCATED, standard meanings
 */
const void *fdt_getprop(const void *fdt, int nodeoffset,
			const char *name, int *lenp);

#endif
