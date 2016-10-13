#include "utils.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <unordered_map>

#include <glob.h>

#include <iostream>

namespace pmt {

std::string RemoveRepeatedLetters(const std::string &str) {
  std::unordered_map<char, int> container;
  std::ostringstream oss;
  std::copy_if(str.begin(), str.end(), std::ostream_iterator<char>(oss),
               [&container] (char c) -> bool { return container[c]++ == 0; });

  return oss.str();
}

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

