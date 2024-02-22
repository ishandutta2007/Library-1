#pragma once
#include "src/Graph/HeavyLightDecomposition.hpp"
namespace period_internal {
template <class Map> struct PeriodB {
 using Iter= typename Map::const_iterator;
 Map mp;
};
template <class T> using PerB= std::conditional_t<std::is_integral_v<T>, PeriodB<std::unordered_map<T, int>>, PeriodB<std::map<T, int>>>;
}
template <class T= int> struct Period: period_internal::PerB<T> {
 using typename period_internal::PerB<T>::Iter;
 using Path= std::vector<std::pair<int, int>>;
 std::vector<int> t, rt;
 std::vector<T> dc;
 HeavyLightDecomposition hld;
 static std::vector<int> iota(int n) {
  std::vector<int> v(n);
  for (int i= n; i--;) v[i]= i;
  return v;
 }
public:
 Period()= default;
 template <class F> Period(const F &f, const std::vector<T> &inits) {
  int n= 0;
  auto id= [&](const T &x) {
   if (auto it= this->mp.find(x); it != this->mp.end()) return it->second;
   return dc.emplace_back(x), t.push_back(-1), rt.push_back(-1), this->mp[x]= n++;
  };
  for (const T &s: inits)
   if (int v= id(s), w; rt[v] == -1) {
    for (w= v;; rt[w]= -2, w= t[w]= id(f(dc[w])))
     if (rt[w] != -1) {
      if (rt[w] != -2) w= rt[w];
      break;
     }
    for (int u= v; rt[u] == -2; u= t[u]) rt[u]= w;
   }
  Graph g(n + 1, n);
  for (int v= n; v--;) g[v]= {(rt[v] == v ? n : t[v]), v};
  hld= HeavyLightDecomposition(g.adjacency_vertex(1), n);
 }
 Period(const std::vector<int> &functional): Period([&](int x) { return functional[x]; }, iota(functional.size())) { static_assert(std::is_same_v<T, int>); }
 int operator()(const T &x) const {
  Iter it= this->mp.find(x);
  assert(it != this->mp.end());
  return t.size() - hld.to_seq(it->second);
 }
 size_t size() const { return t.size(); }
 // f^k(x)
 template <class Int, class= std::void_t<decltype(std::declval<Int>() % std::declval<int>())>> T jump(const T &x, Int k) const {
  Iter it= this->mp.find(x);
  assert(it != this->mp.end());
  int v= it->second, n= t.size(), d= hld.depth(v) - 1;
  if (k <= d) return dc[hld.la(v, (int)k)];
  int b= t[v= rt[v]], l= (k-= d) % hld.depth(b);
  if (l == 0) return dc[v];
  return dc[hld.la(b, l - 1)];
 }
 // x, f(x), f(f(x)), ... f^k(x)
 // (x,...,f^i(x)), (f^(i+1)(x),...,f^(j-1)(x)) x loop, (f^j(x),...,f^k(x))
 // sequence of half-open intervals [l,r)
 template <class Int, class= std::void_t<decltype(std::declval<Int>() % std::declval<int>())>> std::tuple<Path, Path, Int, Path> path(const T &x, Int k) const {
  Iter it= this->mp.find(x);
  assert(it != this->mp.end());
  int v= it->second, n= t.size(), d= hld.depth(v) - 1;
  std::array<Path, 3> pth;
  Int cnt= 0;
  if (k > d) {
   int b= t[rt[v]], c= hld.depth(b), l= (k-= d) % c;
   if (pth[0]= hld.path(v, hld.la(v, d)), pth[1]= hld.path(b, hld.la(b, c - 1)), cnt= k / c; l) pth[2]= hld.path(b, hld.la(b, l - 1));
  } else pth[0]= hld.path(v, hld.la(v, (int)k));
  for (int s= 3; s--;)
   for (auto &[l, r]: pth[s]) l= n - l, r= n - r + 1;
  return {pth[0], pth[1], cnt, pth[2]};
 }
};