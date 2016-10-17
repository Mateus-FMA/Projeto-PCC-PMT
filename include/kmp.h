#ifndef PMT_INCLUDE_KMP_
#define PMT_INCLUDE_KMP_

#include <string>
#include <vector>

namespace pmt {

class KMPStringMatcher {
 public:
  KMPStringMatcher() : s_bord_(1, -1) {}
  explicit KMPStringMatcher(const std::string &pattern);
  ~KMPStringMatcher() {}

  std::vector<int> FindOccurrences(const std::string &text);
  void SetPattern(const std::string &pattern);

 private:
  void ComputeStrictBorderTable();

  std::string pattern_;
  std::vector<int> s_bord_;
};

}  // namespace pmt

#endif  // PMT_INCLUDE_KMP_

