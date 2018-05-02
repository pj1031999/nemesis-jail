#include <unistd.h>

int main() {
  while (fork()) {
    ;
  }
  return 0;
}
