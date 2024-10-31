#include <iostream>

#include "prac1.h"

int main() {
  std::string regex;
  char x;
  size_t k;

  std::cin >> regex >> x >> k;

  try {
    size_t answer = GetLen(regex, x, k);
    if (answer == INF) {
      std::cout << "INF";
    }
    else {
      std::cout << answer;
    }
  } catch (const std::invalid_argument& error) {
    std::cout << error.what();
  }
  return 0;
}
