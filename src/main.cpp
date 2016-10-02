#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include "boyer_moore.h"
#include "brute_force.h"
#include "kmp.h"

std::string PrintOccurrences(const std::vector<int> &occurrences) {
  std::ostringstream oss;
  oss << "{";

  if (!occurrences.empty()) {
    std::copy(occurrences.begin(), occurrences.end() - 1,
              std::ostream_iterator<size_t>(oss, ", "));
    oss << occurrences.back();
  }

  oss	<< "}";

  return oss.str();
}

int main() {
  std::string pattern = "abcabcacab";
  std::string text = "babcbabcabcaabcabcabcacabc";
  std::vector<int> bf_occurrences = pmt::BruteForceStringMatcher(pattern, text);
  std::vector<int> kmp_occurrences = pmt::KMPStringMatcher(pattern, text);
  std::vector<int> bm_occurrences = pmt::BoyerMooreStringMatcher(pattern, text);

  std::cout << PrintOccurrences(bf_occurrences) << std::endl;
  std::cout << PrintOccurrences(kmp_occurrences) << std::endl;
  std::cout << PrintOccurrences(bm_occurrences) << std::endl;

  return 0;
}
