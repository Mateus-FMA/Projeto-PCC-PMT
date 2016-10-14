#include "kmp.h"

namespace pmt {
namespace {

std::vector<int> ComputeStrictBorderTable(const std::string &pattern) {
  int m = static_cast<int>(pattern.size());
  std::vector<int> s_bord(m + 1, -1);
  int t = -1;

  for (int j = 0; j < m; ++j) {
    // Same (initial) idea as the sub-routine ComputeBorderTable on boyer_moore.cpp.
    while (t >= 0 && pattern[t] != pattern[j]) {
      t = s_bord[t];
    }

    ++t;
    // If pattern[t] != pattern[j + 1], then the longest strict border of pattern[:j + 1] is t
    // itself; otherwise, it is the previous longest strict border. If j + 1 == m, s_bord[m] = t
    // (which is actually bord[m]).
    if (j == m - 1 || pattern[t] != pattern[j + 1]) {
      s_bord[j + 1] = t;
    } else {
      s_bord[j + 1] = s_bord[t];
    }
  }

  return s_bord;
}

std::vector<int> KMPStringMatcher(const std::string &pattern, const std::string &text,
                                  const std::vector<int> &s_bord) {
  int m = static_cast<int>(pattern.size());
  int n = static_cast<int>(text.size());

  int i = 0;
  int j = 0;
  std::vector<int> occurrences;

  while (i <= n - m) {
    while (j < m && pattern[j] == text[i + j]) {
      ++j;
    }

    if (j == m) {
      occurrences.push_back(i);
    }

    i += j - s_bord[j];
    j = s_bord[j] > 0 ? s_bord[j] : 0;
  }

  return occurrences;
}

}  // namespace

std::vector<int> KMPStringMatcher(const std::string &pattern, const std::string &text) {
  return KMPStringMatcher(pattern, text, ComputeStrictBorderTable(pattern));
}

std::vector<int> KMPMultiStringMatcher(const std::string &pattern,
                                       const std::vector<std::string> &text_list) {
  std::vector<int> s_bord = ComputeStrictBorderTable(pattern);
  std::vector<int> occurrences;

  for (size_t i = 0; i < text_list.size(); ++i) {
    std::vector<int> tmp = KMPStringMatcher(pattern, text_list[i], s_bord);
    occurrences.insert(occurrences.end(), tmp.begin(), tmp.end());
  }

  return occurrences;
}

}  // namespace pmt

