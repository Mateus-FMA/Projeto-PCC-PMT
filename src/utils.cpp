#include "utils.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <unordered_map>

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

}  // namespace pmt

