#include <common.h>
#include <rman.h>

static mtx_t rm_lock; /* global rman list lock */ 
static rman_list_t rman_list; /* global rman list */

int rman_init(rman_t *rman){
    mtx_init(&rm_lock, MTX_RECURSE);
    TAILQ_INIT(&rman_list);

    return 0;
}