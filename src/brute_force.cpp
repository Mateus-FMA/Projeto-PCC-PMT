#include "brute_force.h"

namespace pmt {

std::vector<int> BruteForceStringMatcher(const std::string &pattern, const std::string &text) {
  int m = static_cast<int>(pattern.size());
  int n = static_cast<int>(text.size());

  std::vector<int> occurrences;

  for (int i = 0; i <= n - m; ++i) {
    int j = 0;

    while (j < m && pattern[j] == text[i + j]) {
      ++j;
    }

    if (j == m) {
      occurrences.push_back(i);
    }
  }

  return occurrences;
}

}  // namespace pmt

