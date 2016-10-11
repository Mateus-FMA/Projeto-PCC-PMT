#ifndef PMT_INCLUDE_UTILS_H_
#define PMT_INCLUDE_UTILS_H_

#include <string>
#include <vector>

namespace pmt {

std::string RemoveRepeatedLetters(const std::string &str);
std::string PrintOccurrences(const std::vector<int> &occurrences);

}  // namespace pmt

#endif  // PMT_INCLUDE_UTILS_H_

