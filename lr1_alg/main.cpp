#include "src/lr1.h"

int main() {
  LR1_Parser alg;
  fit(alg.grammar);
  alg.ConstructParser();
  size_t num;
  std::cin >> num;
  for (size_t i = 0; i < num; ++i) {
    std::string str;
    std::cin >> str;
    alg.Predict(str);
  }
}