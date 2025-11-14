#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
struct Mo {
 std::vector<int> L, R;
 Mo() {}
 void query(int l, int r) { L.push_back(l), R.push_back(r); } /* [l, r) */
 template <typename AL, typename AR, typename EL, typename ER, typename O> void run(const AL &add_left, const AR &add_right, const EL &erase_left, const ER &erase_right, const O &out) {
  int q= L.size(), n= *std::max_element(R.begin(), R.end()), bs= n / std::min<int>(n, std::sqrt(q));
  std::vector<int> ord(q);
  std::iota(ord.begin(), ord.end(), 0), std::sort(ord.begin(), ord.end(), [&](int a, int b) {
   int ablk= L[a] / bs, bblk= L[b] / bs;
   return ablk != bblk ? ablk < bblk : (ablk & 1) ? R[a] > R[b] : R[a] < R[b];
  });
  int l= 0, r= 0;
  for (auto i: ord) {
   while (l > L[i]) add_left(--l);
   while (r < R[i]) add_right(r++);
   while (l < L[i]) erase_left(l++);
   while (r > R[i]) erase_right(--r);
   out(i);
  }
 }
 template <typename A, typename E, typename O> void run(const A &add, const E &erase, const O &out) { run(add, add, erase, erase, out); }
};