#pragma once
#include <memory>
#include <cassert>
#include <vector>
#include <algorithm>
#include "src/Internal/detection_idiom.hpp"
template <class M> class SegmentTree {
 _DETECT_BOOL(monoid, typename M::T, decltype(&M::op), decltype(&M::ti));
 _DETECT_BOOL(dual, typename T::T, typename T::E, decltype(&T::mp), decltype(&T::cp));
 _DETECT_TYPE(nullptr_or_E, typename T::E, std::nullptr_t, typename T::E);
 using T= typename M::T;
 using E= typename nullptr_or_E<M>::type;
 int n;
 std::unique_ptr<T[]> dat;
 std::unique_ptr<E[]> laz;
 std::unique_ptr<bool[]> flg;
 inline void update(int k) { dat[k]= M::op(dat[k << 1], dat[k << 1 | 1]); }
 inline bool map(int k, E x, int sz) {
  if constexpr (std::is_invocable_r_v<bool, decltype(M::mp), T &, E, int>) return M::mp(dat[k], x, sz);
  else if constexpr (std::is_invocable_r_v<bool, decltype(M::mp), T &, E>) return M::mp(dat[k], x);
  else if constexpr (std::is_invocable_r_v<void, decltype(M::mp), T &, E, int>) return M::mp(dat[k], x, sz), true;
  else return M::mp(dat[k], x), true;
 }
 inline void prop(int k, E x, int sz) {
  if (k < n) {
   if (flg[k]) M::cp(laz[k], x);
   else laz[k]= x;
   flg[k]= true;
   if constexpr (monoid_v<M>)
    if (!map(k, x, sz)) push(k, sz), update(k);
  } else {
   if constexpr (monoid_v<M>) map(k, x, 1);
   else map(k - n, x, 1);
  }
 }
 inline void push(int k, int sz) {
  if (flg[k]) prop(k << 1, laz[k], sz >> 1), prop(k << 1 | 1, laz[k], sz >> 1), flg[k]= false;
 }
 inline bool valid(int k) const {
  int d= __builtin_clz(k) - __builtin_clz(n);
  return (n >> d) != k || ((n >> d) << d) == n;
 }
public:
 SegmentTree() {}
 SegmentTree(int n): n(n), dat(std::make_unique<T[]>(n << monoid_v<M>)) {
  if constexpr (monoid_v<M>) std::fill_n(dat.get(), n << 1, M::ti());
  if constexpr (dual_v<M>) laz= std::make_unique<E[]>(n), flg= std::make_unique<bool[]>(n), std::fill_n(flg.get(), n, false);
 }
 template <class F> SegmentTree(int n, const F &init): n(n), dat(std::make_unique<T[]>(n << monoid_v<M>)) {
  auto a= dat.get() + (n & -monoid_v<M>);
  for (int i= 0; i < n; ++i) a[i]= init(i);
  if constexpr (monoid_v<M>) build();
  if constexpr (dual_v<M>) laz= std::make_unique<E[]>(n), flg= std::make_unique<bool[]>(n), std::fill_n(flg.get(), n, false);
 }
 SegmentTree(int n, T x): SegmentTree(n, [x](int) { return x; }) {}
 SegmentTree(const std::vector<T> &v): SegmentTree(v.size(), [&v](int i) { return v[i]; }) {}
 SegmentTree(const T *bg, const T *ed): SegmentTree(ed - bg, [bg](int i) { return bg[i]; }) {}
 void build() {
  static_assert(monoid_v<M>, "\"build\" is not available\n");
  for (int i= n; --i;) update(i);
 }
 inline void unsafe_set(int i, T x) {
  static_assert(monoid_v<M>, "\"unsafe_set\" is not available\n");
  dat[i + n]= x;
 }
 inline void set(int i, T x) {
  get(i);
  if constexpr (monoid_v<M>)
   for (dat[i+= n]= x; i>>= 1;) update(i);
  else dat[i]= x;
 }
 inline void mul(int i, T x) {
  static_assert(monoid_v<M>, "\"mul\" is not available\n");
  set(i, M::op(get(i), x));
 }
 inline T get(int i) {
  i+= n;
  if constexpr (dual_v<M>)
   for (int j= 31 - __builtin_clz(i); j; --j) push(i >> j, 1 << j);
  if constexpr (monoid_v<M>) return dat[i];
  else return dat[i - n];
 }
 inline T operator[](int i) { return get(i); }
 inline T prod(int l, int r) {
  static_assert(monoid_v<M>, "\"prod\" is not available\n");
  l+= n, r+= n;
  if constexpr (dual_v<M>) {
   for (int j= 31 - __builtin_clz(l); ((l >> j) << j) != l; --j) push(l >> j, 1 << j);
   for (int j= 31 - __builtin_clz(r); ((r >> j) << j) != r; --j) push(r >> j, 1 << j);
  }
  T s1= M::ti(), s2= M::ti();
  for (; l < r; l>>= 1, r>>= 1) {
   if (l & 1) s1= M::op(s1, dat[l++]);
   if (r & 1) s2= M::op(dat[--r], s2);
  }
  return M::op(s1, s2);
 }
 inline void apply(int l, int r, E x) {
  static_assert(dual_v<M>, "\"apply\" is not available\n");
  l+= n, r+= n;
  for (int j= 31 - __builtin_clz(l); ((l >> j) << j) != l; j--) push(l >> j, 1 << j);
  for (int j= 31 - __builtin_clz(r); ((r >> j) << j) != r; j--) push(r >> j, 1 << j);
  for (int a= l, b= r, sz= 1; a < b; a>>= 1, b>>= 1, sz<<= 1) {
   if (a & 1) prop(a++, x, sz);
   if (b & 1) prop(--b, x, sz);
  }
  if constexpr (monoid_v<M>) {
   for (int j= __builtin_ctz(l) + 1; l >> j; ++j) update(l >> j);
   for (int j= __builtin_ctz(r) + 1; r >> j; ++j) update(r >> j);
  }
 }
 template <class C> int max_right(int l, const C &check) {
  static_assert(monoid_v<M>, "\"max_right\" is not available\n");
  assert(check(M::ti()));
  if (check(prod(l, n))) return n;
  T s= M::ti(), t;
  int sz= 1;
  for (get(l), l+= n;; s= t, ++l) {
   while (!(l & 1) && valid(l >> 1)) l>>= 1, sz<<= 1;
   if (!check(t= M::op(s, dat[l]))) {
    while (l < n) {
     if constexpr (dual_v<M>) push(l, sz);
     l<<= 1, sz>>= 1;
     if (check(t= M::op(s, dat[l]))) s= t, ++l;
    }
    return l - n;
   }
  }
 }
 template <class C> int min_left(int r, const C &check) {
  static_assert(monoid_v<M>, "\"min_left\" is not available\n");
  assert(check(M::ti()));
  if (check(prod(0, r))) return 0;
  T s= M::ti(), t;
  int sz= 1;
  for (get(--r), r+= n;; s= t, --r) {
   while (!valid(r)) r= r << 1 | 1, sz>>= 1;
   while ((r & 1) && valid(r >> 1)) r>>= 1, sz<<= 1;
   if (!check(t= M::op(dat[r], s))) {
    while (r < n) {
     if constexpr (dual_v<M>) push(r, sz);
     r= r << 1 | 1, sz>>= 1;
     if (check(t= M::op(dat[r], s))) s= t, --r;
    }
    return r + 1 - n;
   }
  }
 }
};