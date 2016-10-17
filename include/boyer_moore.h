#ifndef PMT_INCLUDE_BOYER_MOORE_
#define PMT_INCLUDE_BOYER_MOORE_

#include <array>
#include <string>
#include <vector>

namespace pmt {

class BoyerMooreStringMatcher {
 public:
  BoyerMooreStringMatcher();
  explicit BoyerMooreStringMatcher(const std::string &pattern);

  std::vector<int> FindOccurrences(const std::string &text);
  void SetPattern(const std::string &pattern);

 private:
  // TODO(Mateus): only supports ASCII by now.
  static const int kAlphabetSize = 256;

  void ComputeBadCharacterTable();
  std::vector<int> ComputeBorderTable(const std::string &pattern);
  void ComputeGoodSuffixTable();

  std::string pattern_;
  std::vector<int> good_suffix_;
  std::array<int, kAlphabetSize> bad_character_;
};

}

#endif // PMT_INCLUDE_BOYER_MOORE_

