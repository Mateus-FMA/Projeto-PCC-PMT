#include <algorithm>
#include <iostream>
#include <vector>

#include "boyer_moore.h"
#include "brute_force.h"
#include "kmp.h"
#include "ukkonen.h"
#include "utils.h"

int main() {
  std::string pattern = "abcabcacab";
  std::string text = "babcbabcabcaabcabcabcacabc";

  std::vector<int> bf_occurrences = pmt::BruteForceStringMatcher(pattern, text);
  std::vector<int> kmp_occurrences = pmt::KMPStringMatcher(pattern, text);
  std::vector<int> bm_occurrences = pmt::BoyerMooreStringMatcher(pattern, text);
  std::vector<int> ukk_occurrences = pmt::UkkonenStringMatcher(pattern, text, 0);

  std::transform(ukk_occurrences.begin(), ukk_occurrences.end(), ukk_occurrences.begin(),
                 [&pattern] (int val) -> int { 
                   return val + 1 - pattern.size() > 0 ? val + 1 - pattern.size() : 0;
                 });

  std::cout << pmt::PrintOccurrences(bf_occurrences) << std::endl;
  std::cout << pmt::PrintOccurrences(kmp_occurrences) << std::endl;
  std::cout << pmt::PrintOccurrences(bm_occurrences) << std::endl;
  std::cout << pmt::PrintOccurrences(ukk_occurrences) << std::endl;

  return 0;
}
