#include "boyer_moore.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <unordered_map>

namespace pmt {
namespace {

std::string RemoveRepeatedLetters(const std::string &str) {
  std::unordered_map<char, int> container;
  std::ostringstream oss;
  std::copy_if(str.begin(), str.end(), std::ostream_iterator<char>(oss),
               [&container] (char c) -> bool { return container[c]++ == 0; });

  return oss.str();
}

std::vector<int> ComputeBorderTable(const std::string &pattern) {
  int m = static_cast<int>(pattern.size());
  std::vector<int> bord(m + 1, -1);
  int t = -1;

  for (int j = 0; j < m; ++j) {
    // By induction, one may prove that the sequence of all borders of a string x is
    // (x, Border(x), ... , Border^k(x)), where k is the smallest non-negative integer such that
    // Border^k(x) is the empty word. It then follows that if bord[j + 1] > 0, then bord[j + 1] =
    // t + 1, where t = bord^k[j + 1] and k is the smallest positive integer such that
    // pattern[Bord^k[j + 1] + 1] = pattern[j], j = 0, ... , m - 1. Note that bord[0] = -1, for all
    // strings x.
    while (t >= 0 && pattern[t + 1] != pattern[j]) {
      t = bord[t];
    }

    ++t;
    bord[j + 1] = t;
  }

  return bord;
}

std::vector<int> ComputeGoodSuffixTable(const std::string &pattern) {
  int m = static_cast<int>(pattern.size());
  std::vector<int> bord = ComputeBorderTable(pattern);
  const int max_shift = m - bord[m];
  std::vector<int> good_shift(m + 1, max_shift);

  for (int j = 0; j < m; ++j) {
    int s = 1;

    while (s < max_shift) {
      if (s < j && pattern[j - s] == pattern[j]) {
	      ++s;
	      continue;			
      }

      int k = j + 1;

      while (k < m) {
	      if (s < k && pattern[k - s] != pattern[k]) {
		      ++s;
		      break;
	      }

	      ++k;
      }

      if (k == m) {
	      break;			
      }
    }

    good_shift[j + 1] = s;
  }

  return good_shift;
}

// TODO(Mateus): implementar esta função de tal modo que se aceite caracteres fora do alfabeto
// ASCII.
std::unordered_map<char, int> ComputeBadCharacterTable(const std::string &pattern,
																											 const std::string &alphabet) {
  std::unordered_map<char, int> bad_character;

  for (size_t i = 0; i < alphabet.size(); ++i) {
    bad_character[alphabet[i]] = -1;
  }

  for (size_t i = 0; i < pattern.size(); ++i) {
    bad_character[pattern[i]] = i;
  }

  return bad_character;
}

std::vector<int> BoyerMooreStringMatcher(const std::string &pattern, const std::string &text,
																				 const std::unordered_map<char, int> &bad_character,
																				 const std::vector<int> &good_suffix) {
  int m = static_cast<int>(pattern.size());
  int n = static_cast<int>(text.size());

  int i = 0;
  std::vector<int> occurrences;

  while (i <= n - m) {
    int j = m - 1;

    while (j >= 0 && pattern[j] == text[i + j]) {
      --j;
    }

    if (j < 0) {
      occurrences.push_back(i);
      i += good_suffix[0];
    } else {
      int gs_shift = good_suffix[j];
      int bc_shift = j - bad_character.at(text[i + j]);
      i += gs_shift > bc_shift ? gs_shift : bc_shift;
    }
  }

  return occurrences;
}

}  // namespace

std::vector<int> BoyerMooreStringMatcher(const std::string &pattern, const std::string &text) {
  std::string alphabet = RemoveRepeatedLetters(pattern + text);

  return BoyerMooreStringMatcher(pattern, text, ComputeBadCharacterTable(pattern, alphabet),
                                 ComputeGoodSuffixTable(pattern));
}

std::vector<int> BoyerMooreMultiStringMatcher(const std::string &pattern,
                                              const std::vector<std::string> &text_list) {
  std::string all_chars = pattern;

  for (size_t i = 0; i < text_list.size(); ++i) {
    all_chars += text_list[i];
  }

  std::string alphabet = RemoveRepeatedLetters(all_chars);
  std::unordered_map<char, int> bad_character = ComputeBadCharacterTable(pattern, alphabet);
  std::vector<int> good_suffix = ComputeGoodSuffixTable(pattern);
  std::vector<int> occurrences;

  for (size_t i = 0; i < text_list.size(); ++i) {
    std::vector<int> tmp = BoyerMooreStringMatcher(pattern, text_list[i], bad_character,
                                                   good_suffix);
    occurrences.insert(occurrences.end(), tmp.begin(), tmp.end());
  }

  return occurrences;
}

}  // namespace pmt

