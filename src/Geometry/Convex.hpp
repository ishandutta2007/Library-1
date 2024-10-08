#pragma once
#include "src/Geometry/Polygon.hpp"
namespace geo {
template <class K> struct Convex: Polygon<K> {
 using P= Point<K>;
 Convex() {}
 Convex(vector<P> ps, bool strict= true) {
  sort(ps.begin(), ps.end()), ps.erase(unique(ps.begin(), ps.end()), ps.end());
  int n= ps.size(), k= 0;
  auto &ch= this->dat;
  if (n < 3) {
   ch= ps;
   return;
  }
  ch.resize(2 * n);
  for (int i= 0; i < n; ch[k++]= ps[i++])
   while (k > 1 && sgn(cross(ch[k - 1] - ch[k - 2], ps[i] - ch[k - 2])) < strict) --k;
  for (int i= n - 1, t= k; i--; ch[k++]= ps[i])
   while (k > t && sgn(cross(ch[k - 1] - ch[k - 2], ps[i] - ch[k - 2])) < strict) --k;
  ch.resize(k - 1), this->build();
 }
 pair<P, P> farthest_pair() const {
  auto &ch= this->dat;
  int n= ch.size(), i= 0, j= 0;
  if (n == 2) return {ch[0], ch[1]};
  if (n == 1) return {ch[0], ch[0]};
  for (int k= n; k--;) {
   if (ch[i] > ch[k]) i= k;
   if (ch[j] < ch[k]) j= k;
  }
  pair<P, P> ret{ch[i], ch[j]};
  make_long_t<K> mx= dist2(ch[i], ch[j]);
  for (int si= i, sj= j; i != sj || j != si;) {
   if (int ni= this->next(i), nj= this->next(j); sgn(cross(ch[ni] - ch[i], ch[nj] - ch[j])) < 0) i= ni;
   else j= nj;
   if (make_long_t<K> len= dist2(ch[i], ch[j]); mx < len) mx= len, ret= {ch[i], ch[j]};
  }
  return ret;
 }
 long double diameter() const {
  auto [p, q]= farthest_pair();
  return dist(p, q);
 }
 // side>0 => left, side<0 => right
 Convex cut(const Line<K> &l, int side= 1) const {
  Convex ret;
  for (const auto &e: this->edges()) {
   auto d= e.q - e.p;
   K a= cross(d, l.d), b= cross(l.p - e.p, l.d);
   int s= sgn(b);
   if (s * side >= 0) ret.dat.emplace_back(e.p);
   if (s && sgn(a))
    if (b/= a; 0 < sgn(b) && sgn(b - 1) < 0) ret.dat.emplace_back(e.p + b * d);
  }
  return ret.build(), ret;
 }
 // { (x,y): (x,y) in polygon and (ax+by+c) * side >= 0 }
 Convex cut(K a, K b, K c, int side= 1) const {
  int sa= sgn(a), sb= sgn(b), sc= sgn(c);
  if (!sa && !sb) return sc * side < 0 ? Convex() : *this;
  return cut(Line<K>(a, b, c), side);
 }
 friend Affine<K>;
};
template <class K> pair<Point<K>, Point<K>> farthest_pair(const vector<Point<K>> &ps) { return Convex<K>(ps).farthest_pair(); }
template <class K> Convex<K> Affine<K>::operator()(const Convex<K> &c) {
 Convex<K> d;
 for (const auto &p: c) d.dat.emplace_back((*this)(p));
 return d.build(), d;
}
}