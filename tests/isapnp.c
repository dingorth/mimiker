#include <stdc.h>
#include <ktest.h>
#include <malloc.h>
//#include <taskqueue.h>
#include <dev/isapnpreg.h>

//static unsigned counter;

/* static void func(void *arg) { */
/*   int n = *(int *)arg; */
/*   /\* Access to counter is intentionally not synchronized. *\/ */
/*   counter += n; */
/* } */

static int test_isapnp(void) {
  /* taskqueue_t tq; */

  

  /* kprintf("TEEEEEEEEEEEEEST\n"); */

  
  /* int N[] = {1, 2, 3}; */

  /* task_t task0 = TASK_INIT(func, &N[0]); */
  /* task_t task1 = TASK_INIT(func, &N[1]); */
  /* task_t task2 = TASK_INIT(func, &N[2]); */

  /* counter = 0; */

  /* taskqueue_init(&tq); */
  /* taskqueue_add(&tq, &task0); */
  /* taskqueue_add(&tq, &task1); */
  /* taskqueue_add(&tq, &task2); */

  /* taskqueue_run(&tq); */

  /* assert(counter == 1 + 2 + 3); */

  /* taskqueue_destroy(&tq); */

  return KTEST_SUCCESS;
}

KTEST_ADD(isapnp, test_isapnp, 0);
