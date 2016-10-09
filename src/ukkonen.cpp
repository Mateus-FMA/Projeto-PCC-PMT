#include "ukkonen.h"

#include <algorithm>
#include <array>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace pmt {
namespace {

// TODO(Mateus): although this solution is better than a sequential search on a vector/list on
// average, it is still very prone to collisions.
struct PairHash {
  size_t operator() (const std::pair<int, char> &p) const {
    return std::hash<int>()(p.first) ^ std::hash<char>()(p.second);
  }
};

struct UkkonenTTreeNode {
  UkkonenTTreeNode() : left(nullptr), mid(nullptr), right(nullptr) {}
  ~UkkonenTTreeNode() {
    delete left;
    delete mid;
    delete right;
  }

  UkkonenTTreeNode *left;
  UkkonenTTreeNode *mid;
  UkkonenTTreeNode *right;
};

class UkkonenTernaryTree {
 public:
  UkkonenTernaryTree() : root_(nullptr) {}
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
  }

  bool Contains(const std::vector<int> &state) {
    if (!root_) return false;

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
      
      if (!current) return false;
    }

    return true;
  }

 private:
  UkkonenTTreeNode *root_;
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

void BuildUkkonenAFD(const std::string &pattern, const std::string &text, int max_edit_distance,
                     std::unordered_map<std::pair<int, char>, int, PairHash> *delta,
                     std::unordered_set<int> *final_states) {
  // TODO(Mateus): pensar em uma implementação eficiente para INDEX(S).
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

  for (int j = 0; j < n; ++j) {
    q = delta[std::make_pair(q, text[j])];

    if (final_states.find(q) != final_states.end()) {
      occurrences.push_back(j);
    } 
  }

  return occurrences;
}

}  // namespace pmt

