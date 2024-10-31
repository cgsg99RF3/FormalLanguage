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

        std::vector<size_t> elem = rpn.top();
        rpn.pop();

        std::vector<size_t> dp(elem);
        dp[0] = 0;
        for (size_t j = 1; j < k + 1; ++j) {
          if (elem[j] == j) {
            for (size_t t = 0; t * j < k + 1; ++t)
              dp[j * t] = std::min(dp[j * t], elem[j] * t);
          }
        }

        rpn.push(dp);
      } else if (regex[i] == '+') {
        if (rpn.size() < 2) {
          throw std::invalid_argument("Invalid expression");
        }

        std::vector<size_t> left = rpn.top();
        rpn.pop();
        std::vector<size_t> right = rpn.top();
        rpn.pop();

        std::vector<size_t> dp(k + 1, INF);
        for (size_t j = 0; j < k + 1; ++j) {
          dp[j] = std::min(left[j], right[j]);
        }

        rpn.push(dp);
      } else if (regex[i] == '.') {
        if (rpn.size() < 2) {
          throw std::invalid_argument("Invalid expression");
        }

        std::vector<size_t> left = rpn.top();
        rpn.pop();
        std::vector<size_t> right = rpn.top();
        rpn.pop();

        std::vector<size_t> dp(k + 1, INF);
        for (size_t j = 0; j < k + 1; ++j) {
          if (right[j] != INF && left[0] != INF) {
            dp[j] = right[j] + left[0];
          }
          for (size_t t = 0; t <= j; ++t) {
            if (right[j - t] != INF &&
                left[t] != INF &&
                right[j - t] == j - t) {
              dp[j] = std::min(dp[j], left[t] + right[j - t]);
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
