#include "utils.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <unordered_map>

#include <glob.h>

#include <iostream>

namespace pmt {
namespace {

const std::string kANSIRedColor = "\033[31m";
const std::string kANSIResetAll = "\033[0m";

}  // namespace


std::string RemoveRepeatedLetters(const std::string &str) {
  std::unordered_map<char, int> container;
  std::ostringstream oss;
  std::copy_if(str.begin(), str.end(), std::ostream_iterator<char>(oss),
               [&container] (char c) -> bool { return container[c]++ == 0; });

  return oss.str();
}

std::string PrintOccurrences(const std::vector<int> &occurrences, const std::string &text,
                             int pattern_length) {
  std::ostringstream oss;
  int line_start = 0;
  int last_idx = 0;

  for (size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '\n') {
      bool has_occurrence = false;
      int prev_last_idx = last_idx;

      for (size_t j = last_idx; j < occurrences.size(); ++j) {
        if (occurrences[j] > static_cast<int>(i)) {
          last_idx = static_cast<int>(j);
          break;
        } else {
          has_occurrence = true;
          if (j == occurrences.size() - 1) last_idx = static_cast<int>(j + 1);
        }
      }

      if (has_occurrence) {
        int line_pos = line_start;
        while (prev_last_idx < last_idx) {
          oss << text.substr(line_pos, occurrences[prev_last_idx] - line_pos);
          oss << kANSIRedColor << text.substr(occurrences[prev_last_idx], pattern_length)
              << kANSIResetAll;

          line_pos = occurrences[prev_last_idx] + pattern_length;
          ++prev_last_idx;
        }

        oss << text.substr(line_pos, i - line_pos) << "\n";
      }

      line_start = static_cast<int>(i + 1);
    }
  }

  return oss.str();
}

std::vector<std::string> GetFilenames(const std::string &pattern) {
  glob_t glob_results;
  std::vector<std::string> filenames;

  glob(pattern.c_str(), GLOB_NOSORT, nullptr, &glob_results);
  filenames.reserve(glob_results.gl_pathc);

  for (size_t i = 0; i < glob_results.gl_pathc; ++i) {
    filenames.push_back(glob_results.gl_pathv[i]);
  }

  globfree(&glob_results);

  return filenames;
}

}  // namespace pmt

