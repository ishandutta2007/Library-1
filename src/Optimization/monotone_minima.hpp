#pragma once
#include <vector>
template <typename F> std::vector<int> monotone_minima(int H, int W, const F &select) {
 std::vector<int> ret(H);
 auto rec= [&](auto &rec, int h1, int h2, int w1, int w2) -> void {
  if (h1 == h2) return;
  int h= (h1 + h2) / 2, best_w= w1;
  for (int w= w1 + 1; w < w2; ++w)
   if (select(h, best_w, w)) best_w= w;
  ret[h]= best_w, rec(rec, h1, h, w1, best_w + 1), rec(rec, h + 1, h2, best_w, w2);
 };
 return rec(rec, 0, H, 0, W), ret;
}