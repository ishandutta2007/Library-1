#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cassert>
namespace geo {
using namespace std;
struct Visualizer {
 ofstream ofs;
 Visualizer(string s= "visualize.txt"): ofs(s) { ofs << fixed << setprecision(10); }
};
template <class K> int sgn(K x) {
 if constexpr (is_floating_point_v<K>) {
  static constexpr K EPS= 1e-9;
  return x < -EPS ? -1 : x > EPS;
 } else return x < 0 ? -1 : x > 0;
}
template <class K> K err_floor(K x) {
 K y= floor(x);
 if constexpr (is_floating_point_v<K>)
  if (K z= y + 1, w= x - z; 0 <= sgn(w) && sgn(w - 1) < 0) return z;
 return y;
}
template <class K> K err_ceil(K x) {
 K y= ceil(x);
 if constexpr (is_floating_point_v<K>)
  if (K z= y - 1, w= x - z; 0 < sgn(w + 1) && sgn(w) <= 0) return z;
 return y;
}
template <class K> struct Point {
 K x, y;
 Point(K x= K(), K y= K()): x(x), y(y) {}
 Point &operator+=(const Point &p) { return x+= p.x, y+= p.y, *this; }
 Point &operator-=(const Point &p) { return x-= p.x, y-= p.y, *this; }
 Point &operator*=(K a) { return x*= a, y*= a, *this; }
 Point &operator/=(K a) { return x/= a, y/= a, *this; }
 Point operator+(const Point &p) const { return {x + p.x, y + p.y}; }
 Point operator-(const Point &p) const { return {x - p.x, y - p.y}; }
 Point operator*(K a) const { return {x * a, y * a}; }
 Point operator/(K a) const { return {x / a, y / a}; }
 friend Point operator*(K a, const Point &p) { return {a * p.x, a * p.y}; }
 Point operator-() const { return {-x, -y}; }
 bool operator<(const Point &p) const {
  int s= sgn(x - p.x);
  return s ? s < 0 : sgn(y - p.y) < 0;
 }
 bool operator>(const Point &p) const { return p < *this; }
 bool operator<=(const Point &p) const { return !(p < *this); }
 bool operator>=(const Point &p) const { return !(*this < p); }
 bool operator==(const Point &p) const { return !sgn(x - p.x) && !sgn(y - p.y); }
 bool operator!=(const Point &p) const { return sgn(x - p.x) || sgn(y - p.y); }
 Point operator!() const { return {-y, x}; }  // rotate 90 degree
 friend istream &operator>>(istream &is, Point &p) { return is >> p.x >> p.y; }
 friend ostream &operator<<(ostream &os, const Point &p) { return os << "(" << p.x << ", " << p.y << ")"; }
 friend Visualizer &operator<<(Visualizer &vis, const Point &p) { return vis.ofs << p.x << " " << p.y << "\n", vis; }
};
template <class K> K dot(const Point<K> &p, const Point<K> &q) { return p.x * q.x + p.y * q.y; }
// left turn: > 0, right turn: < 0
template <class K> K cross(const Point<K> &p, const Point<K> &q) { return p.x * q.y - p.y * q.x; }
template <class K> K norm2(const Point<K> &p) { return dot(p, p); }
template <class K> long double norm(const Point<K> &p) { return sqrt(norm2(p)); }
template <class K> K dist2(const Point<K> &p, const Point<K> &q) { return norm2(p - q); }
template <class T, class U> long double dist(const T &a, const U &b) { return sqrt(dist2(a, b)); }
enum CCW { COUNTER_CLOCKWISE, CLOCKWISE, ONLINE_BACK, ONLINE_FRONT, ON_SEGMENT };
ostream &operator<<(ostream &os, CCW c) { return os << (c == COUNTER_CLOCKWISE ? "COUNTER_CLOCKWISE" : c == CLOCKWISE ? "CLOCKWISE" : c == ONLINE_BACK ? "ONLINE_BACK" : c == ONLINE_FRONT ? "ONLINE_FRONT" : "ON_SEGMENT"); }
template <class K> CCW ccw(const Point<K> &p0, const Point<K> &p1, const Point<K> &p2) {
 Point a= p1 - p0, b= p2 - p0;
 if (int s= sgn(cross(a, b)); s) return s > 0 ? COUNTER_CLOCKWISE : CLOCKWISE;
 if (K d= dot(a, b); sgn(d) < 0) return ONLINE_BACK;
 else return sgn(d - norm2(a)) > 0 ? ONLINE_FRONT : ON_SEGMENT;
}
template <class K> struct Line;
template <class K> struct Segment;
template <class K> struct Polygon;
template <class K> struct Convex;
template <class K> struct Affine {
 K a00= 1, a01= 0, a10= 0, a11= 1;
 Point<K> b;
 Point<K> operator()(const Point<K> &p) const { return {a00 * p.x + a01 * p.y + b.x, a10 * p.x + a11 * p.y + b.y}; }
 Line<K> operator()(const Line<K> &l);
 Segment<K> operator()(const Segment<K> &s);
 Polygon<K> operator()(const Polygon<K> &p);
 Convex<K> operator()(const Convex<K> &c);
 Affine operator*(const Affine &r) const { return {a00 * r.a00 + a01 * r.a10, a00 * r.a01 + a01 * r.a11, a10 * r.a00 + a11 * r.a10, a10 * r.a01 + a11 * r.a11, (*this)(r)}; }
 Affine &operator*=(const Affine &r) { return *this= *this * r; }
};
template <class K> Affine<K> translate(const Point<K> &p) { return {1, 0, 0, 1, p}; }
}