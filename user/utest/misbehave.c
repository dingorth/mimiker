#include "utest.h"

#include <stddef.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

int test_misbehave() {
  const char str[] = "Hello world from a user program!\n";

  /* XXX: Currently kernel does not sigsegv offending programs, but in future it
     will, so this test will behave differently. */

  /* Successful syscall */
  assert(write(STDOUT_FILENO, str, sizeof(str) - 1) == sizeof(str) - 1);

  /* Pass bad pointer (kernel space) */
  assert(write(STDOUT_FILENO, (char *)0xDEADC0DE, 100) == -1);
  assert(errno == EFAULT);

  /* Pass bad pointer (user space) */
  assert(write(STDOUT_FILENO, (char *)0x1EE7C0DE, 100) == -1);
  assert(errno == EFAULT);

  return 0;
}

/* Should return -1 and set errno to ENOSYS == 38 (as in Linux) */
int test_exc_sigsys(void){
  int retval = 0;
  asm volatile("li $v0, 250;"
               "syscall;"
               "sw $v0, %0"
               : "=m"(retval)
               :
               : "memory", "v0");
               
  assert(retval == -1);
  assert(errno == ENOSYS);
  return 0;
}
