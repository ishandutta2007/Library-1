#pragma once
#include "src/Geometry/angle.hpp"
#include "src/Geometry/Polygon.hpp"
#include "src/Geometry/Circle.hpp"
namespace geo {
template <class R> R intersection_area(const Circle<R> &c, const Circle<R> &d) {
 R l= dist2(d.o, c.o), g= c.r + d.r;
 if (sgn(l - g * g) >= 0) return 0;
 R h= c.r - d.r, a= c.r * c.r, b= d.r * d.r;
 if (sgn(l - h * h) <= 0) return min(a, b) * M_PI;
 R m= sqrt(l), z= (a - b + l) / (m * 2), e= z / c.r, f= (m - z) / d.r;
 return a * acos(e) + b * acos(f) - m * sqrt(a - z * z);
}
template <class R> R intersection_area(const Circle<R> &c, const Polygon<R> &g) {
 using P= Point<R>;
 auto sub= [&](const P &p, const P &q) {
  P u= q - p;
  R a= norm2(u), b= dot(u, p) / a, e= c.r * c.r, d= b * b - (norm2(p) - e) / a;
  if (sgn(d) <= 0) return e * angle(p, q);
  d= sqrt(d);
  P x= p + clamp(-b - d, R(0), R(1)) * u, y= p + clamp(-b + d, R(0), R(1)) * u;
  return (cross(x, y) + e * (angle(p, x) + angle(y, q)));
 };
 vector<P> ps;
 for (const auto &p: g) ps.emplace_back(p - c.o);
 R ret= 0;
 for (int i= ps.size(); i--;) ret+= sub(ps[i], ps[g.next(i)]);
 return ret / 2;
}
template <class R> R intersection_area(const Polygon<R> &g, const Circle<R> &c) { return intersection_area(c, g); }
}