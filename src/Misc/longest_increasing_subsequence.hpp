#pragma once
#include <vector>
#include <algorithm>
template <class T> std::pair<std::vector<int>, std::vector<std::vector<int>>> longest_increasing_subsequence(const std::vector<T> &a, bool strict= true) {
 int n= a.size();
 std::vector<int> idx(n);
 std::vector<T> dp(n);
 int len= 0;
 if (strict)
  for (int i= 0; i < n; ++i) {
   auto it= std::lower_bound(dp.begin(), dp.begin() + len, a[i]);
   if (*it= a[i]; (idx[i]= it - dp.begin()) == len) ++len;
  }
 else
  for (int i= 0; i < n; ++i) {
   auto it= std::upper_bound(dp.begin(), dp.begin() + len, a[i]);
   if (*it= a[i]; (idx[i]= it - dp.begin()) == len) ++len;
  }
 std::vector<std::vector<int>> cand(len);
 for (int i= n; i--;) {
  if (idx[i] == len - 1 || (!cand[idx[i] + 1].empty() && a[i] < a[cand[idx[i] + 1].back()])) cand[idx[i]].emplace_back(i);
  else idx[i]= -1;
 }
 for (auto &c: cand) std::reverse(c.begin(), c.end());
 return {idx, cand};
}