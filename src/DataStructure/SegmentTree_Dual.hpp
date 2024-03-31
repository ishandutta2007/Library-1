#pragma once
#include <vector>
template <typename M> struct SegmentTree_Dual {
 using T= typename M::T;
 using E= typename M::E;
 SegmentTree_Dual() {}
 SegmentTree_Dual(int n_, T v1= T()): n(n_), height(n == 1 ? 0 : std::__lg(n - 1) + 1), val(n, v1), laz(n * 2, {E(), false}) {}
 SegmentTree_Dual(const std::vector<T> &v): n(v.size()), height(n == 1 ? 1 : std::__lg(n - 1) + 1), val(v), laz(n * 2, {E(), false}) {}
 void apply(int a, int b, E x) {
  a+= n, b+= n;
  for (int i= height; i >= 1; i--)
   if (((a >> i) << i) != a) eval(a >> i);
  for (int i= height; i >= 1; i--)
   if (((b >> i) << i) != b) eval((b - 1) >> i);
  for (int l= a, r= b; l < r; l>>= 1, r>>= 1) {
   if (l & 1) propagate(l++, x);
   if (r & 1) propagate(--r, x);
  }
 }
 void set(int k, T x) {
  for (int i= height; i; i--) eval((k + n) >> i);
  val[k]= x, laz[k + n].flg= false;
 }
 T &operator[](const int k) {
  for (int i= height; i; i--) eval((k + n) >> i);
  if (laz[k + n].flg) M::mp(val[k], laz[k + n].val), laz[k + n].flg= false;
  return val[k];
 }
private:
 const int n, height;
 struct Lazy {
  E val;
  bool flg;
 };
 std::vector<T> val;
 std::vector<Lazy> laz;
 inline void eval(int k) {
  if (!laz[k].flg) return;
  propagate(k << 1 | 0, laz[k].val), propagate(k << 1 | 1, laz[k].val);
  laz[k].flg= false;
 }
 inline void propagate(int k, const E &x) {
  laz[k].flg ? (M::cp(laz[k].val, x), x) : laz[k].val= x;
  laz[k].flg= true;
 }
};