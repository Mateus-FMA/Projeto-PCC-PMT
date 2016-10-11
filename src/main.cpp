#include <algorithm>
#include <iostream>
#include <vector>

#include <getopt.h>

#include "boyer_moore.h"
#include "brute_force.h"
#include "kmp.h"
#include "ukkonen.h"
#include "utils.h"

enum class StringMatcher {
  kBoyerMoore,
  kDefault,
  kKnuthMorrisPratt,
  kSellers,
  kUkkonen
};

void PrintHelpFunction() {

}

int main(int argc, char *argv[]) {
  option long_options[] = {
    // Additional option if the user wants to know the usage of this tool.
    {"help", no_argument, nullptr, 'h'},
    // Options from project specification.
    {"algorithm", required_argument, nullptr, 'a'},
    {"count", no_argument, nullptr, 'c'},
    {"edit", required_argument, nullptr, 'e'},
    {"pattern", required_argument, nullptr, 'p'}
  };

  int c = 0;
  int option_index = 0;

  StringMatcher algorithm = StringMatcher::kDefault;
  int max_edit_distance = 0;
  bool num_occurrences_only = false;
  std::string option_arg;
  std::string pattern_file;
  size_t sz;

  while ((c = getopt_long(argc, argv, "a:ce:hp:", long_options, &option_index)) != -1) {
    switch (c) {
      case 'a':
        option_arg = optarg;
        
        if (!option_arg.compare("bm")) {
          algorithm = StringMatcher::kBoyerMoore;
        } else if (!option_arg.compare("kmp")) {
          algorithm = StringMatcher::kKnuthMorrisPratt;
        } else if (!option_arg.compare("sel")) {
          algorithm = StringMatcher::kSellers;
        } else if (!option_arg.compare("ukk")) {
          algorithm = StringMatcher::kUkkonen;
        } else {
          std::cout << "Program exited with code -1: inexistent option." << std::endl;
          return -1;
        }

        break;

      case 'c':
        num_occurrences_only = true;
        break;

      case 'e':
        try {
          max_edit_distance = std::stoi(option_arg, &sz);

          if (sz != option_arg.size()) {
            std::cout << "Program exited with code -2: invalid edit distance (must be a positive "
                << "integer)." << std::endl;
            return -2;
          }
        } catch (std::invalid_argument &e) {
          std::cout << e.what() << std::endl;
          std::cout << "Program exited with code -2: invalid edit distance (must be a positive "
              << "integer)." << std::endl;
          return -2;
        } catch (std::out_of_range &e) {
          std::cout << e.what() << std::endl;
          std::cout << "Program exited with code -3." << std::endl;
          return -3;
        }

        break;

      case 'h':
        PrintHelpFunction();
        return 0;

      case 'p':
        break;

      default:
        std::cout << "Program exited with code -1: inexistent option." << std::endl;
        return -1;
    }
  }
  
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
