#include "sellers.h"

#include <algorithm>
#include <array>
#include <numeric>

namespace pmt {

std::vector<int> SellersStringMatcher(const std::string &pattern, const std::string &text,
                                      int error){
  int m = static_cast<int>(pattern.size());
  int n = static_cast<int>(text.size());

  std::vector<int> current_col(m + 1);
  std::vector<int> next_col(m + 1);
  std::array<int, 3> candidates;

  std::iota(current_col.begin(), current_col.end(), 0);

  std::vector<int> occurrences;

  for (int j = 1; j <= n; ++j) {
    next_col[0] = 0;

    for (int i = 1; i <= m; ++i) {
      candidates[0] = pattern[i - 1] == text[j - 1] ? current_col[i - 1] : current_col[i - 1] + 1;
      candidates[1] = current_col[i] + 1;
      candidates[2] = next_col[i - 1] + 1;

      next_col[i] = *std::min_element(candidates.begin(), candidates.end());
    }

    if (next_col[m] <= error) {
      occurrences.push_back(j);
    }

    std::copy(next_col.begin(), next_col.end(), current_col.begin());
  }

  return occurrences;
}

std::vector<int> SellersMultiStringMatcher(const std::string &pattern,
                                           const std::vector<std::string> &text_list,
                                           int error) {
  std::vector<int> occurrences;

  for (size_t i = 0; i < text_list.size(); ++i) {
    std::vector<int> tmp = SellersStringMatcher(pattern, text_list[i], error);
    occurrences.insert(occurrences.end(), tmp.begin(), tmp.end());
  }

  return occurrences;
}

} // namespace pmt

