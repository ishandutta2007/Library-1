#pragma once
#include <limits>
#include <algorithm>
#include <set>
#include <array>
#include <cassert>
#include "src/Optimization/MinMaxEnum.hpp"
template <typename T, MinMaxEnum obj= MINIMIZE> class ConvexHullTrick {
 struct Line {
  T k, m;
  mutable T p;
  bool operator<(const Line &o) const { return k < o.k; }
  bool operator<(T x) const { return p < x; }
 };
 static constexpr T INF= std::numeric_limits<T>::max();
 static T lc_div(T a, T b) {
  if constexpr (std::is_integral_v<T>) return a / b - ((a ^ b) < 0 && a % b);
  else return a / b;
 }
 using ms= std::multiset<Line, std::less<>>;
 ms ls;
 bool insect(typename ms::iterator x, typename ms::iterator y) {
  if (y == ls.end()) return x->p= INF, false;
  if (x->k == y->k) x->p= (x->m > y->m ? INF : -INF);
  else x->p= lc_div(y->m - x->m, x->k - y->k);
  return x->p >= y->p;
 }
public:
 void insert_line(T k, T m) {
  if constexpr (obj == MINIMIZE) k= -k, m= -m;
  auto z= ls.insert({k, m, 0}), y= z++, x= y;
  while (insect(y, z)) z= ls.erase(z);
  if (x != ls.begin() && insect(--x, y)) insect(x, y= ls.erase(y));
  while ((y= x) != ls.begin() && (--x)->p >= y->p) insect(x, ls.erase(y));
 }
 bool empty() const { return ls.empty(); }
 std::array<T, 2> query_line(T x) const {
  assert(!ls.empty());
  auto l= ls.lower_bound(x);
  if constexpr (obj == MINIMIZE) return {-l->k, -l->m};
  else return {l->k, l->m};
 }
 T query(T x) const {
  auto [k, m]= query_line(x);
  return k * x + m;
 }
};
template <typename T> class ConvexHullTrick_XY {
 ConvexHullTrick<long double, MINIMIZE> cht_mn;
 ConvexHullTrick<long double, MAXIMIZE> cht_mx;
 T amx= std::numeric_limits<T>::lowest(), amn= std::numeric_limits<T>::max();
public:
 void insert_line(T a, T b) { cht_mn.insert_line(a, b), cht_mx.insert_line(a, b), amn= std::min(amn, a), amx= std::max(amx, a); }
 bool empty() const { return cht_mn.empty(); }
 T get_max(T x, T y) const {
  assert(!cht_mn.empty());
  if (y == 0) return std::max(amn * x, amx * x);
  auto z= (long double)x / y;
  auto [a, b]= y > 0 ? cht_mx.query_line(z) : cht_mn.query_line(z);
  return T(a) * x + T(b) * y;
 }
 T get_min(T x, T y) const {
  assert(!cht_mn.empty());
  if (y == 0) return std::min(amn * x, amx * x);
  auto z= (long double)x / y;
  auto [a, b]= y > 0 ? cht_mn.query_line(z) : cht_mx.query_line(z);
  return T(a) * x + T(b) * y;
 }
};