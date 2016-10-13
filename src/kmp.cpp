#include "kmp.h"

#include <algorithm>

#include <iostream>

namespace pmt {

KMPStringMatcher::KMPStringMatcher(const std::string &pattern)
    : pattern_(pattern),
      s_bord_(pattern.size() + 1, -1) {
  ComputeStrictBorderTable();
}

void KMPStringMatcher::ComputeStrictBorderTable() {
  int m = static_cast<int>(pattern_.size());
  int t = -1;

  for (int j = 0; j < m - 1; ++j) {
    // Same (initial) idea as the sub-routine ComputeBorderTable on boyer_moore.cpp.
    while (t >= 0 && pattern_[t] != pattern_[j]) {
      t = s_bord_[t];
    }

    ++t;
    // If pattern[t] != pattern[j + 1], then the longest strict border of pattern[:j + 1] is t
    // itself; otherwise, it is the previous longest strict border.
    if (pattern_[t] != pattern_[j + 1]) {
      s_bord_[j + 1] = t;
    } else {
      s_bord_[j + 1] = s_bord_[t];
    }
  }

  s_bord_[m] = t + 1;
}

std::vector<int> KMPStringMatcher::FindOccurrences(const std::string &text) {
  int m = static_cast<int>(pattern_.size());
  int n = static_cast<int>(text.size());

  int i = 0;
  int j = 0;
  std::vector<int> occurrences;

  while (i <= n - m) {
    while (j < m && pattern_[j] == text[i + j]) {
      ++j;
    }

    if (j == m) {
      occurrences.push_back(i);
    }

    i += j - s_bord_[j];
    j = s_bord_[j] > 0 ? s_bord_[j] : 0;
  }

  return occurrences;
}

void KMPStringMatcher::Reset(const std::string &pattern) {
  if (pattern_.compare(pattern) != 0) {
    pattern_ = pattern;
    std::fill(s_bord_.begin(), s_bord_.end(), -1);
    ComputeStrictBorderTable();
    std::cout << "Construí a borda estrita..." << std::endl;
  }
}

}  // namespace pmt

