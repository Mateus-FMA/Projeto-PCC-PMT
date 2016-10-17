#ifndef PMT_INCLUDE_UTILS_H_
#define PMT_INCLUDE_UTILS_H_

#include <string>
#include <vector>

namespace pmt {

typedef unsigned char uchar;

std::string RemoveRepeatedLetters(const std::string &str);
std::string PrintOccurrences(const std::vector<int> &occurrences, const std::string &text,
                             int pattern_length);
std::vector<std::string> GetFilenames(const std::string &pattern);

}  // namespace pmt

#endif  // PMT_INCLUDE_UTILS_H_

