#include "ukkonen.h"

#include <algorithm>
#include <array>
#include <deque>
#include <functional>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "utils.h"

namespace pmt {
namespace {

// TODO(Mateus): although this solution is better than a sequential search on a vector/list on
// average, it is still very prone to collisions. Check better implementations later.
struct PairHash {
  size_t operator() (const std::pair<int, char> &p) const {
    return std::hash<int>()(p.first) ^ std::hash<char>()(p.second);
  }
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

  void InsertState(const std::vector<int> &state) {
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

    if (current) {
      current->state = state;
      current->label = num_states_++;
    }
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
                     std::unordered_map<std::pair<int, char>, int, PairHash> *delta,
                     std::unordered_set<int> *final_states) {
  int m = static_cast<int>(pattern.size());
  if (m <= max_edit_distance) {
    final_states->insert(0);
  }

  std::string alphabet = RemoveRepeatedLetters(pattern + text);
  std::vector<int> current(m + 1);
  std::vector<int> next(m + 1, 0);
  UkkonenTernaryTree Q;

  std::iota(current.begin(), current.end(), 0);
  Q.InsertState(current);

  std::deque<UkkonenTTreeNode*> N(1, Q.SearchState(current));
  int i = 0;

  while (!N.empty()) {
    UkkonenTTreeNode *node = N.front();
    N.pop_front();
    std::copy(node->state.begin(), node->state.end(), current.begin());

    for (size_t j = 0; j < alphabet.size(); ++j) {
      NextColumn(current, pattern, alphabet[j], max_edit_distance, &next);
      UkkonenTTreeNode *next_node = Q.SearchState(next);

      if (!next_node) {
        Q.InsertState(next);
        N.push_back(Q.SearchState(next));
        ++i;

        if (next[m] <= max_edit_distance) {
          final_states->insert(i);
        }

        (*delta)[std::make_pair(node->label, alphabet[j])] = i;
      } else {
        (*delta)[std::make_pair(node->label, alphabet[j])] = next_node->label;
      }
    }
  }
}

}  // namespace

std::vector<int> UkkonenStringMatcher(const std::string &pattern, const std::string &text,
                                      int max_edit_distance) {
  int n = static_cast<int>(text.size());
  std::unordered_map<std::pair<int, char>, int, PairHash> delta;
  std::unordered_set<int> final_states;

  BuildUkkonenAFD(pattern, text, max_edit_distance, &delta, &final_states);

  std::vector<int> occurrences;
  int q = 0;

  if (final_states.find(q) != final_states.end()) {
    occurrences.push_back(0);
  }

  for (int j = 0; j < n; ++j) {
    q = delta[std::make_pair(q, text[j])];

    if (final_states.find(q) != final_states.end()) {
      occurrences.push_back(j);
    } 
  }

  return occurrences;
}

}  // namespace pmt

