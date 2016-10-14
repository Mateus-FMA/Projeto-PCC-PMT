#ifndef PMT_INCLUDE_SELLERS_H_
#define PMT_INCLUDE_SELLERS_H_

#include <string>
#include <vector>

namespace pmt{

std::vector<int> SellersStringMatcher(const std::string &pattern, const std::string &text,
                                      int error);
std::vector<int> SellersMultiStringMatcher(const std::string &pattern,
                                           const std::vector<std::string> &text,
                                           int error);

}  // namespace pmt

#endif  // PMT_INCLUDE_SELLERS_H_

