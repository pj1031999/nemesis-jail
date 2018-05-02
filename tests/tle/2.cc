#include <stdio.h>
#include <stdlib.h>

int main() {
  int x = 0;
  for (; ; ++x) {
    x %= 1 + (rand() % 1000000);
    if (x == 0)
      printf("%d\n", x);
  }
}
