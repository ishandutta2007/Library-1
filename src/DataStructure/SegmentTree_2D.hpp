#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <cassert>
#include "src/Internal/tuple_traits.hpp"
template <class pos_t, class M> class SegmentTree_2D {
public:
 using T= typename M::T;
 using Pos= std::array<pos_t, 2>;
 std::vector<pos_t> xs;
 std::vector<Pos> yxs;
 std::vector<int> id, tol;
 std::vector<T> val;
 template <class P> using canbe_Pos= std::is_convertible<to_tuple_t<P>, std::tuple<pos_t, pos_t>>;
 template <class P> using canbe_PosV= std::is_convertible<to_tuple_t<P>, std::tuple<pos_t, pos_t, T>>;
 template <class P, class U> static constexpr bool canbe_Pos_and_T_v= std::conjunction_v<canbe_Pos<P>, std::is_convertible<U, T>>;
 int sz;
 inline int x2i(pos_t x) const { return std::lower_bound(xs.begin(), xs.end(), x) - xs.begin(); }
 inline int y2i(pos_t y) const {
  return std::lower_bound(yxs.begin(), yxs.end(), Pos{y, 0}, [](const Pos &a, const Pos &b) { return a[0] < b[0]; }) - yxs.begin();
 }
 inline int xy2i(pos_t x, pos_t y) const {
  Pos p{y, x};
  auto it= std::lower_bound(yxs.begin(), yxs.end(), p);
  return assert(p == *it), it - yxs.begin();
 }
 template <bool z, size_t k, class P> inline auto get_(const P &p) {
  if constexpr (z) return std::get<k>(p);
  else return std::get<k>(p.first);
 }
 template <bool z, class XYW> inline void build(const XYW *xyw, int n, const T &v= M::ti()) {
  xs.resize(n), yxs.resize(n);
  for (int i= n; i--;) xs[i]= get_<z, 0>(xyw[i]);
  std::sort(xs.begin(), xs.end()), xs.erase(std::unique(xs.begin(), xs.end()), xs.end()), id.resize((sz= 1 << (32 - __builtin_clz(xs.size()))) * 2 + 1);
  std::vector<int> ix(n), ord(n);
  for (int i= n; i--;) ix[i]= x2i(get_<z, 0>(xyw[i]));
  for (int i: ix)
   for (i+= sz; i; i>>= 1) ++id[i + 1];
  for (int i= 1, e= sz * 2; i < e; ++i) id[i + 1]+= id[i];
  val.assign(id.back() * 2, M::ti()), tol.resize(id[sz] + 1), std::iota(ord.begin(), ord.end(), 0), std::sort(ord.begin(), ord.end(), [&](int i, int j) { return get_<z, 1>(xyw[i]) == get_<z, 1>(xyw[j]) ? get_<z, 0>(xyw[i]) < get_<z, 0>(xyw[j]) : get_<z, 1>(xyw[i]) < get_<z, 1>(xyw[j]); });
  for (int i= n; i--;) yxs[i]= {get_<z, 1>(xyw[ord[i]]), get_<z, 0>(xyw[ord[i]])};
  std::vector<int> ptr= id;
  for (int r: ord)
   for (int i= ix[r] + sz, j= -1; i; j= i, i>>= 1) {
    int p= ptr[i]++;
    if constexpr (z) {
     if constexpr (std::tuple_size_v<XYW> == 3) val[id[i + 1] + p]= std::get<2>(xyw[r]);
     else val[id[i + 1] + p]= v;
    } else val[id[i + 1] + p]= xyw[r].second;
    if (j != -1) tol[p + 1]= !(j & 1);
   }
  for (int i= 1, e= id[sz]; i < e; ++i) tol[i + 1]+= tol[i];
  for (int i= 0, e= sz * 2; i < e; ++i) {
   auto dat= val.begin() + id[i] * 2;
   for (int j= id[i + 1] - id[i]; --j > 0;) dat[j]= M::op(dat[j * 2], dat[j * 2 + 1]);
  }
 }
 inline T fold(int i, int a, int b) const {
  int n= id[i + 1] - id[i];
  T ret= M::ti();
  auto dat= val.begin() + id[i] * 2;
  for (a+= n, b+= n; a < b; a>>= 1, b>>= 1) {
   if (a & 1) ret= M::op(ret, dat[a++]);
   if (b & 1) ret= M::op(dat[--b], ret);
  }
  return ret;
 }
 template <bool z> inline void seti(int i, int j, T v) {
  auto dat= val.begin() + id[i] * 2;
  j+= id[i + 1] - id[i];
  if constexpr (z) dat[j]= v;
  else dat[j]= M::op(dat[j], v);
  for (; j;) j>>= 1, dat[j]= M::op(dat[2 * j], dat[2 * j + 1]);
 }
 template <bool z> inline void set_(pos_t x, pos_t y, T v) {
  for (int i= 1, p= xy2i(x, y);;) {
   if (seti<z>(i, p - id[i], v); i >= sz) break;
   if (int lc= tol[p] - tol[id[i]], rc= (p - id[i]) - lc; tol[p + 1] - tol[p]) p= id[2 * i] + lc, i= 2 * i;
   else p= id[2 * i + 1] + rc, i= 2 * i + 1;
  }
 }
public:
 template <class P, typename= std::enable_if_t<std::disjunction_v<canbe_Pos<P>, canbe_PosV<P>>>> SegmentTree_2D(const P *p, size_t n) { build<1>(p, n); }
 template <class P, typename= std::enable_if_t<std::disjunction_v<canbe_Pos<P>, canbe_PosV<P>>>> SegmentTree_2D(const std::vector<P> &p): SegmentTree_2D(p.data(), p.size()) {}
 template <class P, typename= std::enable_if_t<canbe_Pos<P>::value>> SegmentTree_2D(const std::set<P> &p): SegmentTree_2D(std::vector(p.begin(), p.end())) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> SegmentTree_2D(const P *p, size_t n, const U &v) { build<1>(p, n, v); }
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> SegmentTree_2D(const std::vector<P> &p, const U &v): SegmentTree_2D(p.data(), p.size(), v) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> SegmentTree_2D(const std::set<P> &p, const U &v): SegmentTree_2D(std::vector(p.begin(), p.end()), v) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> SegmentTree_2D(const std::pair<P, U> *p, size_t n) { build<0>(p, n); }
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> SegmentTree_2D(const std::vector<std::pair<P, U>> &p): SegmentTree_2D(p.data(), p.size()) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> SegmentTree_2D(const std::map<P, U> &p): SegmentTree_2D(std::vector(p.begin(), p.end())) {}
 // [l,r) x [u,d)
 T fold(pos_t l, pos_t r, pos_t u, pos_t d) const {
  T ret= M::ti();
  int L= x2i(l), R= x2i(r);
  auto dfs= [&](auto &dfs, int i, int a, int b, int c, int d) -> void {
   if (c == d || R <= a || b <= L) return;
   if (L <= a && b <= R) return ret= M::op(ret, fold(i, c, d)), void();
   int m= (a + b) / 2, ac= tol[id[i] + c] - tol[id[i]], bc= c - ac, ad= tol[id[i] + d] - tol[id[i]], bd= d - ad;
   dfs(dfs, i * 2, a, m, ac, ad), dfs(dfs, i * 2 + 1, m, b, bc, bd);
  };
  return dfs(dfs, 1, 0, sz, y2i(u), y2i(d)), ret;
 }
 void set(pos_t x, pos_t y, T v) { set_<1>(x, y, v); }
 void mul(pos_t x, pos_t y, T v) { set_<0>(x, y, v); }
 T get(pos_t x, pos_t y) const { return val[xy2i(x, y) + id[2]]; }
};