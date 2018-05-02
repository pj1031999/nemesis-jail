#include <vector>
#include <cstdlib>

int main() {
  std::vector<int> x;
  
  for (int iter = 1, cnt = 0; ; iter += iter, ++cnt) {
    x.resize(iter);
    if (cnt % 7 == 0) {
      for (auto &it : x)
        it = rand();
    }
  }
  return 0;
}
