#include "earley.h"

size_t GetElem(std::string& str, const std::string& data, size_t ind) {
  str.clear();
  bool flag = true;
  for (size_t i = ind; flag ||
                       (i < data.size() && data[i] != ' ' && data[i] != '\n'); ++i) {
    flag = false;
    if (data[i] != ' ' && data[i] != '\n') {
      str += data[i];
    }
  }
  return ind + str.size() + 1;
}

size_t GetLine(std::string& str, const std::string& data, size_t ind) {
  str.clear();
  for (size_t i = ind; i < data.size() && data[i] != '\n'; ++i) {
    str += data[i];
  }
  return ind + str.size() + 1;
}

void fit_for_test(Grammar& grammar, const std::string& gram_str) {
  std::string str;
  size_t ind = 0;
  ind = GetElem(str, gram_str, ind);
  grammar.num_non_term = stoi(str);

  ind = GetElem(str, gram_str, ind);
  grammar.num_term = stoi(str);

  ind = GetElem(str, gram_str, ind);
  grammar.num_rules = stoi(str);

  ind = GetLine(str, gram_str, ind);

  for (auto el : str) {
    if (el != ' ') {
      std::string cur;
      cur += el;
      if (!std::isupper(static_cast<unsigned char>(el))) {
        throw std::invalid_argument("Invalid Non Terminal symbol");
      }
      grammar.non_term.insert(cur);
    }
  }

  if (grammar.non_term.size() != grammar.num_non_term) {
    throw std::invalid_argument("Invalid grammar");
  }

  ind = GetLine(str, gram_str, ind);

  for (auto& el : str) {
    if (el != ' ') {
      std::string cur;
      cur += el;
      if (std::isupper(static_cast<unsigned char>(el))) {
        throw std::invalid_argument("Invalid Terminal symbol");
      }
      grammar.term.insert(cur);
    }
  }

  if (grammar.term.size() != grammar.num_term) {
    throw std::invalid_argument("Invalid grammar");
  }

  for (size_t i = 0; i < grammar.num_rules; ++i) {
    ind = GetLine(str, gram_str, ind);

    std::string left;
    std::string right;
    size_t ind_rule = 0;
    while (str[ind_rule] != '\0' &&
        (str[ind_rule] == ' ' ||  str[ind_rule] == '-' || str[ind_rule + 1] == '>')) {
      ++ind_rule;
    }

    left = str[ind_rule++];
    if (!grammar.non_term.contains(left)) {
      throw std::invalid_argument("Invalid rule");
    }

    bool arrow_found = false;
    while (str[ind_rule] != '\0' &&
        (str[ind_rule] == ' ' ||  str[ind_rule] == '-' || str[ind_rule] == '>')) {
      if (str[ind_rule] == '-' && str[ind_rule + 1] == '>') {
        arrow_found = true;
      }
      ++ind_rule;
    }

    if (!arrow_found) {
      throw std::invalid_argument("Invalid rule");
    }

    while (str[ind_rule] != '\0') {
      std::string cur;
      cur = str[ind_rule];
      if (!grammar.non_term.contains(cur) && !grammar.term.contains(cur)) {
        throw std::invalid_argument("Invalid rule");
      }
      right.push_back(str[ind_rule++]);
    }

    grammar.rules[left].push_back(right);
  }

  GetElem(str, gram_str, ind);
  grammar.start_term = str;
  if (!grammar.non_term.contains(grammar.start_term)) {
    throw std::invalid_argument("Invalid start terminal");
  }
}

void fit(Grammar& grammar) {
  std::cin >> grammar.num_non_term
           >> grammar.num_term
           >> grammar.num_rules;

  std::string str;
  std::getline(std::cin, str);
  std::getline(std::cin, str);
  for (auto el : str) {
    if (el != ' ') {
      std::string cur;
      cur += el;
      if (!std::isupper(static_cast<unsigned char>(el))) {
        throw std::invalid_argument("Invalid Non Terminal symbol");
      }
      grammar.non_term.insert(cur);
    }
  }

  if (grammar.non_term.size() != grammar.num_non_term) {
    throw std::invalid_argument("Invalid grammar");
  }

  std::getline(std::cin, str);
  for (auto& el : str) {
    if (el != ' ') {
      std::string cur;
      cur += el;
      if (std::isupper(static_cast<unsigned char>(el))) {
        throw std::invalid_argument("Invalid Terminal symbol");
      }
      grammar.term.insert(cur);
    }
  }

  if (grammar.term.size() != grammar.num_term) {
    throw std::invalid_argument("Invalid grammar");
  }

  for (size_t i = 0; i < grammar.num_rules; ++i) {
    std::getline(std::cin, str);

    std::string left;
    std::string right;
    size_t ind = 0;
    while (str[ind] != '\0' &&
        (str[ind] == ' ' ||  str[ind] == '-' || str[ind + 1] == '>')) {
      ++ind;
    }

    left = str[ind++];
    if (!grammar.non_term.contains(left)) {
      throw std::invalid_argument("Invalid rule");
    }

    bool arrow_found = false;
    while (str[ind] != '\0' &&
        (str[ind] == ' ' ||  str[ind] == '-' || str[ind] == '>')) {
      if (str[ind] == '-' && str[ind + 1] == '>') {
        arrow_found = true;
      }
      ++ind;
    }

    if (!arrow_found) {
      throw std::invalid_argument("Invalid rule");
    }

    while (str[ind] != '\0') {
      std::string cur;
      cur = str[ind];
      if (!grammar.non_term.contains(cur) && !grammar.term.contains(cur)) {
        throw std::invalid_argument("Invalid rule");
      }
      right.push_back(str[ind++]);
    }

    grammar.rules[left].push_back(right);
  }

  std::cin >> grammar.start_term;
  if (!grammar.non_term.contains(grammar.start_term)) {
    throw std::invalid_argument("Invalid start terminal");
  }
}