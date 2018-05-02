#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <timer.h>
#include <utility.h>

#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <grp.h>
#include <iostream>
#include <pwd.h>
#include <seccomp.h>
#include <signal.h>
#include <string>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

#define STACK_LIMIT

namespace Nemesis {

namespace Process {

enum ReturnState {
  OK = 0,
  TLE = 1,
  MLE = 2,
  ILL = 3,
  SEG = 4,
  FPE = 5,
  RE = 6,
  OE = 7,
  SYS = 8,
  FSZ = 9
};

enum ProcessState { STOPPED, WORKING };

void supervise(pid_t pid, int sig, unsigned long int limit);

class Process {
private:
  static const size_t SYSCALL_TABLE_SIZE = 14;
  int SYSCALL_TABLE[SYSCALL_TABLE_SIZE] = {
      SCMP_SYS(read),       SCMP_SYS(fstat),    SCMP_SYS(mmap),
      SCMP_SYS(exit),       SCMP_SYS(mprotect), SCMP_SYS(munmap),
      SCMP_SYS(arch_prctl), SCMP_SYS(brk),      SCMP_SYS(access),
      SCMP_SYS(exit_group), SCMP_SYS(uname),    SCMP_SYS(mmap2),
      SCMP_SYS(fstat64),    SCMP_SYS(lseek),
  };

  scmp_filter_ctx ctx;

  rusage *usage;
  pid_t pid;
  std::string path_input;
  std::string path_output;
  std::string path_exec;
  char *path_exec_pointer;
  char **args;

  ReturnState ret;
  ProcessState state;

  unsigned long int time_limit;
  unsigned long int memory_limit;

  Time::Timer timer;

  bool verbose;

  void set_seccomp();
  void child();
  void parent();

public:
  Process();
  ~Process();

  void set_path(const std::string &_path_exec, const std::string &_path_input,
                const std::string &_path_output);
  void set_time_limit(unsigned long int milliseconds);
  void set_memory_limit(unsigned long int kilobytes);
  void execute();
  ReturnState exit();

  ReturnState get_return_state() const;
  unsigned long int get_user_time() const;
  unsigned long int get_system_time() const;
  unsigned long int get_real_time() const;
  unsigned long int get_user_memory() const;
  ProcessState get_state() const;
  void set_verbose();
};

} // namespace Process

} // namespace Nemesis

#endif /* _PROCESS_H_ */
