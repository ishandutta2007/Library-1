#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cstddef>
#include <cassert>
#include "src/Internal/HAS_CHECK.hpp"
template <std::uint8_t K, class pos_t, class M> class KDTree {
 HAS_MEMBER(op);
 HAS_MEMBER(ti);
 HAS_MEMBER(mapping);
 HAS_MEMBER(composition);
 HAS_TYPE(T);
 HAS_TYPE(E);
 template <class L> using monoid= std::conjunction<has_T<L>, has_op<L>, has_ti<L>>;
 template <class L> using dual= std::conjunction<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
 template <class T, class F= std::nullptr_t> struct Node_B {
  using E= F;
  T val;
  int ch[2]= {-1, -1};
  pos_t range[K][2], pos[K];
 };
 template <bool sg_, bool du_, typename tEnable= void> struct Node_D: Node_B<M> {};
 template <bool sg_, bool du_> struct Node_D<sg_, du_, typename std::enable_if_t<sg_ && !du_>>: Node_B<typename M::T> { typename M::T sum; };
 template <bool sg_, bool du_> struct Node_D<sg_, du_, typename std::enable_if_t<!sg_ && du_>>: Node_B<typename M::T, typename M::E> {
  typename M::E lazy;
  bool lazy_flg= false;
 };
 template <bool sg_, bool du_> struct Node_D<sg_, du_, typename std::enable_if_t<sg_ && du_>>: Node_B<typename M::T, typename M::E> {
  typename M::T sum;
  typename M::E lazy;
  bool lazy_flg= false;
 };
 using Node= Node_D<monoid<M>::value, dual<M>::value>;
 using T= decltype(Node::val);
 using E= typename Node::E;
 std::vector<Node> ns;
public:
 using PosVal= std::pair<std::array<pos_t, K>, T>;
 using Iter= typename std::vector<PosVal>::iterator;
 using Range= std::array<std::array<pos_t, 2>, K>;
private:
 inline void pushup(int i) {
  ns[i].sum= ns[i].val;
  if (ns[i].ch[0] != -1) ns[i].sum= M::op(ns[ns[i].ch[0]].sum, ns[i].sum);
  if (ns[i].ch[1] != -1) ns[i].sum= M::op(ns[i].sum, ns[ns[i].ch[1]].sum);
 }
 inline void propagate(int i, const E &x) {
  if (i == -1) return;
  ns[i].lazy_flg ? (M::composition(ns[i].lazy, x), x) : (ns[i].lazy= x);
  M::mapping(ns[i].val, x), ns[i].lazy_flg= true;
  if constexpr (monoid<M>::value) M::mapping(ns[i].sum, x);
 }
 inline void eval(int i) {
  if (!ns[i].lazy_flg) return;
  ns[i].lazy_flg= false;
  propagate(ns[i].ch[0], ns[i].lazy), propagate(ns[i].ch[1], ns[i].lazy);
 }
 inline void build(int &i, Iter bg, Iter ed, int &ts, std::uint8_t div= 0) {
  if (ed - bg < 1) return;
  const int n= ed - bg;
  auto md= bg + n / 2;
  std::nth_element(bg, md, ed, [div](const PosVal &l, const PosVal &r) { return l.first[div] < r.first[div]; });
  ns[i= ts++].val= md->second;
  for (std::uint8_t j= K; j--; ns[i].pos[j]= md->first[j]) {
   auto [mn, mx]= std::minmax_element(bg, ed, [j](const PosVal &l, const PosVal &r) { return l.first[j] < r.first[j]; });
   ns[i].range[j][0]= mn->first[j], ns[i].range[j][1]= mx->first[j];
  }
  if (std::uint8_t nex= (div + 1) % K; n > 1) build(ns[i].ch[0], bg, md, ts, nex), build(ns[i].ch[1], md + 1, ed, ts, nex);
  if constexpr (monoid<M>::value) pushup(i);
 }
 template <class F, class G, class H> inline T fold(int i, const F &in, const G &inall, const H &outall) {
  static_assert(monoid<M>::value, "\"fold\" is not available");
  if (i == -1 || outall(ns[i].range)) return M::ti();
  if (inall(ns[i].range)) return ns[i].sum;
  if constexpr (dual<M>::value) eval(i);
  T ret= M::op(fold(ns[i].ch[0], in, inall, outall), fold(ns[i].ch[1], in, inall, outall));
  ret= in(ns[i].pos) ? M::op(ret, ns[i].val) : ret;
  return ret;
 }
 template <class F, class G, class H> inline void apply(int i, const F &in, const G &inall, const H &outall, const E &x) {
  static_assert(dual<M>::value, "\"apply\" is not available");
  if (i == -1 || outall(ns[i].range)) return;
  if (inall(ns[i].range)) return propagate(i, x), void();
  if (eval(i); in(ns[i].pos)) M::mapping(ns[i].val, x);
  apply(ns[i].ch[0], in, inall, outall, x), apply(ns[i].ch[1], in, inall, outall, x);
  if constexpr (monoid<M>::value) pushup(i);
 }
 inline std::pair<T, bool> get(int i, const std::array<pos_t, K> &pos) {
  if (i == -1) return {T(), false};
  bool myself= true;
  for (std::uint8_t j= K; j--; myself&= pos[j] == ns[i].pos[j])
   if (ns[i].range[j][1] < pos[j] || pos[j] < ns[i].range[j][0]) return {T(), false};
  if (myself) return {ns[i].val, true};
  if constexpr (dual<M>::value) eval(i);
  auto ret= get(ns[i].ch[0], pos);
  return !ret.second ? get(ns[i].ch[1], pos) : ret;
 }
 inline bool set(int i, const std::array<pos_t, K> &pos, const T &x) {
  if (i == -1) return false;
  bool myself= true, ret= true;
  for (std::uint8_t j= K; j--; myself&= pos[j] == ns[i].pos[j])
   if (ns[i].range[j][1] < pos[j] || pos[j] < ns[i].range[j][0]) return false;
  if constexpr (dual<M>::value) eval(i);
  if (myself) ns[i].val= x;
  else if (!(ret= set(ns[i].ch[0], pos, x))) ret= set(ns[i].ch[1], pos, x);
  if constexpr (monoid<M>::value) pushup(i);
  return ret;
 }
 template <typename... Args> static inline Range to_range(std::initializer_list<Args>... intervals) {
  static_assert(sizeof...(intervals) == K);
  static_assert(std::conjunction_v<std::is_same<Args, pos_t>...>);
  Range r;
  std::uint8_t i= 0;
  for (auto &&x: {intervals...}) {
   std::vector<pos_t> tmp(x);
   assert(tmp.size() == 2), assert(tmp[0] <= tmp[1]);
   r[i][0]= tmp[0], r[i][1]= tmp[1], i++;
  }
  return r;
 }
 static inline auto funcs(const Range &r) {
  return std::make_tuple(
      [r](pos_t pos[K]) {
       for (std::uint8_t i= K; i--;)
        if (pos[i] < r[i][0] || r[i][1] < pos[i]) return false;
       return true;
      },
      [r](pos_t range[K][2]) {
       for (std::uint8_t i= K; i--;)
        if (range[i][0] < r[i][0] || r[i][1] < range[i][1]) return false;
       return true;
      },
      [r](pos_t range[K][2]) {
       for (std::uint8_t i= K; i--;)
        if (range[i][1] < r[i][0] || r[i][1] < range[i][0]) return true;
       return false;
      });
 }
public:
 KDTree(std::vector<PosVal> v): ns(v.size()) {
  int root, timestamp= 0;
  build(root, v.begin(), v.end(), timestamp);
 }
 T get(std::array<pos_t, K> pos) {
  auto [ret, flg]= get(0, pos);
  return assert(flg), ret;
 }
 template <typename... Args> T get(Args... ids) {
  static_assert(sizeof...(ids) == K);
  static_assert(std::conjunction_v<std::is_convertible<Args, pos_t>...>);
  auto [ret, flg]= get(0, {ids...});
  return assert(flg), ret;
 }
 void set(T x, std::array<pos_t, K> pos) { assert(set(0, pos, x)); }
 template <typename... Args> void set(T x, Args... ids) {
  static_assert(sizeof...(ids) == K);
  static_assert(std::conjunction_v<std::is_convertible<Args, pos_t>...>);
  assert(set(0, {ids...}, x));
 }
 T fold(const Range &r) {
  auto [in, inall, outall]= funcs(r);
  return fold(0, in, inall, outall);
 }
 template <typename... Args> T fold(std::initializer_list<Args> &&...intervals) {
  auto r= to_range(intervals...);
  auto [in, inall, outall]= funcs(r);
  return fold(0, in, inall, outall);
 }
 template <class F, class G, class H> T fold(const F &in, const G &inall, const H &outall) { return fold(0, in, inall, outall); }
 void apply(E x, const Range &r) {
  auto [in, inall, outall]= funcs(r);
  apply(0, in, inall, outall, x);
 }
 template <typename... Args> void apply(E x, std::initializer_list<Args> &&...intervals) {
  auto r= to_range(intervals...);
  auto [in, inall, outall]= funcs(r);
  apply(0, in, inall, outall, x);
 }
 template <class F, class G, class H> void apply(E x, const F &in, const G &inall, const H &outall) { apply(0, in, inall, outall, x); }
};