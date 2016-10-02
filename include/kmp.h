#ifndef PMT_INCLUDE_KMP_
#define PMT_INCLUDE_KMP_

#include <string>
#include <vector>

namespace pmt {

std::vector<int> KMPStringMatcher(const std::string &pattern, const std::string &text);
std::vector<int> KMPMultiStringMatcher(const std::string &pattern,
																			 const std::vector<std::string> &text_list);

}  // namespace pmt

#endif  // PMT_INCLUDE_KMP_

