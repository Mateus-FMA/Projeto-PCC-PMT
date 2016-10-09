#ifndef PMT_INCLUDE_UKKONEN_
#define PMT_INCLUDE_UKKONEN_

#include <string>
#include <vector>

namespace pmt {

std::vector<int> UkkonenStringMatcher(const std::string &pattern, const std::string &text,
                                      int max_edit_distance);

}  // namespace pmt

#endif  // PMT_INCLUDE_UKKONEN_

