#include <vector>
#include <cstdlib>

int main() {
  std::vector<int> x;
  x.resize(rand() % 1000);
  for (int i = 0; i < 100000; ++i)
    x[i] = rand();
  return 0;
}
