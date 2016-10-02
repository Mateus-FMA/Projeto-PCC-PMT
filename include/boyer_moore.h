#ifndef PMT_INCLUDE_BOYER_MOORE_
#define PMT_INCLUDE_BOYER_MOORE_

#include <string>
#include <vector>

namespace pmt {

std::vector<int> BoyerMooreStringMatcher(const std::string &pattern, const std::string &text);
std::vector<int> BoyerMooreMultiStringMatcher(const std::string &pattern,
																						  const std::vector<std::string> &text);

}

#endif  // PMT_INCLUDE_BOYER_MOORE_
