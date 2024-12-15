#include "src/earley.h"

int main() {
  Earley alg;
  fit(alg.grammar);
  size_t num;
  std::cin >> num;
  for (size_t i = 0; i < num; ++i) {
    std::string str;
    std::cin >> str;
    alg.Predict(str);
  }
}