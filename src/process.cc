#include <process.h>

Nemesis::Process::Process::Process() {
  ctx = seccomp_init(SCMP_ACT_KILL);
  state = STOPPED;
  usage = nullptr;
  path_input = "/dev/null";
  path_output = "/dev/null";
  pid = 0;
  time_limit = 0;
  memory_limit = 0;
  verbose = false;
  path_exec_pointer = nullptr;
}

Nemesis::Process::Process::~Process() {
  if (usage != nullptr) {
    delete usage;
    usage = nullptr;
  }
  if (path_exec_pointer != nullptr) {
    delete[] path_exec_pointer;
    path_exec_pointer = nullptr;
  }
}

void Nemesis::Process::Process::set_seccomp() {
  if (verbose)
    debug("Nemesis::Process::Process::set_seccomp()\n");

  if (verbose)
    debug("path_exec_pointer = %s\n", path_exec_pointer);

  for (size_t i = 0; i < SYSCALL_TABLE_SIZE; ++i)
    if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SYSCALL_TABLE[i], 0) < 0)
      panic("seccomp rule add error %lu\n", i);

  if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 1,
                       SCMP_A0(SCMP_CMP_LE, 2)) < 0)
    panic("seccomp rule add error write\n");

  if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(execve), 1,
                       SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)path_exec_pointer)) < 0)
    panic("seccomp rule add error execve\n");

  if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 1,
                       SCMP_A0(SCMP_CMP_GT, 2)) < 0)
    panic("seccomp rule add error close\n");

  if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1,
                       SCMP_A1(SCMP_CMP_EQ, O_RDONLY)) < 0)
    panic("seccomp rule add error open\n");

  if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 1,
                       SCMP_A1(SCMP_CMP_EQ, O_RDONLY | O_CLOEXEC)) < 0)
    panic("seccomp rule add error open\n");

  if (seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(openat), 1,
                       SCMP_A2(SCMP_CMP_EQ, O_RDONLY | O_CLOEXEC)) < 0)
    panic("seccomp rule add error openat");

  if (seccomp_load(ctx) != 0)
    panic("seccomp load error\n");

  seccomp_release(ctx);

  if (verbose)
    debug("load and release seccomp");
}

void Nemesis::Process::Process::set_path(const std::string &_path_exec,
                                         const std::string &_path_input,
                                         const std::string &_path_output) {
  if (verbose)
    debug("Nemesis::Process::Process::set_path()\n");

  path_exec = _path_exec;
  path_input = _path_input;
  path_output = _path_output;

  path_exec_pointer = new char[path_exec.size() + 1];
  for (size_t iter = 0; iter < path_exec.size(); ++iter)
    path_exec_pointer[iter] = path_exec[iter];
  path_exec_pointer[path_exec.size()] = 0;
  
  if (verbose)
    debug("path_exec = %s path_input = %s path_output = %s\n",
          path_exec.c_str(), path_input.c_str(), path_output.c_str());
}

void Nemesis::Process::Process::set_time_limit(unsigned long int milliseconds) {
  if (verbose)
    debug("Nemesis::Process::Process::set_time_limit()\n");

  time_limit = milliseconds;
  if (verbose)
    debug("time_limit = %lu\n", time_limit);
}

void Nemesis::Process::Process::set_memory_limit(unsigned long int kilobytes) {
  if (verbose)
    debug("Nemesis::Process::Process::set_memory_limit()\n");

  memory_limit = kilobytes;
  if (verbose)
    debug("memory_limit = %lu\n", memory_limit);
}

unsigned long int Nemesis::Process::Process::get_user_time() const {
  if (verbose)
    debug("Nemesis::Process::Process::get_user_time()\n");

  if (usage == nullptr || state != STOPPED)
    panic("get_user_time error\n");

  if (verbose)
    debug("return %lu\n", usage->ru_utime.tv_sec * 1000000UL + usage->ru_utime.tv_usec); 

  return usage->ru_utime.tv_sec * 1000000UL + usage->ru_utime.tv_usec;
}

unsigned long int Nemesis::Process::Process::get_system_time() const {
  if (verbose)
    debug("Nemesis::Process::Process::get_system_time()\n");

  if (usage == nullptr || state != STOPPED)
    panic("get_system_time error");

  if (verbose)
    debug("return %lu\n", usage->ru_stime.tv_sec * 1000000UL + usage->ru_stime.tv_usec);

  return usage->ru_stime.tv_sec * 1000000UL + usage->ru_stime.tv_usec;
}

unsigned long int Nemesis::Process::Process::get_real_time() const {
  if (verbose)
    debug("Nemesis::Process::Process::get_real_time()\n");

  if (verbose)
    debug("return %lu\n", timer.get_time());

  return timer.get_time();
}

unsigned long int Nemesis::Process::Process::get_user_memory() const {
  if (verbose)
    debug("Nemesis::Process::Process:get_user_memory()\n");

  if (usage == nullptr || state != STOPPED)
    panic("get_user_memory error");

  if (verbose)
    debug("return %lu\n", usage->ru_maxrss);

  return usage->ru_maxrss;
}

void Nemesis::Process::Process::execute() {
  if (verbose)
    debug("Nemesis::Process::Process::execute()\n");

  if (state != STOPPED)
    panic("execute running process error");

  pid = fork();

  if (pid < 0)
    panic("fork error");

  if (pid == 0)
    child();

  state = WORKING;
  timer.start();
  std::thread time_thread(supervise, pid, SIGXCPU, time_limit);
  time_thread.detach();
  parent();
}

void Nemesis::Process::supervise(pid_t pid, int sig, unsigned long int limit) {
  usleep(limit * 1000UL + 5000UL);
  kill(pid, sig);
}

void Nemesis::Process::Process::child() {
  if (verbose)
    debug("Nemesis::Process::Process::child()\n");

  pid_t pid = getpid();
  if (setpriority(PRIO_PROCESS, pid, -20) != 0)
    error("setpriority error\n");

  FILE *in = fopen(path_input.c_str(), "r");
  FILE *out = fopen(path_output.c_str(), "w");
  FILE *err = fopen("/dev/null", "w");

  if (in == NULL || out == NULL || err == NULL)
    panic("open file to redirect error");

  if (dup2(fileno(in), fileno(stdin)) == -1)
    panic("dup2 stdin error");
  if (dup2(fileno(out), fileno(stdout)) == -1)
    panic("dup2 stdout error");
  if (dup2(fileno(err), fileno(stderr)) == -1)
    panic("dup2 stderr error");

  fclose(in);
  fclose(out);
  fclose(err);

  char *args[] = {path_exec_pointer, NULL};

  char *env[16];
  char envc[16][64];

  for (size_t iter = 0; iter < 16; ++iter) {
    for (size_t ptr = 0; ptr < 64; ++ptr)
      envc[iter][ptr] = 0;
    env[iter] = nullptr;
  }

  const char *nobody = "nobody";

  struct passwd *pw = getpwnam(nobody);

  if (pw == NULL)
    panic("getpwnam error");

  sprintf(envc[0], "TERM=xterm");
  sprintf(envc[1], "USER=%s", pw->pw_name);
  sprintf(envc[2], "HOME=%s", pw->pw_dir);
  sprintf(envc[3], "SHELL=%s", pw->pw_shell);
  sprintf(envc[4], "LOGNAME=%s", pw->pw_name);
  sprintf(envc[5], "PATH=");

  for (int i = 0; i <= 5; ++i)
    env[i] = envc[i];

  env[6] = NULL;


  if (initgroups(nobody, pw->pw_uid) != 0)
    panic("initgroups error");

  if (setgid(pw->pw_gid) != 0)
    panic("setgid error");

  if (setuid(pw->pw_uid) != 0)
    panic("setuid error");

  rlimit limit;

  getrlimit(RLIMIT_CPU, &limit);
  limit.rlim_cur = (time_limit) / 1000L + 1L;
  limit.rlim_max = (time_limit) / 1000L + 1L;
  if (setrlimit(RLIMIT_CPU, &limit) == -1)
    panic("setrlimit(RLIMIT_CPU) aka set time limit error");

  getrlimit(RLIMIT_AS, &limit);
  limit.rlim_cur = memory_limit * 1024UL;
  limit.rlim_max = memory_limit * 1024UL;
  if (setrlimit(RLIMIT_AS, &limit) == -1)
    panic("setrlimit(RLIMIT_AS) aka set memory limit error");

#ifdef STACK_LIMIT
  getrlimit(RLIMIT_STACK, &limit);
  limit.rlim_cur = memory_limit * 1024UL;
  limit.rlim_max = memory_limit * 1024UL;
  if (setrlimit(RLIMIT_STACK, &limit) == -1)
    panic("setrlimit(RLIMIT_STACK) aka set stack limit error");
#endif

  getrlimit(RLIMIT_FSIZE, &limit);
  limit.rlim_cur = 1024UL * 1024UL * 128UL;
  limit.rlim_max = 1024UL * 1024UL * 128UL;
  if (setrlimit(RLIMIT_FSIZE, &limit) == -1)
    panic("setrlimit(RLIMIT_FSIZE) aka set output limit error");

  getrlimit(RLIMIT_NPROC, &limit);
  limit.rlim_cur = 1;
  limit.rlim_max = 1;
  if (setrlimit(RLIMIT_NPROC, &limit) == -1)
    panic("setrlimit(RLIMIT_NPROC) error");

  set_seccomp();

  execve(path_exec_pointer, args, env);

  panic("execve error");
}

void Nemesis::Process::Process::parent() {
  if (verbose)
    debug("Nemesis::Process::Process::parent()\n");

  int child_status;

  wait(&child_status);

  if (verbose)
    debug("wait(&child_status) done\n");


  timer.stop();

  if (WIFEXITED(child_status) && WEXITSTATUS(child_status) == 0) {
    ret = OK;
    return;
  }

  if (WIFSIGNALED(child_status)) {
    switch (WTERMSIG(child_status)) {
    case SIGSYS:
      ret = ILL;
      break;
    case SIGXCPU:
      ret = TLE;
      break;
    case SIGABRT:
      ret = MLE;
      break;
    case SIGSTOP:
      ret = MLE;
      break;
    case SIGFPE:
      ret = FPE;
      break;
    case SIGSEGV:
      ret = SEG;
      break;
    case SIGXFSZ:
      ret = FSZ;
      break;
    default:
      ret = RE;
      break;
    }
  } else {
    ret = OE;
  }
}

Nemesis::Process::ReturnState Nemesis::Process::Process::exit() {
  if (verbose)
    debug("Nemesis::Process::Process::exit()\n");

  if (state != WORKING)
    panic("process is not running");

  if (usage != NULL)
    delete usage;

  state = STOPPED;
  usage = new rusage;
  getrusage(RUSAGE_CHILDREN, usage);

  if (get_real_time() > time_limit)
    ret = TLE;
  else {
    if (get_user_memory() > memory_limit)
      ret = MLE;
  }

  return ret;
}

Nemesis::Process::ReturnState
Nemesis::Process::Process::get_return_state() const {
  if (verbose)
    debug("Nemesis::Process::Process::get_return_state()\n");
  if (state != STOPPED)
    panic("process is not ended");
  return ret;
}

Nemesis::Process::ProcessState Nemesis::Process::Process::get_state() const {
  if (verbose)
    debug("Nemesis::Process::Process::get_state()\n");
  return state;
}

void Nemesis::Process::Process::set_verbose() {
  debug("Nemesis::Process::Process::set_verbose()\n");
  verbose = true;
}
