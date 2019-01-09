#include <ofw.h>

extern fdt_header_t* __dtb_start;

void check_fdt_header(){
	assert(__dtb_start->magic == 0xd00dfeedu);	
}
