#pragma once
#include <vector>
#include "src/Geometry/Point.hpp"
namespace geo {
// (-PI,PI], counter-clockwise
template <class K> class Polar {
 using P= Point<K>;
 const P o;
 static int quad(const P &p) {
  if (int s= sgn(p.y); s) return s;
  return sgn(p.x) < 0 ? 2 : 0;
 }
public:
 Polar(const P &origin= P()): o(origin) {}
 bool operator()(P p, P q) const {
  if (int a= quad(p-= o), b= quad(q-= o); a != b) return a < b;
  return cross(p, q) > 0;
 }
};
}