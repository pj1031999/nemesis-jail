#include <process.h>
#include <getopt.h>

struct global_args_t {
  std::string exe;
  std::string in;
  std::string out;
  unsigned long int time_limit;
  unsigned long int memory_limit;
  int verbose_flag;
} global_args;

int main(int argc, char **argv) {
  int opt;
  while (true) {
    static struct option long_options[] = {
        {"verbose", no_argument, &global_args.verbose_flag, 1},
        {"exe", required_argument, NULL, 'e'},
        {"in", required_argument, NULL, 'i'},
        {"input", required_argument, NULL, 'i'},
        {"out", required_argument, NULL, 'o'},
        {"output", required_argument, NULL, 'o'},
        {"time_limit", required_argument, NULL, 't'},
        {"memory_limit", required_argument, NULL, 'm'},
        {NULL, NULL, NULL, NULL}};

    static const char *opt_string = "e:i:o:t:m";
    int option_index = 0;

    opt = getopt_long(argc, argv, opt_string, long_options, &option_index);

    if (opt == -1)
      break;

    switch (opt) {
      case 0:
        break;
      case 'e':
        global_args.exe = std::string(optarg);
        break;
      case 'i':
        global_args.in = std::string(optarg);
        break;
      case 'o':
        global_args.out = std::string(optarg);
        break;
      case 't':
        global_args.time_limit = std::atol(optarg);
        break;
      case 'm':
        global_args.memory_limit = std::atol(optarg);
        break;
      default:
        abort();
    }
  }

  Nemesis::Process::Process proc;

  if (global_args.verbose_flag)
    proc.set_verbose();

  proc.set_path(global_args.exe, global_args.in, global_args.out);
  proc.set_memory_limit(global_args.memory_limit);
  proc.set_time_limit(global_args.time_limit);
  proc.execute();
  auto state = proc.exit();

  //printf("state = %d; memory = %lu; time = %lu\n", state, proc.get_user_memory(), proc.get_real_time());

  return 0;
}
