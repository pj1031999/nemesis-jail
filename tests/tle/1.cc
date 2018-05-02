#include <unistd.h>
#include <cstdio>
#include <cstdlib>

int main() {
  for (;;) {
    int x = rand();
    if (x % 1031999 == 7)
      printf("%d\n", x);
  }
  return 0;
}
