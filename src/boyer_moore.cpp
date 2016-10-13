#include "boyer_moore.h"

#include <algorithm>
#include <unordered_map>

#include "utils.h"

namespace pmt {

BoyerMooreStringMatcher::BoyerMooreStringMatcher(const std::string &pattern)
    : good_suffix_(pattern.size() + 1, static_cast<int>(pattern.size())),
      pattern_(pattern) {
  ComputeGoodSuffixTable();
}

// TODO(Mateus): implement this function so it may support non-ASCII characters. Must also find
// a more elegant way to write this, if possible...
void BoyerMooreStringMatcher::ComputeBadCharacterTable(const std::string &alphabet) {
  for (size_t i = 0; i < alphabet.size(); ++i) {
    bad_character_[alphabet[i]] = -1;
  }

  for (size_t i = 0; i < pattern_.size(); ++i) {
    bad_character_[pattern_[i]] = i;
  }
}

std::vector<int> BoyerMooreStringMatcher::ComputeBorderTable(const std::string &pattern) {
  int m = static_cast<int>(pattern.size());
  std::vector<int> bord(m + 1, -1);

  for (int j = 0; j < m; ++j) {
    // By induction, one may prove that the sequence of all borders of a string x is
    // (x, Border(x), ... , Border^k(x)), where k is the smallest non-negative integer such that
    // Border^k(x) is the empty word. It then follows that if bord[j + 1] > 0, then bord[j + 1] =
    // t + 1, where t = bord^k[j + 1] and k is the smallest positive integer such that
    // pattern[Bord^k[j + 1] + 1] = pattern[j], j = 0, ... , m - 1. Note that bord[0] = -1, for all
    // strings x.
    int t = bord[j];
    while (t >= 0 && pattern[t] != pattern[j]) {
      t = bord[t];
    }

    bord[j + 1] = t + 1;
  }

  return bord;
}

void BoyerMooreStringMatcher::ComputeGoodSuffixTable() {
  int m = static_cast<int>(pattern_.size());
  std::string pattern_r(pattern_);

  std::reverse(pattern_r.begin(), pattern_r.end());
  std::vector<int> bord_r = ComputeBorderTable(pattern_r);

  // Preprocessing 1 - find the length N of the LCS between pattern[:j] and pattern. If it exists,
  // then N = m - l + bord_r[l], 1 <= l < m, and good_shift[j] = m - N (where j = m -  bord_r[l])
  // if, and only if, pattern[j - (m - N) - 1] != pattern[j - 1].
  for (int l = 1; l < m; ++l) {
    int j = m - bord_r[l];
    int s = l - bord_r[l];

    if (pattern_[j - s - 1] != pattern_[j - 1]) {
      good_suffix_[j] = good_suffix_[j] < s ? good_suffix_[j] : s;
    }
  }

  // Preprocessing 2 - if there's not a common suffix between pattern[:j] and pattern, then we must
  // get the maximum proper prefix of pattern (let Q be this prefix) such that Q is a suffix of
  // pattern[j:], j = 0, ... , m - 1. We know that Q is a border of pattern (it is a prefix and a
  // suffix of pattern). It then follows that Q = bord^k(pattern_r) for some positive integer k.
  // Finally, we have that good_suffix[j] = m - |Q|.
  int t = bord_r[m];
  int q = 0;

  while (t >= 0) {
    int s = m - t;

    for (int j = q; j <= s; ++j) {
      good_suffix_[j] = good_suffix_[j] < s ? good_suffix_[j] : s; 
    }

    q = s + 1;
    t = bord_r[t];
  }
}

std::vector<int> BoyerMooreStringMatcher::FindOccurrences(const std::string &text) {
  ComputeBadCharacterTable(RemoveRepeatedLetters(pattern_ + text));

  int m = static_cast<int>(pattern_.size());
  int n = static_cast<int>(text.size());

  int i = 0;
  std::vector<int> occurrences;

  while (i <= n - m) {
    int j = m - 1;

    while (j >= 0 && pattern_[j] == text[i + j]) {
      --j;
    }

    if (j < 0) {
      occurrences.push_back(i);
      i += good_suffix_[0];
    } else {
      int gs_shift = good_suffix_[j + 1];
      int bc_shift = j - bad_character_.at(text[i + j]);
      i += gs_shift > bc_shift ? gs_shift : bc_shift;
    }
  }

  return occurrences;
}

void BoyerMooreStringMatcher::Reset(const std::string &pattern) {
  if (pattern_.compare(pattern) != 0) {
    pattern_ = pattern;
    std::fill(good_suffix_.begin(), good_suffix_.end(), static_cast<int>(pattern.size()));
    ComputeGoodSuffixTable();
  }
}

}  // namespace pmt

