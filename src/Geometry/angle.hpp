#pragma once
#include <vector>
#include "src/Geometry/Point.hpp"
namespace geo {
long double radian_to_degree(long double r) { return r * 180.0 / M_PI; }
long double degree_to_radian(long double d) { return d * M_PI / 180.0; }
long double normalize_radian(long double r) { return r= fmod(r + M_PI, 2 * M_PI), r > 0 ? r - M_PI : r + M_PI; }
template <class K> long double angle(const Point<K> &p) { return atan2(p.y, p.x); }
template <class K> long double angle(const Point<K> &p, const Point<K> &q) { return atan2(cross(p, q), dot(p, q)); }
template <class K> Affine<K> rotate(long double theta) {
 K c= cos(theta), s= sin(theta);
 return {c, -s, s, c, Point<K>{0, 0}};
}
template <class K> Affine<K> rotate(const Point<K> &p, long double theta) {
 K c= cos(theta), s= sin(theta);
 return {c, -s, s, c, Point<K>{p.x - c * p.x + s * p.y, p.y - s * p.x - c * p.y}};
}
template <class K> Affine<K> rotate90(const Point<K> &p) { return {0, -1, 1, 0, p - !p}; }
// (-PI,PI], counter-clockwise
template <class K> class AngleComp {
 using P= Point<K>;
 static int quad(const P &p) {
  if (int s= sgn(p.y); s) return s;
  return sgn(p.x) < 0 ? 2 : 0;
 }
public:
 bool operator()(const P &p, const P &q) const {
  if (int a= quad(p), b= quad(q); a != b) return a < b;
  return cross(p, q) > 0;
 }
};
}