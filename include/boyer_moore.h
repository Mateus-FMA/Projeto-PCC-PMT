#ifndef PMT_INCLUDE_BOYER_MOORE_
#define PMT_INCLUDE_BOYER_MOORE_

#include <string>
#include <unordered_map>
#include <vector>

namespace pmt {

class BoyerMooreStringMatcher {
 public:
  explicit BoyerMooreStringMatcher(const std::string &pattern);
  ~BoyerMooreStringMatcher() {}

  std::vector<int> FindOccurrences(const std::string &text);
  void Reset(const std::string &pattern);

 private:
  void ComputeBadCharacterTable(const std::string &alphabet);
  std::vector<int> ComputeBorderTable(const std::string &pattern);  
  void ComputeGoodSuffixTable();

  std::string alphabet_;
  std::unordered_map<char, int> bad_character_;
  std::vector<int> good_suffix_;
  std::string pattern_;
};

}

#endif  // PMT_INCLUDE_BOYER_MOORE_

