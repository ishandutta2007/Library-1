#pragma once
#include "src/Geometry/Segment.hpp"
namespace geo {
template <class R> struct Circle {
 using P= Point<R>;
 P o;
 R r;
 Circle() {}
 Circle(const P &o, R r): o(o), r(r) {}
 long double area() const { return r * r * M_PI; }
 // +1: in, 0: on, -1: out
 int where(const P &p) const { return sgn(r * r - dist2(p, o)); }
 // +1: intersect, 0: contact, -1: disjoint
 int where(const Line<R> &l) const { return sgn(r * r - dist2(l, o)); }
 vector<Line<R>> tangent(const P &p) const {
  P d= p - o, e= !d;
  R b= norm(d), a= b - r * r;
  if (int s= sgn(a); s < 0) return {};
  else if (s == 0) return {{p, e}};
  d*= r, e*= sqrt(a);
  return {Line(p, !(d + e)), Line(p, !(d - e))};
 }
 friend ostream &operator<<(ostream &os, const Circle &c) { return os << c.o << " " << c.r; }
 friend Visualizer &operator<<(Visualizer &vis, const Circle &c) { return vis.ofs << "Circle " << c.o << " " << c.r << '\n', vis; }
};
// 2: properly intersect, 1: contact, 0: disjoint, 3: same
template <class R> vector<Point<R>> cross_points(const Circle<R> &c, const Circle<R> &d) {
 Point v= d.o - c.o;
 R g= norm(v), a= c.r - d.r, b= c.r + d.r;
 if (!sgn(g)) {
  if (sgn(a)) return {};
  return {{c.o.x + c.r, c.o.y}, {c.o.x - c.r, c.o.y}, {c.o.x, c.o.y + c.r}};
 }
 int in= sgn(g - a * a), out= sgn(g - b * b);
 if (in < 0 || out > 0) return {};
 if (!in) return {(c.r * d.o - d.r * c.o) / a};
 if (!out) return {(c.r * d.o + d.r * c.o) / b};
 R e= (a * b + g) / (g * 2);
 Point q= c.o + e * v, n= !v * sqrt(c.r * c.r / g - e * e);
 return {q + n, q - n};
}
// 2: properly intersect, 1: contact, 0: disjoint
template <class R> vector<Point<R>> cross_points(const Circle<R> &c, const Line<R> &l) {
 Point<R> v= l.p - c.o;
 R a= norm(l.d), b= dot(l.d, v) / a, d= b * b - (norm(v) - c.r * c.r) / a;
 int s= sgn(d);
 if (s < 0) return {};
 if (!s) return {l.p - b * l.d};
 d= sqrt(d);
 return {l.p - (b + d) * l.d, l.p - (b - d) * l.d};
}
template <class R> vector<Point<R>> cross_points(const Line<R> &l, const Circle<R> &c) { return cross_points(c, l); }
template <class R> vector<Point<R>> cross_points(const Circle<R> &c, const Segment<R> &s) {
 Point<R> u= s.q - s.p, v= s.p - c.o;
 R a= norm(u), b= dot(u, v) / a, d= b * b - (norm(v) - c.r * c.r) / a;
 int t= sgn(d);
 if (t < 0) return {};
 if (!t && sgn(b) <= 0 && sgn(1 + b) >= 0) return {s.p - b * u};
 d= sqrt(d), a= -b - d, b= -b + d;
 vector<Point<R>> ps;
 if (0 <= sgn(a) && sgn(a - 1) <= 0) ps.emplace_back(s.p + a * u);
 if (0 <= sgn(b) && sgn(b - 1) <= 0) ps.emplace_back(s.p + b * u);
 return ps;
}
template <class R> vector<Point<R>> cross_points(const Segment<R> &s, const Circle<R> &c) { return cross_points(c, s); }
template <class R> Circle<R> circumscribed_circle(const Point<R> &A, const Point<R> &B, const Point<R> &C) {
 Point u= !(B - A), v= C - A, o= (A + B + dot(C - B, v) / dot(u, v) * u) / 2;
 return {o, dist(A, o)};
}
template <class R> Circle<R> inscribed_circle(const Point<R> &A, const Point<R> &B, const Point<R> &C) {
 R a= dist(B, C), b= dist(C, A), c= dist(A, B), s= (a + b + c) / 2;
 return {(a * A + b * B + c * C) / (s * 2), sqrt((s - a) * (s - b) * (s - c) / s)};
}
template <class R> vector<Line<R>> common_tangent(const Circle<R> &c, const Circle<R> &d) {
 Point u= d.o - c.o, v= !u;
 R g= norm(u), b;
 if (!sgn(g)) return {};  // same origin
 vector<Line<R>> ls;
 for (R a: {c.r - d.r, c.r + d.r}) {
  if (int s= sgn(b= g - a * a); !s) ls.emplace_back(Line(c.o + c.r * a / g * u, v));
  else if (s > 0) {
   Point x= a / g * u, y= sqrt(b) / g * v, e= x + y, f= x - y;
   ls.emplace_back(Line(c.o + c.r * e, !e)), ls.emplace_back(Line(c.o + c.r * f, !f));
  }
 }
 return ls;
}
}