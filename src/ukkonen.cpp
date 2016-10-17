#include "ukkonen.h"

#include <algorithm>
#include <array>
#include <deque>
#include <functional>
#include <numeric>
#include <unordered_set>
#include <utility>

#include "utils.h"

namespace pmt {
namespace {

// TODO(Mateus): only supports ASCII currently.
class TransitionFunction {
 public:
  TransitionFunction() : rows_(0), cols_(0) {}
  ~TransitionFunction() {}

  int& operator()(int label, uchar c) {
    int col = static_cast<int>(c);

    if (rows_ <= label || cols_ <= col) {
      int max_rows = std::max(rows_, (label + 1));
      int max_cols = std::max(cols_, (col + 1));

      table_.resize(max_rows * max_cols);
      std::fill(table_.begin() + rows_ * cols_, table_.end(), -1);

      rows_ = max_rows;
      cols_ = max_cols;
    }

    return table_[label * cols_ + col];
  }

 private:
  std::vector<int> table_;
  int rows_;
  int cols_;
};

const int kNonLeafNode = -1;

struct UkkonenTTreeNode {
  UkkonenTTreeNode() : left(nullptr), mid(nullptr), right(nullptr), label(kNonLeafNode) {}
  ~UkkonenTTreeNode() {
    delete left;
    delete mid;
    delete right;
  }

  UkkonenTTreeNode *left;
  UkkonenTTreeNode *mid;
  UkkonenTTreeNode *right;

  int label;
  std::vector<int> state;
};

class UkkonenTernaryTree {
 public:
  UkkonenTernaryTree() : root_(nullptr), num_states_(0) {}
  ~UkkonenTernaryTree() { delete root_; }

  UkkonenTTreeNode* InsertState(const std::vector<int> &state) {
    if (!state.empty() && !root_) {
      root_ = new UkkonenTTreeNode();
    }

    UkkonenTTreeNode *current = root_;
    for (size_t i = 1; i < state.size(); ++i) {
      int edge_label = state[i] - state[i - 1];

      if (edge_label == -1) {
        if (!current->left) current->left = new UkkonenTTreeNode();
        current = current->left;
      } else if (edge_label == 0) {
        if (!current->mid) current->mid = new UkkonenTTreeNode();
        current = current->mid;
      } else {  // edge_label == 1.
        if (!current->right) current->right = new UkkonenTTreeNode();
        current = current->right;
      }
    }

    current->state = state;
    current->label = num_states_++;

    return current;
  }

  UkkonenTTreeNode* SearchState(const std::vector<int> &state) const {
    if (!root_) {
      return nullptr;
    }

    UkkonenTTreeNode *current = root_;
    for (size_t i = 1; i < state.size(); ++i) {
      int edge_label = state[i] - state[i - 1];
      
      if (edge_label == -1) {
        current = current->left;
      } else if (edge_label == 0) {
        current = current->mid;
      } else {  // edge_label == 1.
        current = current->right;
      }
      
      if (!current) {
        return nullptr;
      }
    }

    return current;
  }

 private:
  UkkonenTTreeNode *root_;
  int num_states_;
};

void NextColumn(const std::vector<int> &current, const std::string &pattern, char b,
                int max_edit_distance, std::vector<int> *next) {
  // Possible values for i-th index on next column.
  std::array<int, 4> candidates;
  candidates[0] = max_edit_distance + 1;

  for (size_t i = 1; i < current.size(); ++i) {
    candidates[1] = pattern[i - 1] == b ? current[i - 1] : current[i - 1] + 1;
    candidates[2] = current[i] + 1;
    candidates[3] = (*next)[i - 1] + 1;

    (*next)[i] = *std::min_element(candidates.begin(), candidates.end());
  }
}

// TODO(Mateus): only supports ASCII (same thing from bad-character table on boyer_moore.cpp).
void BuildUkkonenAFD(const std::string &pattern, const std::string &text, int max_edit_distance,
                     TransitionFunction *delta, std::unordered_set<int> *final_states) {
  int m = static_cast<int>(pattern.size());
  if (m <= max_edit_distance) {
    final_states->insert(0);
  }

  std::string alphabet = RemoveRepeatedLetters(pattern + text);
  std::vector<int> state(m + 1);

  std::iota(state.begin(), state.end(), 0);

  UkkonenTernaryTree Q;
  std::deque<UkkonenTTreeNode*> N(1, Q.InsertState(state));
  int i = 0;

  while (!N.empty()) {
    UkkonenTTreeNode *node = N.front();
    N.pop_front();

    for (size_t j = 0; j < alphabet.size(); ++j) {
      NextColumn(node->state, pattern, alphabet[j], max_edit_distance, &state);
      UkkonenTTreeNode *next_node = Q.SearchState(state);

      if (!next_node) {
        N.push_back(Q.InsertState(state));
        ++i;

        if (state[m] <= max_edit_distance) {
          final_states->insert(i);
        }

        (*delta)(node->label, static_cast<uchar>(alphabet[j])) = i;
      } else {
        (*delta)(node->label, static_cast<uchar>(alphabet[j])) = next_node->label;
      }
    }
  }
}

}  // namespace

std::vector<int> UkkonenStringMatcher(const std::string &pattern, const std::string &text,
                                      int max_edit_distance) {
  int n = static_cast<int>(text.size());
  TransitionFunction delta;
  std::unordered_set<int> final_states;

  BuildUkkonenAFD(pattern, text, max_edit_distance, &delta, &final_states);

  std::vector<int> occurrences;
  int q = 0;

  for (int j = 0; j < n; ++j) {
    q = delta(q, static_cast<uchar>(text[j]));

    if (final_states.find(q) != final_states.end()) {
      occurrences.push_back(j);
    } 
  }

  return occurrences;
}

}  // namespace pmt

