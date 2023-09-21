#pragma once
#include <algorithm>
#include "src/Geometry/Line.hpp"
namespace geo {
template <class K> struct Segment {
 using P= Point<K>;
 P p, q;
 Segment() {}
 Segment(const P &p, const P &q): p(p), q(q) {}
 // do not consider the direction
 bool operator==(const Segment &s) const { return (p == s.p && q == s.q) || (p == s.q && q == s.p); }
 bool operator!=(const Segment &s) const { return !(*this == s); }
 bool is_on(const P &r) const { return ccw(p, q, r) == ON_SEGMENT; }
 P &operator[](int i) { return i ? q : p; }
 const P &operator[](int i) const { return i ? q : p; }
 long double length() const { return dist(p, q); }
 P closest_point(const P &r) const {
  P d= q - p;
  K a= dot(r - p, d), b;
  return sgn(a) > 0 ? sgn(a - (b= norm2(d))) < 0 ? p + a / b * d : q : p;
 }
 friend ostream &operator<<(ostream &os, const Segment &s) { return os << s.p << "---" << s.q; }
 friend Visualizer &operator<<(Visualizer &vis, const Segment &s) { return vis.ofs << "Segment " << s.p1 << " " << s.p2 << '\n', vis; }
};
// 1: properly crossing, 0: no intersect, 2: same line
template <class K> vector<Point<K>> cross_points(const Segment<K> &s, const Line<K> &l) {
 Point d= s.q - s.p;
 K a= cross(d, l.d), b= cross(l.p - s.p, l.d);
 if (sgn(a)) {
  if (b/= a; sgn(b) < 0 || sgn(b - 1) > 0) return {};  // no intersect
  else return {s.p + b * d};                           // properly crossing}
 }
 if (sgn(b)) return {};  // disjoint parallel
 return {s.p, s.q};      // same line
}
template <class K> vector<Point<K>> cross_points(const Line<K> &l, const Segment<K> &s) { return cross_points(s, l); }
// 2: same line, 0: no intersect, 1: ...
template <class K> vector<Point<K>> cross_points(const Segment<K> &s, const Segment<K> &t) {
 Point d= s.q - s.p, e= t.q - t.p;
 K a= cross(d, e), b= cross(t.p - s.p, e);
 if (sgn(a)) {
  if (b/= a; sgn(b) < 0 || sgn(b - 1) > 0) return {};                       // no intersect
  if (b= cross(d, s.p - t.p) / a; sgn(b) < 0 || sgn(b - 1) > 0) return {};  // no intersect
  return {t.p + b * e};                                                     // properly crossing
 }
 if (sgn(b)) return {};  // disjoint parallel
 vector<Point<K>> ps;    // same line
 auto insert_if_possible= [&](const Point<K> &p) {
  for (auto q: ps)
   if (p == q) return;
  ps.emplace_back(p);
 };
 if (sgn(dot(t.p - s.p, t.q - s.p)) <= 0) insert_if_possible(s.p);
 if (sgn(dot(t.p - s.q, t.q - s.q)) <= 0) insert_if_possible(s.q);
 if (sgn(dot(s.p - t.p, s.q - t.p)) <= 0) insert_if_possible(t.p);
 if (sgn(dot(s.p - t.q, s.q - t.q)) <= 0) insert_if_possible(t.q);
 return ps;
}
enum INTERSECTION { CROSSING, TOUCHING, DISJOINT, OVERLAP };
ostream &operator<<(ostream &os, INTERSECTION i) { return os << (i == CROSSING ? "CROSSING" : i == TOUCHING ? "TOUCHING" : i == DISJOINT ? "DISJOINT" : "OVERLAP"); }
template <class K> INTERSECTION intersection(const Segment<K> &s, const Segment<K> &t) {
 auto cp= cross_points(s, t);
 return cp.size() == 0 ? DISJOINT : cp.size() == 2 ? OVERLAP : cp[0] == s.p || cp[0] == s.q || cp[0] == t.p || cp[0] == t.q ? TOUCHING : CROSSING;
}
template <class K> K dist2(const Segment<K> &s, const Point<K> &p) { return dist2(p, s.closest_point(p)); }
template <class K> K dist2(const Point<K> &p, const Segment<K> &s) { return dist2(s, p); }
template <class K> K dist2(const Segment<K> &s, const Line<K> &l) { return cross_points(s, l).size() ? 0 : min(dist2(s.p, l), dist2(s.q, l)); }
template <class K> K dist2(const Line<K> &l, const Segment<K> &s) { return dist2(s, l); }
template <class K> K dist2(const Segment<K> &s, const Segment<K> &t) { return cross_points(s, t).size() ? 0 : min({dist2(s, t.p), dist2(s, t.q), dist2(t, s.p), dist2(t, s.q)}); }
template <class K> Segment<K> Affine<K>::operator()(const Segment<K> &s) { return {(*this)(s.p), (*this)(s.q)}; }
}