#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iomanip>
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
  std::cout << "Usage: pmt [options] pattern textfile [textfile ...]\n\n  \"pattern\" is the "
      << "word you want to find in the specified text.\n  \"textfile\" is the file with the text "
      << "to be processed (wildcards are also supported to choose multiple files, but you need to "
      << "specify the argument under quotes due to the parsing of the options).\n\nAvailable option"
      << "s:\n    -a --algorithm\tSpecifies a string matching algorithm supported by this\n\t\t\t"
      << "tool. Valid arguments are: \"bm\" (Boyer-Moore), \"kmp\"\n\t\t\t(Knuth-Morris-Pratt), "
      << "\"sel" "\" (Sellers) and \"ukk\"\n\t\t\t(Ukkonen). If Sellers or Ukkonen are chosen, "
      << "option -e\n\t\t\tor --edit must be enabled.\n\n    " << std::setw(14) << std::left 
      << "-c --count" << "\tPrint only the number of occurrences of the specified\n\t\t\tpattern "
      << "on the text.\n\n    " << std::setw(14) << std::left << "-e --edit" << "\tMaximum edit "
      << "distance. Used for approximate string\n\t\t\tmatching.\n\n    " << std::setw(14)
      << std::left << "-p --pattern" << "\tSpecifies a file containing all the patterns which we "
      << "\n\t\t\twant to find on the text. If this option is enabled,\n\t\t\t\"pattern\" will be "
      << "interpreted as a text file.\n" << std::endl;

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
  int max_edit_distance = -1;
  bool num_occurrences_only = false;
  std::string option_arg;
  std::vector<std::string> pattern_files;
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
          std::cout << "Program exited with code 1: inexistent algorithm." << std::endl;
          return 1;
        }

        break;

      case 'c':
        num_occurrences_only = true;
        break;

      case 'e':
        try {
          option_arg = optarg;
          max_edit_distance = std::stoi(option_arg, &sz);

          if (sz != option_arg.size() || max_edit_distance < 0) {
            std::cout << "Program exited with code -2: invalid edit distance (must be a "
                << "non-negative integer)." << std::endl;
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
        option_arg = optarg;
        pattern_files = pmt::GetFilenames(option_arg);

        break;

      default:
        std::cout << "Program exited with code -1: inexistent option." << std::endl;
        return -1;
    }
  }
  
  if (max_edit_distance >= 0 && (algorithm == StringMatcher::kBoyerMoore ||
      algorithm == StringMatcher::kKnuthMorrisPratt)) {
    std::cout << "Program exited with code 2: cannot use KMP or Boyer-Moore algorithms on "
        << "approximate string matching. Disable option -e or --edit to perform exact string "
        << "matching." << std::endl;
    return 2;
  } else if (max_edit_distance < 0 && (algorithm == StringMatcher::kSellers ||
             algorithm == StringMatcher::kUkkonen)) {
    std::cout << "Program exited with code 3: cannot use Sellers or Ukkonen algorithms on exact "
        << "string matching. Enable option -e or --edit to perform approximate string matching."
        << std::endl;
    return 3;
  }

  if (optind >= argc) {
    std::cout << "Program exited with code 4: incorrect number of arguments (type pmt --help for "
        << "more details)." << std::endl;
    return 4;
  }

  std::vector<std::string> patterns;

  if (pattern_files.empty()) {
    patterns.push_back(argv[optind++]);

    if (optind >= argc) {
      std::cout << "Program exited with code 4: incorrect number of arguments (type pmt --help for "
          << "more details)." << std::endl;
      return 4;
    }
  } else {
    for (size_t i = 0; i < pattern_files.size(); ++i) {
      std::ifstream ifs(pattern_files[i], std::ifstream::binary);
      if (!ifs) {
        std::cout << "Program exited with code 5: cannot open file(s) from argument list."
            << std::endl;
        return 5;
      }

      std::string line;      
      while (std::getline(ifs, line)) {
        patterns.push_back(line);
      }

      ifs.close();
    }
  }
  
  pmt::KMPStringMatcher *kmp = nullptr;
  pmt::BoyerMooreStringMatcher *bm = nullptr;

  const int kMaxSize = 1 << 20;  // 1 MB.
  std::array<char, kMaxSize> buffer;

  for (size_t i = 0; i < patterns.size(); ++i) {
    std::vector<int> occurrences;
    // Read all text files and return the occurrences of the current pattern for each file.
    for (int j = optind; j < argc; ++j) {
      std::ifstream ifs(argv[j], std::ifstream::binary);
      if (!ifs) {
        std::cout << "Program exited with code 5: cannot open file(s) from argument list."
            << std::endl;
        return 5;
      }

      while (ifs.get(buffer.data(), kMaxSize, '\0')) {
        std::vector<int> partial_occ;
        
        if (max_edit_distance < 0) {  // Exact string matching.
          switch (algorithm) {
            case StringMatcher::kBoyerMoore:
              if (!bm) {
                bm = new pmt::BoyerMooreStringMatcher(patterns[i]);
              } else {
                bm->Reset(patterns[i]);
              }

              partial_occ = bm->FindOccurrences(buffer.data());
              break;

            case StringMatcher::kKnuthMorrisPratt:
              if (!kmp) {
                kmp = new pmt::KMPStringMatcher(patterns[i]);
              } else {
                kmp->Reset(patterns[i]);
              }

              partial_occ = kmp->FindOccurrences(buffer.data());
              break;

            default:  // The default approach of this tool for exact string matching.
              if (patterns[i].size() <= 1) {
                partial_occ = pmt::BruteForceStringMatcher(patterns[i], buffer.data());
              } else {
                if (!bm) {
                  bm = new pmt::BoyerMooreStringMatcher(patterns[i]);
                } else {
                  bm->Reset(patterns[i]);
                }

                partial_occ = bm->FindOccurrences(buffer.data());
              }

              break;
          }
        } else {  // Approximate string matching.
          switch (algorithm) {
            case StringMatcher::kSellers:
              // TODO(Mateus): add Sellers algorithm here later.
              break;

            case StringMatcher::kUkkonen:
              partial_occ = pmt::UkkonenStringMatcher(patterns[i], buffer.data(),
                                                      max_edit_distance);
              break;

            default:  // The default approach of this tool for approximate string matching.
              // TODO(Mateus): add Sellers algorithm here later.
              partial_occ = pmt::UkkonenStringMatcher(patterns[i], buffer.data(),
                                                      max_edit_distance);
              break;
          }

          std::transform(partial_occ.begin(), partial_occ.end(), partial_occ.begin(),
                         [&patterns, i] (int val) -> int { 
                            return val + 1 - patterns[i].size() > 0 ?
                                   val + 1 - patterns[i].size() : 0;
                         });
        }

        occurrences.insert(occurrences.end(), partial_occ.begin(), partial_occ.end());
      }

      ifs.close();
    }

    std::cout << "  Padrão " << i + 1 << ": ";

    if (num_occurrences_only) {
      std::cout << occurrences.size() << std::endl;
    } else {
      std::cout << pmt::PrintOccurrences(occurrences) << std::endl;
    }
  }
  

  delete kmp;
  delete bm;

  return 0;
}
