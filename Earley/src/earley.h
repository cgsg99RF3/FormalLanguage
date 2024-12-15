#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <cctype>

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

class State {
 public:
  std::string left;
  std::string right;
  size_t dot_pos;
  size_t start_pos;
  std::string rule;

  State() = default;
  State(const std::string& left, const std::string& right,
        size_t dot_pos, size_t start_pos)
      : left(left), right(right),
        dot_pos(dot_pos), start_pos(start_pos) {
    rule = left + "->" + right + '$' + std::to_string(dot_pos);
  }

  std::string Next() {
    if (dot_pos < right.size()) {
      std::string str;
      str += right[dot_pos];
      return str;
    }
    return "$";
  }

  bool IsComplete() {
    return Next() == "$";
  }
};

class Chart {
 public:
  size_t word_len;
  size_t end_pos;
  std::vector<State> states;
  std::vector<State> complete_small_states;
  std::unordered_map<std::string, std::vector<bool>> possible_states;
  std::unordered_map<std::string, std::vector<State>> next_sign_states;

  Chart(size_t word_len) : word_len(word_len + 1) {}
  Chart() = default;

  bool Add(State& state) {
    if (!possible_states.contains(state.rule) ||
        !possible_states[state.rule][state.start_pos]) {
      possible_states[state.rule].resize(word_len, false);
      possible_states[state.rule][state.start_pos] = true;
      states.push_back(state);
      next_sign_states[state.Next()].push_back(state);
      if (state.IsComplete() && state.start_pos == end_pos) {
        complete_small_states.push_back(state);
      }
      return true;
    }
    return false;
  }

  bool Contains(State& state) {
    return possible_states.contains(state.rule) &&
           possible_states[state.rule][state.start_pos];
  }
};

class Earley {
 public:
  std::vector<Chart> charts;
  Grammar grammar;
  std::string word;
  bool changes = true;

  bool Predict(const std::string& cur_word) {
    changes = true;
    word = cur_word;
    charts.clear();
    charts.resize(word.size() + 1, Chart(word.size()));

    for (size_t i = 0; i < charts.size(); ++i) {
      charts[i].end_pos = i;
    }

    State start("!", grammar.start_term, 0, 0);
    charts[0].Add(start);
    size_t predict_start_id = 0;
    size_t compl_start_id = 0;
    while (changes) {
      predict_start_id = Prediction(0, predict_start_id);
      compl_start_id = Complete(0, compl_start_id);
    }
    for (size_t i = 1; i <= word.size(); ++i) {
      Scan(i - 1);
      changes = true;
      predict_start_id = 0;
      compl_start_id = 0;
      while (changes) {
        predict_start_id = Prediction(i, predict_start_id);
        compl_start_id = Complete(i, compl_start_id);
      }
    }
    State end("!", grammar.start_term, 1, 0);
    if (charts[word.size()].Contains(end)) {
      std::cout << "Yes\n";
      return true;
    }
    std::cout << "No\n";
    return false;
  }

  void Scan(size_t ind) {
    std::string str;
    str = word[ind];
    for (State& state : charts[ind].states) {
      if (state.Next() == str) {
        State new_state(state.left, state.right, state.dot_pos + 1, state.start_pos);
        charts[ind + 1].Add(new_state);
      }
    }
  }

  size_t Prediction(size_t ind, size_t start_id) {
    changes = false;
    size_t finish_id = charts[ind].states.size();
    for (size_t i = start_id; i < finish_id; ++i) {
      State state =  charts[ind].states[i];
      if (grammar.rules.contains(state.Next())) {
        for (auto& rule : grammar.rules[state.Next()]) {
          State new_state(state.Next(), rule, 0, ind);
          changes |= charts[ind].Add(new_state);
        }
      }
    }
    return finish_id;
  }

  size_t Complete(size_t ind, size_t start_id) {
    size_t finish_id = charts[ind].next_sign_states["$"].size();
    for (size_t i = start_id; i < finish_id; ++i) {
      State state = charts[ind].next_sign_states["$"][i];
      for (auto& comp_state : charts[state.start_pos].next_sign_states[state.left]) {
        State new_state(comp_state.left, comp_state.right, comp_state.dot_pos + 1, comp_state.start_pos);
        changes |= charts[ind].Add(new_state);
      }
    }

    for (auto& state : charts[ind].complete_small_states) {
      for (auto& comp_state : charts[ind].next_sign_states[state.left]) {
        State new_state(comp_state.left, comp_state.right, comp_state.dot_pos + 1, comp_state.start_pos);
        changes |= charts[ind].Add(new_state);
      }
    }
    return finish_id;
  }
};

void fit_for_test(Grammar& grammar, const std::string& gram_str);
void fit(Grammar& grammar);