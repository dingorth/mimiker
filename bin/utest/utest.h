#ifndef __UTEST_H__
#define __UTEST_H__

typedef int (*proc_func_t)(void *);

int utest_spawn(proc_func_t func, void *arg);
void utest_child_exited(int exitcode);

/* List of available tests. */
int test_mmap(void);
int test_munmap_sigsegv(void);
int test_sbrk(void);
int test_sbrk_sigsegv(void);
int test_misbehave(void);

int test_fd_read(void);
int test_fd_devnull(void);
int test_fd_multidesc(void);
int test_fd_readwrite(void);
int test_fd_copy(void);
int test_fd_bad_desc(void);
int test_fd_open_path(void);
int test_fd_dup(void);
int test_fd_pipe(void);
int test_fd_all(void);

int test_signal_basic(void);
int test_signal_send(void);
int test_signal_abort(void);
int test_signal_segfault(void);

int test_fork_wait(void);
int test_fork_signal(void);
int test_fork_sigchld_ignored(void);

int test_lseek_basic(void);
int test_lseek_errors(void);

int test_access_basic(void);

int test_stat(void);
int test_fstat(void);

int test_fpu_fcsr(void);
int test_fpu_gpr_preservation(void);
int test_fpu_cpy_ctx_on_fork(void);
int test_fpu_ctx_signals(void);

int test_exc_cop_unusable(void);
int test_exc_reserved_instruction(void);
int test_exc_integer_overflow(void);
int test_exc_sigsys(void);
int test_exc_unaligned_access(void);
int test_syscall_in_bds(void);

int test_setjmp(void);

int test_sigaction_with_setjmp(void);
int test_sigaction_handler_returns(void);

#endif /* __UTEST_H__ */
