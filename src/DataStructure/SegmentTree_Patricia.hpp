#pragma once
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <cstddef>
#include <cstdint>
#include "src/Internal/HAS_CHECK.hpp"
template <typename M, bool persistent= false, uint8_t HEIGHT= 31> class SegmentTree_Patricia {
 HAS_MEMBER(op);
 HAS_MEMBER(ti);
 HAS_TYPE(T);
 template <class L> static constexpr bool monoid_v= std::conjunction_v<has_T<L>, has_op<L>, has_ti<L>>;
 using id_t= long long;
 template <class T, class tDerived> struct Node_B {
  id_t bits;
  uint8_t len;
  T val;
  tDerived *ch[2]= {nullptr, nullptr};
 };
 template <bool mo, typename tEnable= void> struct Node_D: Node_B<M, Node_D<mo, tEnable>> {};
 template <bool mo> struct Node_D<mo, typename std::enable_if_t<mo>>: Node_B<typename M::T, Node_D<mo>> {};
 using Node= Node_D<monoid_v<M>>;
 using T= decltype(Node::val);
 using np= Node *;
 np root;
 static inline constexpr T def_val() {
  if constexpr (monoid_v<M>) return M::ti();
  else return T();
 }
 template <class S> np build(const id_t &n, id_t l, id_t r, const S &bg) {
  if (n <= l) return nullptr;
  id_t m= (l + r) / 2;
  while (n <= m) r= m, m= (l + r) / 2;
  if (r - l == 1) {
   if constexpr (std::is_same_v<S, T>) return new Node{l, HEIGHT + 1, bg};
   else return new Node{l, HEIGHT + 1, *(bg + l)};
  }
  uint8_t h= __builtin_ctzll(r - l);
  np t= new Node{m >> h, uint8_t(HEIGHT + 1 - h), def_val(), {build(n, l, m, bg), build(n, m, r, bg)}};
  if constexpr (monoid_v<M>) t->val= M::op(t->ch[0]->val, t->ch[1]->val);
  return t;
 }
 void dump(np t, const id_t &l, const id_t &r, std::array<id_t, 2> b, typename std::vector<T>::iterator itr, uint8_t h) {
  if (r <= b[0] || b[1] <= l) return;
  if (l <= b[0] && b[1] <= r && !t) {
   for (id_t i= b[0]; i < b[1]; i++) *(itr + i)= def_val();
  } else if (b[1] - b[0] != 1) {
   auto m= (b[0] + b[1]) >> 1;
   dump(next(t, h, 0), l, r, {b[0], m}, itr, h - 1);
   dump(next(t, h, 1), l, r, {m, b[1]}, itr, h - 1);
  } else *(itr + b[0])= t->val;
 }
 T fold(np &t, const id_t &l, const id_t &r, const id_t &bias) {
  static id_t bits, b[2];
  if (!t) return def_val();
  uint8_t h= (HEIGHT + 1) - t->len;
  bits= (bias >> h) ^ t->bits, b[0]= bits << h, b[1]= (bits + 1) << h;
  if (r <= b[0] || b[1] <= l) return def_val();
  if (l <= b[0] && b[1] <= r) return t->val;
  bool flg= (bias >> (h - 1)) & 1;
  return M::op(fold(t->ch[flg], l, r, bias), fold(t->ch[!flg], l, r, bias));
 }
 void set_val(np &t, const id_t &k, const T &val) {
  if (!t) return t= new Node{k, HEIGHT + 1, val}, void();
  if constexpr (persistent) t= new Node{*t};
  id_t bits= (k >> ((HEIGHT + 1) - t->len));
  if (bits != t->bits) {
   uint8_t i= 64 - __builtin_clzll(bits ^ t->bits);
   bool flg= (t->bits >> (i - 1)) & 1;
   t->ch[flg]= new Node{*t}, t->ch[!flg]= new Node{k, HEIGHT + 1, val};
   t->len-= i, t->bits>>= i;
  } else if (t->len != HEIGHT + 1) {
   set_val(t->ch[(k >> (HEIGHT - t->len)) & 1], k, val);
  } else return t->val= val, void();
  if constexpr (monoid_v<M>) t->val= M::op(t->ch[0]->val, t->ch[1]->val);
 }
 T &at_val(np &t, const id_t &k) {
  if (!t) return t= new Node{k, HEIGHT + 1, def_val()}, t->val;
  if constexpr (persistent) t= new Node{*t};
  id_t bits= (k >> ((HEIGHT + 1) - t->len));
  if (bits != t->bits) {
   uint8_t i= 64 - __builtin_clzll(bits ^ t->bits);
   bool flg= (t->bits >> (i - 1)) & 1;
   t->ch[flg]= new Node{*t}, t->ch[!flg]= new Node{k, HEIGHT + 1, def_val()};
   t->len-= i, t->bits>>= i;
   return t->ch[!flg]->val;
  } else if (t->len != HEIGHT + 1) return at_val(t->ch[(k >> (HEIGHT - t->len)) & 1], k);
  return t->val;
 }
 bool is_null(np &t, const id_t &k) {
  if (!t || (k >> ((HEIGHT + 1) - t->len)) != t->bits) return true;
  if (t->len == HEIGHT + 1) return false;
  return is_null(t->ch[(k >> (HEIGHT - t->len)) & 1], k);
 }
 T get_val(np &t, const id_t &k) {
  if (!t || (k >> ((HEIGHT + 1) - t->len)) != t->bits) return def_val();
  if (t->len == HEIGHT + 1) return t->val;
  return get_val(t->ch[(k >> (HEIGHT - t->len)) & 1], k);
 }
 template <bool last> static inline T calc_op(np &t, const T &v) {
  if constexpr (last) return M::op((t ? t->val : def_val()), v);
  else return M::op(v, (t ? t->val : def_val()));
 }
 template <bool last> static inline bool is_in(const id_t &m, const id_t &k) {
  if constexpr (last) return k <= m;
  else return m <= k;
 }
 static inline np next(np &t, const uint8_t &h, const bool &f) {
  if (!t) return nullptr;
  uint8_t len= h + t->len - (HEIGHT + 1);
  if (!len) return t->ch[f];
  return (f == ((t->bits >> (len - 1)) & 1)) ? t : nullptr;
 }
 template <bool last, class C, std::size_t N> static id_t find(const id_t &k, std::array<id_t, 2> b, const id_t &bias, uint8_t h, const C &check, std::array<np, N> &ts, std::array<T, N> &sums) {
  static_assert(monoid_v<M>, "\"find\" is not available\n");
  static std::array<T, N> sums2;
  if (std::all_of(ts.begin(), ts.end(), [](np t) { return !t; })) return -1;
  if (!h) {
   for (std::size_t i= N; i--;) sums[i]= calc_op<last>(ts[i], sums[i]);
   return std::apply(check, sums) ? std::get<last>(b) : -1;
  } else if (is_in<last>(k, b[0])) {
   for (std::size_t i= N; i--;) sums2[i]= calc_op<last>(ts[i], sums[i]);
   if (!std::apply(check, sums2)) return sums= std::move(sums2), -1;
  }
  std::array<np, N> ss;
  id_t m= (b[0] + b[1]) >> 1;
  bool flg= (bias >> (h - 1)) & 1;
  if (!is_in<last>(m, k)) {
   for (std::size_t i= N; i--;) ss[i]= next(ts[i], h, flg);
   id_t ret= find<last>(k, {b[0], m}, bias, h - 1, check, ss, sums);
   if (ret >= 0) return ret;
  }
  for (std::size_t i= N; i--;) ss[i]= next(ts[i], h, !flg);
  return find<last>(k, {m, b[1]}, bias, h - 1, check, ss, sums);
 }
public:
 SegmentTree_Patricia(np t= nullptr): root(t) {}
 SegmentTree_Patricia(std::size_t n, T val): root(build(n, 0, 1LL << HEIGHT, val)) {}
 SegmentTree_Patricia(T *bg, T *ed): root(build(ed - bg, 0, 1LL << HEIGHT, bg)) {}
 SegmentTree_Patricia(const std::vector<T> &ar): SegmentTree_Patricia(ar.data(), ar.data() + ar.size()) {}
 void set(id_t k, T val) { set_val(root, k, val); }
 T get(id_t k) { return get_val(root, k); }
 bool is_null(id_t k) { return is_null(root, k); }
 T &at(id_t k) {
  static_assert(!monoid_v<M>, "\"at\" is not available\n");
  return at_val(root, k);
 }
 template <class L= M, std::enable_if_t<monoid_v<L>, std::nullptr_t> = nullptr> T operator[](id_t k) { return get(k); }
 template <class L= M, std::enable_if_t<!monoid_v<L>, std::nullptr_t> = nullptr> T &operator[](id_t k) { return at(k); }
 T fold(id_t a, id_t b, id_t bias= 0) {
  static_assert(monoid_v<M>, "\"fold\" is not available\n");
  return fold(root, a, b, bias);
 }
 // find i s.t.
 //  check(fold(k,i)) == False, check(fold(k,i+1)) == True
 // return -1 if not found
 template <class C> id_t find_first(id_t a, C check, id_t bias= 0) {
  std::array<T, 1> sum{def_val()};
  std::array<np, 1> t{root};
  return find<0>(a, {0, 1LL << HEIGHT}, bias, HEIGHT, check, t, sum);
 }
 template <std::size_t N, class C> static id_t find_first(id_t a, C check, std::array<SegmentTree_Patricia, N> segs, id_t bias= 0) {
  std::array<T, N> sums;
  sums.fill(def_val());
  std::array<np, N> ts;
  for (std::size_t i= 0; i < N; i++) ts[i]= segs[i].root;
  return find<0>(a, {0, 1LL << HEIGHT}, bias, HEIGHT, check, ts, sums);
 }
 // find i s.t.
 //  check(fold(i+1,k)) == False, check(fold(i,k)) == True
 // return -1 if not found
 template <class C> id_t find_last(id_t b, C check, id_t bias= 0) {
  std::array<T, 1> sum{def_val()};
  std::array<np, 1> t{root};
  return find<1>(b, {1LL << HEIGHT, 0}, ~bias, HEIGHT, check, t, sum);
 }
 template <std::size_t N, class C> static id_t find_last(id_t b, C check, std::array<SegmentTree_Patricia, N> segs, id_t bias= 0) {
  std::array<T, N> sums;
  sums.fill(def_val());
  std::array<np, N> ts;
  for (std::size_t i= 0; i < N; i++) ts[i]= segs[i].root;
  return find<1>(b, {1LL << HEIGHT, 0}, ~bias, HEIGHT, check, ts, sums);
 }
 std::vector<T> dump(id_t bg, id_t ed) {
  std::vector<T> ret(ed - bg);
  dump(root, bg, ed, {0, 1LL << HEIGHT}, ret.begin(), HEIGHT);
  return ret;
 }
 static std::string which_available() {
  std::string ret= "";
  if constexpr (monoid_v<M>) ret+= "\"fold\" \"find\"";
  else ret+= "\"at\" ";
  return ret;
 }
};
