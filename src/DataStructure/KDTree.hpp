#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include <map>
#include <set>
#include <cassert>
#include "src/Internal/HAS_CHECK.hpp"
#include "src/Internal/tuple_traits.hpp"
#include "src/Internal/long_traits.hpp"
namespace kdtree_internal {
template <class pos_t, size_t K, class M, class A, class B> class KDTreeImpl {};
template <class pos_t, size_t K, class M, class... PK, class... PK2> class KDTreeImpl<pos_t, K, M, std::tuple<PK...>, std::tuple<PK2...>> {
 HAS_MEMBER(op);
 HAS_MEMBER(ti);
 HAS_MEMBER(mp);
 HAS_MEMBER(cp);
 HAS_TYPE(T);
 HAS_TYPE(E);
 MYSELF_OR(T);
 NULLPTR_OR(E);
 using Sec= std::array<pos_t, 2>;
 using Pos= std::array<pos_t, K>;
 using Range= std::array<Sec, K>;
 using long_pos_t= make_long_t<pos_t>;
 template <class L> static constexpr bool monoid_v= std::conjunction_v<has_T<L>, has_op<L>, has_ti<L>>;
 template <class L> static constexpr bool dual_v= std::conjunction_v<has_T<L>, has_E<L>, has_mp<L>, has_cp<L>>;
 struct Node_BB {
  int ch[2]= {-1, -1};
  Pos pos;
  pos_t range[K][2];
 };
 template <class U> struct Node_B: Node_BB {
  U val;
 };
 template <class D, bool sg, bool du> struct Node_D: Node_B<M> {};
 template <bool sg, bool du> struct Node_D<void, sg, du>: Node_BB {};
 template <class D> struct Node_D<D, 1, 0>: Node_B<typename M::T> {
  typename M::T sum;
 };
 template <class D> struct Node_D<D, 0, 1>: Node_B<typename M::T> {
  typename M::E laz;
  bool laz_flg= false;
 };
 template <class D> struct Node_D<D, 1, 1>: Node_B<typename M::T> {
  typename M::T sum;
  typename M::E laz;
  bool laz_flg= false;
 };
 using Node= Node_D<M, monoid_v<M>, dual_v<M>>;
 using Iter= typename std::vector<int>::iterator;
 using T= std::conditional_t<std::is_void_v<M>, std::nullptr_t, myself_or_T_t<M>>;
 using E= nullptr_or_E_t<M>;
 template <class P> using canbe_Pos= std::is_convertible<to_tuple_t<P>, std::tuple<PK...>>;
 template <class P> using canbe_PosV= std::is_convertible<to_tuple_t<P>, std::tuple<PK..., T>>;
 template <class P, class U> static constexpr bool canbe_Pos_and_T_v= std::conjunction_v<canbe_Pos<P>, std::is_convertible<U, T>>;
 std::vector<Node> ns;
 static inline T def_val() {
  if constexpr (monoid_v<M>) return M::ti();
  else return T();
 }
 template <bool z, size_t k, class P> static inline auto get_(const P &p) {
  if constexpr (z) return std::get<k>(p);
  else return std::get<k>(p.first);
 }
 template <class P, size_t... I> Range to_range(const P &p, std::index_sequence<I...>) { return {(assert(std::get<I + I>(p) <= std::get<I + I + 1>(p)), Sec{std::get<I + I>(p), std::get<I + I + 1>(p)})...}; }
 inline void update(int t) {
  ns[t].sum= ns[t].val;
  if (ns[t].ch[0] != -1) ns[t].sum= M::op(ns[t].sum, ns[ns[t].ch[0]].sum);
  if (ns[t].ch[1] != -1) ns[t].sum= M::op(ns[t].sum, ns[ns[t].ch[1]].sum);
 }
 inline void propagate(int t, const E &x) {
  if (t == -1) return;
  if (ns[t].laz_flg) M::cp(ns[t].laz, x);
  else ns[t].laz= x, ns[t].laz_flg= true;
  M::mp(ns[t].val, x);
  if constexpr (monoid_v<M>) M::mp(ns[t].sum, x);
 }
 inline void push(int t) {
  if (ns[t].laz_flg) ns[t].laz_flg= false, propagate(ns[t].ch[0], ns[t].laz), propagate(ns[t].ch[1], ns[t].laz);
 }
 template <bool z, class P, size_t k> inline void set_range(int t, int m, Iter bg, Iter ed, const P *p) {
  auto [mn, mx]= std::minmax_element(bg, ed, [&](int a, int b) { return get_<z, k>(p[a]) < get_<z, k>(p[b]); });
  ns[t].range[k][0]= get_<z, k>(p[*mn]), ns[t].range[k][1]= get_<z, k>(p[*mx]), ns[t].pos[k]= get_<z, k>(p[m]);
 }
 template <bool z, class P, size_t... I> inline void set_range_lp(int t, int m, Iter bg, Iter ed, const P *p, std::index_sequence<I...>) { (void)(int[]){(set_range<z, P, I>(t, m, bg, ed, p), 0)...}; }
 template <bool z, uint8_t div, class P> inline int build(int &ts, Iter bg, Iter ed, const P *p, const T &v= def_val()) {
  if (bg == ed) return -1;
  auto md= bg + (ed - bg) / 2;
  int t= ts++;
  std::nth_element(bg, md, ed, [&](int a, int b) { return get_<z, div>(p[a]) < get_<z, div>(p[b]); }), set_range_lp<z>(t, *md, bg, ed, p, std::make_index_sequence<K>());
  if constexpr (z) {
   if constexpr (!std::is_void_v<M>) {
    if constexpr (std::tuple_size_v<P> == K + 1) ns[t].val= std::get<K>(p[*md]);
    else ns[t].val= v;
   }
  } else ns[t].val= p[*md].second;
  static constexpr uint8_t nx= div + 1 == K ? 0 : div + 1;
  ns[t].ch[0]= build<z, nx>(ts, bg, md, p, v), ns[t].ch[1]= build<z, nx>(ts, md + 1, ed, p, v);
  if constexpr (monoid_v<M>) update(t);
  return t;
 }
 template <bool z, uint8_t div, class P> inline int build(Iter bg, Iter ed, const P *p, int &ts) {
  if (bg == ed) return -1;
  auto md= bg + (ed - bg) / 2;
  int t= ts++;
  std::nth_element(bg, md, ed, [&](int a, int b) { return get_<z, div>(p[a]) < get_<z, div>(p[b]); }), set_range_lp<z>(t, bg, ed, p, std::make_index_sequence<K>());
  if constexpr (z) {
   if constexpr (!std::is_void_v<M>) {
    if constexpr (std::tuple_size_v<P> == K + 1) ns[t].val= std::get<K>(p[t]);
    else ns[t].val= def_val();
   }
  } else ns[t].val= p[t].second;
  static constexpr uint8_t nx= div + 1 == K ? 0 : div + 1;
  ns[t].ch[0]= build<z, nx>(bg, md, p, ts), ns[t].ch[1]= build<z, nx>(md + 1, ed, p, ts);
  if constexpr (monoid_v<M>) update(t);
  return t;
 }
 static inline auto in_cuboid(const Range &r) {
  return [r](const Pos &pos) {
   for (uint8_t k= K; k--;)
    if (r[k][1] < pos[k] || pos[k] < r[k][0]) return false;
   return true;
  };
 }
 static inline auto out_cuboid(const Range &r) {
  return [r](const pos_t rr[K][2]) {
   for (uint8_t k= K; k--;)
    if (rr[k][1] < r[k][0] || r[k][1] < rr[k][0]) return true;
   return false;
  };
 }
 static inline auto inall_cuboid(const Range &r) {
  return [r](const pos_t rr[K][2]) {
   for (uint8_t k= K; k--;)
    if (rr[k][0] < r[k][0] || r[k][1] < rr[k][1]) return false;
   return true;
  };
 }
 static inline long_pos_t min_dist2(const pos_t r[K][2], const Pos &pos) {
  long_pos_t d2= 0, dx;
  for (uint8_t k= K; k--;) dx= std::clamp(pos[k], r[k][0], r[k][1]) - pos[k], d2+= dx * dx;
  return d2;
 }
 static inline auto in_ball(const Pos &c, long_pos_t r2) {
  return [c, r2](const Pos &pos) {
   long_pos_t d2= 0, dx;
   for (uint8_t k= K; k--;) dx= pos[k] - c[k], d2+= dx * dx;
   return d2 <= r2;
  };
 }
 static inline auto inall_ball(const Pos &c, long_pos_t r2) {
  return [c, r2](const pos_t rr[K][2]) {
   long_pos_t d2= 0, dx0, dx1;
   for (uint8_t k= K; k--;) dx0= rr[k][0] - c[k], dx1= rr[k][1] - c[k], d2+= std::max(dx0 * dx0, dx1 * dx1);
   return d2 <= r2;
  };
 }
 static inline auto out_ball(const Pos &c, long_pos_t r2) {
  return [c, r2](const pos_t r[K][2]) { return min_dist2(r, c) > r2; };
 }
 inline void nns(int t, const Pos &pos, std::pair<int, long_pos_t> &ret) const {
  if (t == -1) return;
  long_pos_t d2= min_dist2(ns[t].range, pos);
  if (ret.first != -1 && d2 >= ret.second) return;
  long_pos_t dx= d2= 0;
  for (uint8_t k= K; k--;) dx= pos[k] - ns[t].pos[k], d2+= dx * dx;
  if (ret.first == -1 || d2 < ret.second) ret= {t, d2};
  bool f= 0;
  if (auto [l, r]= ns[t].ch; l != -1 && r != -1) f= min_dist2(ns[l].range, pos) > min_dist2(ns[r].range, pos);
  nns(ns[t].ch[f], pos, ret), nns(ns[t].ch[!f], pos, ret);
 }
 template <class In, class Out> inline void col(int t, const In &in, const Out &out, std::vector<T> &ret) const {
  if (t == -1 || out(ns[t].range)) return;
  if (in(ns[t].pos)) ret.push_back(ns[t].val);
  col(ns[t].ch[0], in, out, ret), col(ns[t].ch[1], in, out, ret);
 }
 template <class In, class InAll, class Out> inline T fld(int t, const In &in, const InAll &inall, const Out &out) {
  if (t == -1 || out(ns[t].range)) return def_val();
  if (inall(ns[t].range)) return ns[t].sum;
  if constexpr (dual_v<M>) push(t);
  T ret= M::op(fld(ns[t].ch[0], in, inall, out), fld(ns[t].ch[1], in, inall, out));
  return in(ns[t].pos) ? M::op(ret, ns[t].val) : ret;
 }
 template <class In, class InAll, class Out> inline void app(int t, const In &in, const InAll &inall, const Out &out, const E &x) {
  if (t == -1 || out(ns[t].range)) return;
  if (inall(ns[t].range)) return propagate(t, x);
  if (push(t); in(ns[t].pos)) M::mp(ns[t].val, x);
  app(ns[t].ch[0], in, inall, out, x), app(ns[t].ch[1], in, inall, out, x);
  if constexpr (monoid_v<M>) update(t);
 }
 template <bool z> inline bool set(int t, const Pos &pos, const T &x) {
  if (t == -1) return false;
  bool isok= true;
  for (uint8_t k= K; k--; isok&= pos[k] == ns[t].pos[k])
   if (ns[t].range[k][1] < pos[k] || pos[k] < ns[t].range[k][0]) return false;
  if constexpr (dual_v<M>) push(t);
  if (isok) {
   if constexpr (z) ns[t].val= x;
   else ns[t].val= M::op(ns[t].val, x);
  } else if (!(isok= set<z>(ns[t].ch[0], pos, x))) isok= set<z>(ns[t].ch[1], pos, x);
  if constexpr (monoid_v<M>)
   if (isok) update(t);
  return isok;
 }
 inline std::pair<T, bool> get(int t, const Pos &pos) {
  if (t == -1) return {T(), false};
  bool myself= true;
  for (uint8_t k= K; k--; myself&= pos[k] == ns[t].pos[k])
   if (ns[t].range[k][1] < pos[k] || pos[k] < ns[t].range[k][0]) return {T(), false};
  if (myself) return {ns[t].val, true};
  if constexpr (dual_v<M>) push(t);
  auto ret= get(ns[t].ch[0], pos);
  return !ret.second ? get(ns[t].ch[1], pos) : ret;
 }
public:
 template <class P, typename= std::enable_if_t<std::disjunction_v<canbe_Pos<P>, canbe_PosV<P>>>> KDTreeImpl(const P *p, size_t n): ns(n) {
  std::vector<int> ids(n);
  int ts= 0;
  std::iota(ids.begin(), ids.end(), 0), build<1, 0>(ts, ids.begin(), ids.end(), p);
 }
 template <class P, typename= std::enable_if_t<std::disjunction_v<canbe_Pos<P>, canbe_PosV<P>>>> KDTreeImpl(const std::vector<P> &p): KDTreeImpl(p.data(), p.size()) {}
 template <class P, typename= std::enable_if_t<canbe_Pos<P>::value>> KDTreeImpl(const std::set<P> &p): KDTreeImpl(std::vector(p.begin(), p.end())) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> KDTreeImpl(const P *p, size_t n, U v): ns(n) {
  std::vector<int> ids(n);
  int ts= 0;
  std::iota(ids.begin(), ids.end(), 0), build<1, 0>(ts, ids.begin(), ids.end(), p, v);
 }
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> KDTreeImpl(const std::vector<P> &p, U v): KDTreeImpl(p.data(), p.size(), v) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> KDTreeImpl(const std::set<P> &p, U v): KDTreeImpl(std::vector(p.begin(), p.end()), v) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> KDTreeImpl(const std::pair<P, U> *p, size_t n): ns(n) {
  std::vector<int> ids(n);
  int ts= 0;
  std::iota(ids.begin(), ids.end(), 0), build<0, 0>(ts, ids.begin(), ids.end(), p);
 }
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> KDTreeImpl(const std::vector<std::pair<P, U>> &p): KDTreeImpl(p.data(), p.size()) {}
 template <class P, class U, typename= std::enable_if_t<canbe_Pos_and_T_v<P, U>>> KDTreeImpl(const std::map<P, U> &p): KDTreeImpl(std::vector(p.begin(), p.end())) {}
 std::vector<T> enum_cuboid(PK2... xs) {
  static_assert(!std::is_void_v<M>, "\"enum_cuboid\" is not available");
  std::vector<T> ret;
  auto r= to_range(std::forward_as_tuple(xs...), std::make_index_sequence<K>());
  return col(-ns.empty(), in_cuboid(r), out_cuboid(r), ret), ret;
 }
 std::vector<T> enum_ball(PK... xs, pos_t r) const {
  static_assert(!std::is_void_v<M>, "\"enum_ball\" is not available");
  std::vector<T> ret;
  long_pos_t r2= long_pos_t(r) * r;
  return col(-ns.empty(), in_ball({xs...}, r2), out_ball({xs...}, r2), ret), ret;
 }
 T prod_cuboid(PK2... xs) {
  static_assert(monoid_v<M>, "\"prod_cuboid\" is not available");
  auto r= to_range(std::forward_as_tuple(xs...), std::make_index_sequence<K>());
  return fld(-ns.empty(), in_cuboid(r), inall_cuboid(r), out_cuboid(r));
 }
 T prod_ball(PK... xs, pos_t r) {
  static_assert(monoid_v<M>, "\"prod_ball\" is not available");
  long_pos_t r2= long_pos_t(r) * r;
  return fld(-ns.empty(), in_ball({xs...}, r2), inall_ball({xs...}, r2), out_ball({xs...}, r2));
 }
 void apply_cuboid(PK2... xs, E a) {
  static_assert(dual_v<M>, "\"apply_cuboid\" is not available");
  auto r= to_range(std::forward_as_tuple(xs...), std::make_index_sequence<K>());
  app(-ns.empty(), in_cuboid(r), inall_cuboid(r), out_cuboid(r), a);
 }
 void apply_ball(PK... xs, pos_t r, E a) {
  static_assert(dual_v<M>, "\"apply_ball\" is not available");
  long_pos_t r2= long_pos_t(r) * r;
  app(-ns.empty(), in_ball({xs...}, r2), inall_ball({xs...}, r2), out({xs...}, r2), a);
 }
 void set(PK... xs, T v) { assert(ns.size()), assert(set<1>(0, {xs...}, v)); }
 void mul(PK... xs, T v) {
  static_assert(monoid_v<M>, "\"mul\" is not available");
  assert(ns.size()), assert(set<0>(0, {xs...}, v));
 }
 T get(PK... xs) {
  assert(ns.size());
  auto [ret, flg]= get(0, {xs...});
  return assert(flg), ret;
 }
 Pos nearest_neighbor(PK... xs) const {
  assert(ns.size());
  std::pair<int, long_pos_t> ret= {-1, -1};
  return nns(0, {xs...}, ret), ns[ret.first].pos;
 }
};
template <class pos_t, size_t K, class M= void> using KDTree= KDTreeImpl<pos_t, K, M, to_tuple_t<std::array<pos_t, K>>, to_tuple_t<std::array<pos_t, K + K>>>;
}
using kdtree_internal::KDTree;