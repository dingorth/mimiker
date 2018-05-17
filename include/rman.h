#ifndef _SYS_RMAN_H_
#define _SYS_RMAN_H_

#include <common.h>
#include <queue.h>
#include <device.h>
#include <mutex.h>

// rman is purely kernel-side structure for now

// static MALLOC_DECLARE(M_RMAN);

typedef struct rman rman_t;
typedef TAILQ_HEAD(, rman) rman_list_t;
typedef uint32_t rman_res_t;

struct r_resource {

};

struct rman {
    TAILQ_HEAD(, resource) rm_rs;   /* list of resources */
    mtx_t *rm_rs_lock;  /* rs_lock */
    TAILQ_ENTRY(rman_list) rm_node; /* node on rman list */
    char *desc; /* description */
    device_t *dev; /* owner of this rman */
    rman_res_t  rm_start;   /* index of globally first entry */
    rman_res_t  rm_end; /* index of globally last entry */
};

int rman_init(rman_t *rman);
int rman_manage_region(rman_t *rman, rman_res_t start, rman_res_t end);

#endif /* _SYS_RMAN_H_ */
