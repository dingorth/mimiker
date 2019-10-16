/*
 * System call switch table.
 *
 * DO NOT EDIT: this file is automatically generated.
 * created from; Mimiker system call name/number "master" file.
 */

static int sys_syscall(proc_t *, syscall_args_t *, register_t *);
static int sys_exit(proc_t *, exit_args_t *, register_t *);
static int sys_fork(proc_t *, void *, register_t *);
static int sys_read(proc_t *, read_args_t *, register_t *);
static int sys_write(proc_t *, write_args_t *, register_t *);
static int sys_open(proc_t *, open_args_t *, register_t *);
static int sys_close(proc_t *, close_args_t *, register_t *);
static int sys_lseek(proc_t *, lseek_args_t *, register_t *);
static int sys_unlink(proc_t *, unlink_args_t *, register_t *);
static int sys_getpid(proc_t *, void *, register_t *);
static int sys_kill(proc_t *, kill_args_t *, register_t *);
static int sys_fstat(proc_t *, fstat_args_t *, register_t *);
static int sys_sbrk(proc_t *, sbrk_args_t *, register_t *);
static int sys_mmap(proc_t *, mmap_args_t *, register_t *);
static int sys_mount(proc_t *, mount_args_t *, register_t *);
static int sys_getdirentries(proc_t *, getdirentries_args_t *, register_t *);
static int sys_dup(proc_t *, dup_args_t *, register_t *);
static int sys_dup2(proc_t *, dup2_args_t *, register_t *);
static int sys_sigaction(proc_t *, sigaction_args_t *, register_t *);
static int sys_sigreturn(proc_t *, sigreturn_args_t *, register_t *);
static int sys_wait4(proc_t *, wait4_args_t *, register_t *);
static int sys_mkdir(proc_t *, mkdir_args_t *, register_t *);
static int sys_rmdir(proc_t *, rmdir_args_t *, register_t *);
static int sys_access(proc_t *, access_args_t *, register_t *);
static int sys_stat(proc_t *, stat_args_t *, register_t *);
static int sys_pipe2(proc_t *, pipe2_args_t *, register_t *);
static int sys_clock_gettime(proc_t *, clock_gettime_args_t *, register_t *);
static int sys_clock_nanosleep(proc_t *, clock_nanosleep_args_t *, register_t *);
static int sys_execve(proc_t *, execve_args_t *, register_t *);
static int sys_getppid(proc_t *, void *, register_t *);
static int sys_setpgid(proc_t *, setpgid_args_t *, register_t *);
static int sys_getpgid(proc_t *, getpgid_args_t *, register_t *);
static int sys_umask(proc_t *, umask_args_t *, register_t *);
static int sys_munmap(proc_t *, munmap_args_t *, register_t *);
static int sys_mprotect(proc_t *, mprotect_args_t *, register_t *);
static int sys_chdir(proc_t *, chdir_args_t *, register_t *);
static int sys_getcwd(proc_t *, getcwd_args_t *, register_t *);
static int sys_sigaltstack(proc_t *, sigaltstack_args_t *, register_t *);
static int sys_sigprocmask(proc_t *, sigprocmask_args_t *, register_t *);
static int sys_setcontext(proc_t *, setcontext_args_t *, register_t *);
static int sys_ioctl(proc_t *, ioctl_args_t *, register_t *);
static int sys_getuid(proc_t *, void *, register_t *);
static int sys_geteuid(proc_t *, void *, register_t *);
static int sys_getgid(proc_t *, void *, register_t *);
static int sys_getegid(proc_t *, void *, register_t *);
static int sys_issetugid(proc_t *, void *, register_t *);

struct sysent sysent[] = {
  [SYS_syscall] = { (syscall_t *)sys_syscall },
  [SYS_exit] = { (syscall_t *)sys_exit },
  [SYS_fork] = { (syscall_t *)sys_fork },
  [SYS_read] = { (syscall_t *)sys_read },
  [SYS_write] = { (syscall_t *)sys_write },
  [SYS_open] = { (syscall_t *)sys_open },
  [SYS_close] = { (syscall_t *)sys_close },
  [SYS_lseek] = { (syscall_t *)sys_lseek },
  [SYS_unlink] = { (syscall_t *)sys_unlink },
  [SYS_getpid] = { (syscall_t *)sys_getpid },
  [SYS_kill] = { (syscall_t *)sys_kill },
  [SYS_fstat] = { (syscall_t *)sys_fstat },
  [SYS_sbrk] = { (syscall_t *)sys_sbrk },
  [SYS_mmap] = { (syscall_t *)sys_mmap },
  [SYS_mount] = { (syscall_t *)sys_mount },
  [SYS_getdirentries] = { (syscall_t *)sys_getdirentries },
  [SYS_dup] = { (syscall_t *)sys_dup },
  [SYS_dup2] = { (syscall_t *)sys_dup2 },
  [SYS_sigaction] = { (syscall_t *)sys_sigaction },
  [SYS_sigreturn] = { (syscall_t *)sys_sigreturn },
  [SYS_wait4] = { (syscall_t *)sys_wait4 },
  [SYS_mkdir] = { (syscall_t *)sys_mkdir },
  [SYS_rmdir] = { (syscall_t *)sys_rmdir },
  [SYS_access] = { (syscall_t *)sys_access },
  [SYS_stat] = { (syscall_t *)sys_stat },
  [SYS_pipe2] = { (syscall_t *)sys_pipe2 },
  [SYS_clock_gettime] = { (syscall_t *)sys_clock_gettime },
  [SYS_clock_nanosleep] = { (syscall_t *)sys_clock_nanosleep },
  [SYS_execve] = { (syscall_t *)sys_execve },
  [SYS_getppid] = { (syscall_t *)sys_getppid },
  [SYS_setpgid] = { (syscall_t *)sys_setpgid },
  [SYS_getpgid] = { (syscall_t *)sys_getpgid },
  [SYS_umask] = { (syscall_t *)sys_umask },
  [SYS_munmap] = { (syscall_t *)sys_munmap },
  [SYS_mprotect] = { (syscall_t *)sys_mprotect },
  [SYS_chdir] = { (syscall_t *)sys_chdir },
  [SYS_getcwd] = { (syscall_t *)sys_getcwd },
  [SYS_sigaltstack] = { (syscall_t *)sys_sigaltstack },
  [SYS_sigprocmask] = { (syscall_t *)sys_sigprocmask },
  [SYS_setcontext] = { (syscall_t *)sys_setcontext },
  [SYS_ioctl] = { (syscall_t *)sys_ioctl },
  [SYS_getuid] = { (syscall_t *)sys_getuid },
  [SYS_geteuid] = { (syscall_t *)sys_geteuid },
  [SYS_getgid] = { (syscall_t *)sys_getgid },
  [SYS_getegid] = { (syscall_t *)sys_getegid },
  [SYS_issetugid] = { (syscall_t *)sys_issetugid },
};

