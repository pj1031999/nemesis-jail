#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <cstdio>
#include <cstdlib>

#define panic(...)                                                             \
  do {                                                                         \
    fprintf(stderr, "PANIC: " __VA_ARGS__);                                    \
    std::exit(EXIT_FAILURE);                                                   \
  } while (0);
#define debug(...) do { fprintf(stderr, "DEBUG: " __VA_ARGS__); } while (0);
#define error(...) do { fprintf(stderr, "ERROR: " __VA_ARGS__); } while (0);

#endif /* _UTILITY_H_ */
