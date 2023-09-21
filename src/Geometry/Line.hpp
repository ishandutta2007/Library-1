#pragma once
#include <vector>
#include "src/Geometry/Point.hpp"
namespace geo {
template <class K> struct Line {
 using P= Point<K>;
 P p, d;  // p+td
 Line() {}
 // p + td
 Line(const P &p, const P &d): p(p), d(d) { assert(sgn(norm2(d))); }
 // ax+by+c=0 ................. ax+by+c>0: left, ax+by+c=0: on, ax+by+c<0: right
 Line(K a, K b, K c) {
  int sa= sgn(a), sb= sgn(b);
  assert(sa || sb);
  if (sb) p= {0, -c / b}, d= sb > 0 ? P{1, -a / b} : P{-1, a / b};
  else p= {-c / a, 0}, d= {0, K(sa > 0 ? -1 : 1)};
 }
 bool operator==(const Line &l) const { return !sgn(cross(d, l.d)) && !where(l.p); }
 bool operator!=(const Line &l) const { return sgn(cross(d, l.d)) || where(l.p); }
 // +1: left, 0: on, -1: right
 int where(const P &q) const { return sgn(cross(d, q - p)); }
 P project(const P &q) const { return p + dot(q - p, d) / norm2(d) * d; }
 // return  a,b,c of ax+by+c=0
 tuple<K, K, K> coef() const { return make_tuple(-d.y, d.x, cross(p, d)); }
 friend ostream &operator<<(ostream &os, const Line &l) { return os << l.p << " + t" << l.d; }
 friend Visualizer &operator<<(Visualizer &vis, const Line &l) {
  auto [a, b, c]= l.coef();
  return vis.ofs << "Line " << a << " " << b << " " << c << '\n', vis;
 }
};
// p + t(q-p)
template <class K> Line<K> line_through(const Point<K> &p, const Point<K> &q) { return Line(p, q - p); }
template <class K> bool is_parallel(const Line<K> &l, const Line<K> &m) { return !sgn(cross(l.d, m.d)); }
template <class K> bool is_orthogonal(const Line<K> &l, const Line<K> &m) { return !sgn(dot(l.d, m.d)); }
// 1 : properly crossing, 0 : disjoint parallel, 2 : same line
template <class K> vector<Point<K>> cross_points(const Line<K> &l, const Line<K> &m) {
 K a= cross(m.d, l.d), b= cross(l.p - m.p, l.d);
 if (sgn(a)) return {m.p + b / a * m.d};  // properly crossing
 if (sgn(b)) return {};                   // disjoint parallel
 return {m.p, m.p + m.d};                 // same line
}
// perpendicular bisector ............ p on leftside
template <class K> Line<K> bisector(const Point<K> &p, const Point<K> &q) { return Line((p + q) / 2, !(q - p)); }
// angle bisector ........... parallel -> 1 line, non-parallel -> 2 lines
template <class K> vector<Line<K>> bisector(const Line<K> &l, const Line<K> &m) {
 auto cp= cross_points(l, m);
 if (cp.size() != 1) return {Line((l.p + m.p) / 2, l.d)};
 auto d= l.d / norm(l.d) + m.d / norm(m.d);
 return {Line(cp[0], d), Line(cp[0], !d)};
}
template <class K> K dist2(const Line<K> &l, const Point<K> &p) {
 K a= cross(l.d, p - l.p);
 return a * a / norm2(l.d);
}
template <class K> K dist2(const Point<K> &p, const Line<K> &l) { return dist2(l, p); }
template <class K> K dist2(const Line<K> &l, const Line<K> &m) { return is_parallel(l, m) ? dist2(l, m.p) : 0; }
template <class K> long double angle(const Line<K> &l) { return angle(l.d); }
template <class K> long double angle(const Line<K> &l, const Line<K> &m) { return angle(l.d, m.d); }
template <class K> Affine<K> reflect(const Line<K> &l) {
 K a= l.d.x * l.d.x, b= l.d.x * l.d.y * 2, c= l.d.y * l.d.y, d= a + c;
 a/= d, b/= d, c/= d, d= a - c;
 return {d, b, b, -d, Point<K>{c * 2 * l.p.x - b * l.p.y, a * 2 * l.p.y - b * l.p.x}};
}
template <class K> Line<K> Affine<K>::operator()(const Line<K> &l) { return line_through((*this)(l.p), (*this)(l.p + l.d)); }
}