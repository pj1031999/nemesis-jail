#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <cstdio>
#include <cstdlib>

#define panic(...)                                                             \
  do {                                                                         \
    fprintf(stderr, "panic: " __VA_ARGS__);                                    \
    std::exit(EXIT_FAILURE);                                                        \
  } while (0);
#define debug(...) do { fprintf(stderr, "debug: " __VA_ARGS__); } while (0);
#define error(...) do { fprintf(stderr, "error: " __VA_ARGS__); } while (0);

#endif /* _UTILITY_H_ */
