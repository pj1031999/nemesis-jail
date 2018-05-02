#include <unistd.h>

int main() {
  char buf[10] = "aaaaa";
  write(0, buf, 5);
  write(1, buf, 5);
  write(2, buf, 5);
  write(3, buf, 5);
  return 0;
}
