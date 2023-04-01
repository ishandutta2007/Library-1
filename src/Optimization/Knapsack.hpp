#pragma once
#include <vector>
#include <algorithm>
template <class value_t, class weight_t> class Knapsack {
 std::vector<std::pair<weight_t, value_t>> I, L, R, tmp;
public:
 Knapsack() {}
 void add(value_t v, weight_t w) { I.emplace_back(w, v); }
 void build() {
  const int n= I.size(), l= n / 2, r= n - l;
  int i= 0, u, s;
  for (s= u= 1, L.resize(1 << l), tmp.resize(1 << l); i < l; std::merge(L.begin(), L.begin() + u, L.begin() + u, L.begin() + u + u, tmp.begin()), ++i, s= u<<= 1, L.swap(tmp))
   for (auto [w, v]= I[i]; s--;) L[u | s]= {L[s].first + w, L[s].second + v};
  for (s= u= 1, R.resize(1 << r), tmp.resize(1 << r); i < n; std::merge(R.begin(), R.begin() + u, R.begin() + u, R.begin() + u + u, tmp.begin()), ++i, s= u<<= 1, R.swap(tmp))
   for (auto [w, v]= I[i]; s--;) R[u | s]= {R[s].first + w, R[s].second + v};
  for (i= 1; i < u; ++i) R[i].second= std::max(R[i].second, R[i - 1].second);
 }
 value_t solve(weight_t cap) const {
  value_t ret= 0;
  int j= R.size() - 1;
  for (auto [w, v]: L) {
   if (cap < w) break;
   for (auto c= cap - w; R[j].first > c;) --j;
   ret= std::max(ret, v + R[j].second);
  }
  return ret;
 }
};
