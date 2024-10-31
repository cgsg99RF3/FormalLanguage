#include <stack>
#include <stdexcept>
#include <set>
#include <vector>

#include "prac1.h"

size_t GetLen(const std::string& regex, char x, int k) {
  std::stack<std::vector<size_t>> rpn;

  std::set<char> symbols({'*', '+', '.'});
  std::set<char> alphabet({'a', 'b', 'c'});

  for (size_t i = 0; i < regex.size(); ++i) {
    if (symbols.contains(regex[i])) {
      if (regex[i] == '*') {
        if (rpn.empty()) {
          throw std::invalid_argument("Invalid notation");
        }

        std::vector<size_t> elem1 = rpn.top();
        rpn.pop();

        std::vector<size_t> dp(elem1);
        dp[0] = 0;
        for (size_t j = 1; j < k + 1; ++j) {
          if (elem1[j] == j) {
            for (size_t t = 0; t * j < k + 1; ++t)
              dp[j * t] = std::min(dp[j * t], elem1[j] * t);
          }
        }

        rpn.push(dp);
      } else if (regex[i] == '+') {
        if (rpn.size() < 2) {
          throw std::invalid_argument("Invalid expression");
        }

        std::vector<size_t> elem1 = rpn.top();
        rpn.pop();
        std::vector<size_t> elem2 = rpn.top();
        rpn.pop();

        std::vector<size_t> dp(k + 1, INF);
        for (size_t j = 0; j < k + 1; ++j) {
          dp[j] = std::min(elem1[j], elem2[j]);
        }

        rpn.push(dp);
      } else if (regex[i] == '.') {
        if (rpn.size() < 2) {
          throw std::invalid_argument("Invalid expression");
        }

        std::vector<size_t> elem1 = rpn.top();
        rpn.pop();
        std::vector<size_t> elem2 = rpn.top();
        rpn.pop();

        std::vector<size_t> dp(k + 1, INF);
        for (size_t j = 0; j < k + 1; ++j) {
          if (elem2[j] != INF && elem1[0] != INF) {
            dp[j] = elem2[j] + elem1[0];
          }
          for (size_t t = 0; t <= j; ++t) {
            if (elem2[j - t] != INF &&
                elem1[t] != INF &&
                elem2[j - t] == j - t) {
              dp[j] = std::min(dp[j], elem1[t] + elem2[j - t]);
            }
          }
        }

        rpn.push(dp);
      }
    } else if (alphabet.contains(regex[i])) {
      std::vector<size_t> dp(k + 1, INF);
      dp[0] = 1;

      if (regex[i] == x) {
        dp[1] = 1;
      }

      rpn.push(dp);
    } else {
      throw std::invalid_argument("Regex contains invalid symbol");
    }
  }

  if (rpn.size() != 1) {
    throw std::invalid_argument("Invalid expression");
  }

  std::vector<size_t> ans = rpn.top();
  return ans[k];
}
