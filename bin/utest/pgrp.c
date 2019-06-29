#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utest.h"

int test_setpgid(void) {
  /* Init process can create and enter new process group. */
  assert(!setpgid(1, 0));

  pid_t pid = fork();
  if (pid == 0) {
    /* Process inherits group of its parent. */
    assert(getpgid(0) == 1);
    /* Process can make its own group. */
    assert(!setpgid(0, 0));
    /* New group has ID equal to the ID of the moved process. */
    assert(getpgid(0) == getpid());

    exit(0);
  }
  wait(NULL);

  /* It is forbidden to move the process to non-existing group. */
  assert(setpgid(0, pid));
  /* It is forbidden to change the group of non-existing process. */
  assert(setpgid(pid, pid));

  return 0;
}

static int sig_delivered = 0;

static void sa_handler(int signo) {
  assert(signo == SIGUSR1);
  sig_delivered = 1;
}

static void kill_tests_setup(void) {
  sig_delivered = 0;
  setpgid(0, 1);

  sigaction_t newact;
  sigaction_t oldact;

  newact.sa_handler = sa_handler;
  newact.sa_restorer = sigreturn;
  sigaction(SIGUSR1, &newact, &oldact);
}

int test_kill(void) {
  kill_tests_setup();

  pid_t pid = fork();
  if (pid == 0) {
    kill(1, SIGUSR1);
    /* Signal is not delivered to all processes in the group. */
    assert(!sig_delivered);
    exit(0);
  }

  wait(NULL);
  /* Signal is delivered to appropriate process. */
  assert(sig_delivered);

  return 0;
}

/* In this tests there are five processes marked with: a, b, c, d, e.
 * Process groups are marked with brackets: (a -> b) --> (c -> d) --> (e)
 * Process e sends signal to the process group containing c and d.
 * Process b sends signal to own process group containing a and b. */
int test_killpg(void) {
  kill_tests_setup();

  pid_t pid_a = fork();
  if (pid_a == 0) {
    setpgid(0, 0);

    pid_t pid_b = fork();
    if (pid_b == 0) {

      pid_t pid_c = fork();
      if (pid_c == 0) {
        setpgid(0, 0);
        pid_t pgid_c = getpgid(0);

        pid_t pid_d = fork();
        if (pid_d == 0) {

          pid_t pid_e = fork();
          if (pid_e == 0) {
            setpgid(0, 0);

            assert(!killpg(pgid_c, SIGUSR1));
            /* Process e should not receive
             * signal from process e. */
            assert(!sig_delivered);
            exit(0); // process e
          }

          wait(NULL);
          /* Process d should receive signal from process e. */
          assert(sig_delivered);
          exit(0); // process d
        }

        wait(NULL);
        /* Process c should receive signal from process e. */
        assert(sig_delivered);
        exit(0); // process c
      }
      wait(NULL);
      /* Process b should not receive signal from process e. */
      assert(!sig_delivered);
      assert(!killpg(0, SIGUSR1));
      /* Process b should receive signal from process b. */
      assert(sig_delivered);
      exit(0); // process b
    }

    wait(NULL);
    /* Process a should receive signal from process b. */
    assert(sig_delivered);
    /* It is forbidden to send signal to non-existing group. */
    assert(killpg(pid_b, SIGUSR1));
    /* Invalid argument. */
    assert(killpg(1, SIGUSR1));
    /* Invalid argument (negative number). */
    assert(killpg(-1, SIGUSR1));
  }

  return 0;
}
