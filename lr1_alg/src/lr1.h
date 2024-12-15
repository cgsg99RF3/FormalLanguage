#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <cctype>
#include <stack>

class State {
 public:
  std::string left;
  std::string right;
  size_t dot_pos;
  std::string sym;
  std::string rule;

  State() {}
  State(const std::string& left, const std::string& right,
        size_t dot_pos, const std::string& sym)
      : left(left), right(right),
        dot_pos(dot_pos), sym(sym) {
    rule = left + "->" + right + std::to_string(dot_pos) + sym;
  }

  std::string Next() {
    std::string str;
    if (dot_pos < right.size()) {
      str = right[dot_pos];
    } else {
      str = "$";
    }
    return str;
  }

  std::string LeftAfterDot() {
    std::string ans;
    for (size_t i = dot_pos + 1; i < right.size(); ++i) {
      ans.push_back(right[i]);
    }
    if (ans.empty()) {
      ans = "$";
    }
    return ans;
  }

  bool IsComplete() {
    return Next() == "$";
  }
};

class Grammar {
 public:
  size_t num_term;
  size_t num_non_term;
  size_t num_rules;
  std::string start_term;
  std::unordered_set<std::string> term;
  std::unordered_set<std::string> non_term;
  std::unordered_map<std::string, std::vector<std::string>> rules;

  Grammar() = default;
};

class LR1_Parser {
 public:
  struct Elem {
    size_t ind;
    State state = State();
    Elem() = default;
  };

  Grammar grammar;
  std::unordered_map<std::string, std::unordered_set<std::string>> first;
  std::vector<std::unordered_map<std::string, std::vector<State>>> closure;
  std::vector<std::unordered_set<std::string>> in_closure;
  std::vector<std::unordered_map<std::string, size_t>> path;
  std::vector<std::unordered_map<std::string, std::pair<std::string, Elem>>> action;

  bool AddFirst(const std::string& str, const std::string& elem) {
    if (!first.contains(str)) {
      first[str].insert(elem);
      return true;
    }
    if (!first[str].contains(elem)) {
      first[str].insert(elem);
      return true;
    }
    return false;
  }

  void ComputeFirst() {
    first["$"].insert("$");
    for (auto& term : grammar.term) {
      first[term].insert(term);
    }

    bool changes = true;
    while (changes) {
      changes = false;
      for (auto& rule : grammar.rules) {
        for (auto& right : rule.second) {
          changes |= ComputeFirst_String(rule.first, right);
        }
      }
    }
  }

  bool ComputeFirst_String(const std::string& left, const std::string& right) {
    bool changes = false;
    std::string r_elem;
    r_elem = right[0];
    if (first.contains(r_elem)) {
      for (auto& str : first[r_elem]) {
        if (str != "$") {
          changes |= AddFirst(left, str);
        }
      }
    }
    size_t i = 0;
    for (i = 0; i < right.size() - 1; ++i) {
      r_elem = right[i];
      if (!first.contains(r_elem) || !first[r_elem].contains("$")) {
        break;
      }
      r_elem = right[i + 1];
      if (first.contains(r_elem)) {
        for (auto& str : first[r_elem]) {
          if (str != "$") {
            changes |= AddFirst(left, str);
          }
        }
      }
    }
    r_elem = right[i];
    if ((i == right.size() - 1) &&
        first.contains(r_elem) && first[r_elem].contains("$")) {
      changes |= AddFirst(left, "$");
    }
    return changes;
  }

  bool UpdateClosure(State& state, size_t ind) {
    if (in_closure[ind].contains(state.rule)) {
      return false;
    }
    in_closure[ind].insert(state.rule);
    closure[ind][state.Next()].push_back(state);
    return true;
  }

  void ComputeClosure(size_t ind) {
    bool changes = true;
    while (changes) {
      changes = false;
      for (auto& states : closure[ind]) {
        for (auto state : states.second) {
          if (grammar.non_term.contains(state.Next())) {
            std::string left = state.LeftAfterDot();
            left += state.sym;
            ComputeFirst_String(left, left);
            for (auto& elem : first[left]) {
              for (auto& rule : grammar.rules[state.Next()]) {
                State new_state(state.Next(), rule, 0, elem);
                changes |= UpdateClosure(new_state, ind);
              }
            }
          }
        }
      }
    }
  }

  bool Goto(size_t new_ind, size_t ind, const std::string& elem) {
    bool changes = false;
    for (auto state : closure[ind][elem]) {
      if (!state.IsComplete()) {
        State new_state(state.left, state.right, state.dot_pos + 1, state.sym);
        changes |= UpdateClosure(new_state, new_ind);
      }
    }
    if (changes) {
      ComputeClosure(new_ind);
    }
    return changes;
  }

  void Construct() {
    grammar.term.insert("$");
    ComputeFirst();
    size_t ind = 0;
    State start("!", grammar.start_term, 0, "$");
    closure.resize(1);
    in_closure.resize(1);
    path.resize(1);
    UpdateClosure(start, ind);
    ComputeClosure(ind);
    bool changes = true;
    size_t prev_sz = 0;
    while (changes) {
      changes = false;
      size_t sz = closure.size();
      for (size_t i = prev_sz; i < sz; ++i) {
        for (auto& elem : closure[i]) {
          std::string str = elem.first;
          if (str == "$") {
            continue;
          }
          if (!path[i].contains(str)) {
            ++ind;
            path[i][str] = ind;
            closure.resize(ind + 1);
            path.resize(ind + 1);
            in_closure.resize(ind + 1);
            Goto(ind, i, str);
            bool flag = true;
            for (size_t j = 0; j < ind; ++j) {
              flag = true;
              for (auto& item : closure[j]) {
                if (!flag) {
                  break;
                }
                for (auto& cur : item.second) {
                  if (!in_closure[ind].contains(cur.rule)) {
                    flag = false;
                    break;
                  }
                }
              }
              if (flag) {
                path[i][str] = j;
                --ind;
                closure.pop_back();
                path.pop_back();
                in_closure.pop_back();
              }
            }
            changes |= !flag;
          } else {
            changes |= Goto(ind, i, str);
          }
        }
      }
      prev_sz = sz;
    }
  }

  void FillTables() {
    action.resize(closure.size());
    for (size_t i = 0; i < closure.size(); ++i) {
      for (auto& item : closure[i]) {
        for (auto& state : item.second) {
          if (grammar.term.contains(item.first) && path[i].contains(state.Next())) {
            Elem elem;
            elem.ind = path[i][state.Next()];
            action[i][state.Next()] = {"shift", elem};
          } else if (state.left == "!" &&
              state.right == grammar.start_term &&
              state.dot_pos == 1 && state.sym == "$") {
            Elem elem;
            action[i]["$"] = {"accept", elem};
          } else if (state.IsComplete()) {
            Elem elem;
            elem.state = state;
            action[i][state.sym] = {"reduce", elem};
          }
        }
      }
    }
  }

  void ConstructParser() {
    Construct();
    FillTables();
  }

  bool Predict(const std::string& data) {
    std::string str = data + "$";
    std::stack<std::pair<int, std::string>> lrstack;
    size_t pos = 0;
    lrstack.push({0 ,""});
    while (pos <= str.size()) {
      size_t ind = lrstack.top().first;
      std::string cur;
      cur = str[pos];
      if (!action[ind].contains(cur)) {
        std::cout << "No\n";
        return false;
      }
      if (action[ind][cur].first == "shift") {
        lrstack.push({-1, cur});
        ++pos;
        lrstack.push({action[ind][cur].second.ind, ""});
      } else if (action[ind][cur].first == "reduce") {
        for (size_t i = 0; i < 2 * action[ind][cur].second.state.right.size(); ++i) {
          lrstack.pop();
        }
        size_t new_ind = lrstack.top().first;
        if (!path[new_ind].contains(action[ind][cur].second.state.left)) {
          std::cout << "No\n";
          return false;
        }
        lrstack.push({-1, action[ind][cur].second.state.left});
        lrstack.push({path[new_ind][action[ind][cur].second.state.left], ""});
      } else if (action[ind][cur].first == "accept") {
        std::cout << "Yes\n";
        return true;
      }
    }
    std::cout << "No\n";
    return false;
  }
};

void fit_for_test(Grammar& grammar, const std::string& gram_str);
void fit(Grammar& grammar);
