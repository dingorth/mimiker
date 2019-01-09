#ifndef _FDT_H_
#define _FDT_H_

#include <common.h>

/*
 *  * Prototypes for Open Firmware Interface Routines
 *   */

typedef uint32_t	ihandle_t;
typedef uint32_t	phandle_t;
typedef uint32_t	pcell_t;

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

void check_fdt_header(void);

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
