/**
 * @title 幾何テンプレ
 * @category 幾何
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

namespace geometry {

using Real = long double;
int sgn(Real x) {
  static constexpr Real EPS = 1e-8;
  return x < -EPS ? -1 : x > +EPS ? 1 : 0;
}
const Real PI = acos(-1.0);
enum {
  COUNTER_CLOCKWISE = +1,
  CLOCKWISE = -1,
  ONLINE_BACK = +2,
  ONLINE_FRONT = -2,
  ON_SEGMENT = 0
};
enum { IN = 2, ON = 1, OUT = 0 };
//-----------------------------------------------------------------------------
// Point
//-----------------------------------------------------------------------------
struct Point {
  Real x, y;
  Point &operator+=(Point p) {
    x += p.x;
    y += p.y;
    return *this;
  }
  Point &operator-=(Point p) {
    x -= p.x;
    y -= p.y;
    return *this;
  }
  Point &operator*=(Real a) {
    x *= a;
    y *= a;
    return *this;
  }
  Point &operator/=(Real a) { return *this *= (1.0 / a); }
  Point operator-() const { return {-x, -y}; }
  bool operator<(Point p) const {
    int s = sgn(x - p.x);
    return s ? s < 0 : sgn(y - p.y) < 0;
  }
};
bool operator==(Point p, Point q) { return !(p < q) && !(q < p); }
bool operator!=(Point p, Point q) { return p < q || q < p; }
bool operator<=(Point p, Point q) { return !(q < p); }
Point operator+(Point p, Point q) { return p += q; }
Point operator-(Point p, Point q) { return p -= q; }
Point operator*(Real a, Point p) { return p *= a; }
Point operator*(Point p, Real a) { return p *= a; }
Point operator/(Point p, Real a) { return p /= a; }
Real dot(Point p, Point q) { return p.x * q.x + p.y * q.y; }
Real cross(Point p, Point q) { return p.x * q.y - p.y * q.x; }  // left turn > 0
Real norm2(Point p) { return dot(p, p); }
Real norm(Point p) { return sqrt(norm2(p)); }
Real arg(Point p) { return atan2(p.y, p.x); }
Real dist(Point p, Point q) { return norm(p - q); }
Real arg(Point p, Point q) { return atan2(cross(p, q), dot(p, q)); }
Point orth(Point p) { return {-p.y, p.x}; }
Point rotate(Real theta, Point p) {
  return {cos(theta) * p.x - sin(theta) * p.y,
          sin(theta) * p.x + cos(theta) * p.y};
}
istream &operator>>(istream &is, Point &p) {
  is >> p.x >> p.y;
  return is;
}
ostream &operator<<(ostream &os, Point p) {
  os << fixed << setprecision(12) << p.x << " " << p.y;
  return os;
}
// usage: sort(ps.begin(),ps.end(), polar_angle(origin, direction));
// (-PI,PI]
struct polar_angle {
  const Point o;
  const int s;  // +1 for ccw, -1 for cw
  polar_angle(Point origin = {0, 0}, int dir = +1) : o(origin), s(dir) {}
  int quad(Point p) const {
    for (int i = 0; i < 4; ++i, swap(p.x = -p.x, p.y))
      if (p.x < 0 && p.y < 0) return 2 * i;
    for (int i = 0; i < 4; ++i, swap(p.x = -p.x, p.y))
      if (p.x == 0 && p.y < 0) return 2 * i + 1;
    return 3;  // arg(0,0) = 0
  }
  bool operator()(Point p, Point q) const {
    p = p - o;
    q = q - o;
    if (quad(p) != quad(q)) return s * quad(p) < s * quad(q);
    if (cross(p, q)) return s * cross(p, q) > 0;
    return norm2(p) < norm2(q);  // closer first
  }
};

int ccw(Point p0, Point p1, Point p2) {
  Point a = p1 - p0, b = p2 - p0;
  if (sgn(cross(a, b)) > 0) return COUNTER_CLOCKWISE;
  if (sgn(cross(a, b)) < 0) return CLOCKWISE;
  if (sgn(dot(a, b)) < 0) return ONLINE_BACK;
  if (norm2(a) < norm2(b)) return ONLINE_FRONT;
  return ON_SEGMENT;
}

//-----------------------------------------------------------------------------
// Line and Segment
//-----------------------------------------------------------------------------
struct Line {
  Point p1, p2;
  bool operator==(Line l) const {
    return !sgn(cross(p1 - p2, l.p1 - l.p2)) && !sgn(cross(p1 - p2, l.p1 - p1));
  }
  bool is_on(Point p) { return !sgn(cross(p1 - p, p2 - p)); }
  Point project(Point p) {
    Point v = p2 - p1;
    return p1 + dot(p - p1, v) / dot(v, v) * v;
  }
  Point reflect(Point p) { return 2 * project(p) - p; }
};

struct Segment {
  Point p1, p2;
  bool operator==(Segment s) const {  // do not consider the direction
    return (p1 == s.p1 && p2 == s.p2) || (p1 == s.p2 && p2 == s.p1);
  }
  int is_on(Point p) {
    if (p1 == p || p2 == p) return 2;  // end Point
    return !sgn(cross(p1 - p, p2 - p)) && sgn(dot(p1 - p, p2 - p) < 0);
  }
  Point closest_point(Point p) {
    Point v = p2 - p1;
    Real a = dot(v, v), b = dot(p - p1, v);
    if (sgn(b) < 0) return p1;
    if (sgn(a - b) < 0) return p2;
    return p1 + b / a * v;
  }
};

bool is_orthogonal(Line l, Line m) {
  return !sgn(dot(l.p1 - l.p2, m.p1 - m.p2));
}
bool is_parallel(Line l, Line m) {
  return !sgn(cross(l.p1 - l.p2, m.p1 - m.p2));
}

vector<Point> cross_points(Line l, Line m) {
  Real a = cross(m.p2 - m.p1, l.p2 - l.p1);
  Real b = cross(l.p1 - m.p1, l.p2 - l.p1);
  if (sgn(a)) return {m.p1 + b / a * (m.p2 - m.p1)};  // properly crossing
  if (!sgn(b)) return {m.p1, m.p2};                   // same line
  return {};                                          // disjoint parallel
}
vector<Point> cross_points(Line l, Segment s) {
  Real a = cross(s.p2 - s.p1, l.p2 - l.p1);
  Real b = cross(l.p1 - s.p1, l.p2 - l.p1);
  if (a < 0) a = -a, b = -b;
  if (sgn(b) < 0 || sgn(a - b) < 0) return {};        // no intersect
  if (sgn(a)) return {s.p1 + b / a * (s.p2 - s.p1)};  // properly crossing
  if (!sgn(b)) return {s.p1, s.p2};                   // same line
  return {};                                          // disjoint parallel
}
vector<Point> cross_points(Segment s, Line l) { return cross_points(l, s); }
vector<Point> cross_points(Segment s, Segment t) {
  Real a = cross(s.p2 - s.p1, t.p2 - t.p1);
  Real b = cross(t.p1 - s.p1, t.p2 - t.p1);
  Real c = cross(s.p2 - s.p1, s.p1 - t.p1);
  if (a < 0) a = -a, b = -b, c = -c;
  if (sgn(b) < 0 || sgn(a - b) < 0 || sgn(c) < 0 || sgn(a - c) < 0)
    return {};                                        // disjoint
  if (sgn(a)) return {s.p1 + b / a * (s.p2 - s.p1)};  // properly crossing
  vector<Point> ps;                                   // same line
  auto insert_if_possible = [&](Point p) {
    for (auto q : ps)
      if (sgn(dot(p - q, p - q)) == 0) return;
    ps.push_back(p);
  };
  if (sgn(dot(s.p1 - t.p1, s.p2 - t.p1)) <= 0) insert_if_possible(t.p1);
  if (sgn(dot(s.p1 - t.p2, s.p2 - t.p2)) <= 0) insert_if_possible(t.p2);
  if (sgn(dot(t.p1 - s.p1, t.p2 - s.p1)) <= 0) insert_if_possible(s.p1);
  if (sgn(dot(t.p1 - s.p2, t.p2 - s.p2)) <= 0) insert_if_possible(s.p2);
  return ps;
}

Real dist(Line l, Point p) { return dist(p, l.project(p)); }
Real dist(Point p, Line l) { return dist(l, p); }
Real dist(Line l, Line m) {
  return sgn(cross(l.p1 - l.p2, m.p1 - m.p2)) ? 0 : dist(l.p1, m);
}
Real dist(Segment s, Point p) { return dist(p, s.closest_point(p)); }
Real dist(Point p, Segment s) { return dist(s, p); }
Real dist(Line l, Segment s) {
  return cross_points(l, s).size() ? 0 : min(dist(l, s.p1), dist(l, s.p2));
}
Real dist(Segment s, Line l) { return dist(l, s); }
Real dist(Segment s, Segment t) {
  if (cross_points(s, t).size()) return 0;
  return min({dist(s.p1, t), dist(s.p2, t), dist(t.p1, s), dist(t.p2, s)});
}
//-----------------------------------------------------------------------------
// Circle
//-----------------------------------------------------------------------------
struct Circle {
  Point o;
  Real r;
  Real area() { return PI * r * r; }
  int contains(Point p) {
    int s = sgn(norm2(p - o) - r * r);
    return s < 0 ? IN : s == 0 ? ON : OUT;
  }
  vector<Line> tangent(Point p) {
    Point u = p - o, v = orth(u);
    Real dis = norm2(u) - r * r;
    if (sgn(dis) < 0) return {};
    if (sgn(dis) == 0) return {{p, p + v}};
    u *= r * r / norm2(u);
    v *= r * sqrt(dis) / norm2(v);
    return {{p, o + u - v}, {p, o + u + v}};
  }
};

Circle inscribed_circle(Point A, Point B, Point C) {
  Real a = dist(B, C), b = dist(C, A), c = dist(A, B);
  Point o = (a * A + b * B + c * C) / (a + b + c);
  return {o, dist(Segment{A, B}, o)};
}

Circle circumscribed_circle(Point A, Point B, Point C) {
  Point u = orth(B - A), v = C - A;
  Point o = (A + B + u * dot(C - B, v) / dot(u, v)) / 2;
  return {o, dist(A, o)};
}

vector<Line> tangent(Circle c, Circle d) {
  Real dis = dist(c.o, d.o);
  if (sgn(dis) == 0) return {};  // same origin
  Point u = (d.o - c.o) / dis, v = orth(u);
  vector<Line> ls;
  for (int s = +1; s >= -1; s -= 2) {
    Real h = (c.r + s * d.r) / dis;
    if (sgn(1 - h * h) == 0) {  // touch inner/outer
      ls.emplace_back(Line{c.o + h * c.r * u, c.o + h * c.r * (u + v)});
    } else if (sgn(1 - h * h) > 0) {  // properly intersect
      Point uu = h * u, vv = sqrt(1 - h * h) * v;
      ls.emplace_back(Line{c.o + c.r * (uu + vv), d.o - d.r * (uu + vv) * s});
      ls.emplace_back(Line{c.o + c.r * (uu - vv), d.o - d.r * (uu - vv) * s});
    }
  }
  return ls;
}

vector<Point> cross_points(Circle c, Circle d) {
  if (c.r < d.r) swap(c, d);
  Real g = norm2(c.o - d.o);
  if (sgn(g) == 0) {
    if (sgn(c.r - d.r)) return {};
    return {{c.o.x + c.r, c.o.y},
            {c.o.x, c.o.y + c.r},
            {c.o.x - c.r, c.o.y}};  // same
  }
  int inner = sgn(g - (c.r - d.r) * (c.r - d.r));
  int outer = sgn(g - (c.r + d.r) * (c.r + d.r));
  if (inner < 0 || outer > 0) return {};
  if (inner == 0) return {(c.r * d.o - d.r * c.o) / (c.r - d.r)};
  if (outer == 0) return {(c.r * d.o + d.r * c.o) / (c.r + d.r)};
  Real eta = (c.r * c.r - d.r * d.r + g) / (2 * g);
  Real mu = sqrt(c.r * c.r / g - eta * eta);
  Point q = c.o + eta * (d.o - c.o), v = mu * orth(d.o - c.o);
  return {q + v, q - v};
}
vector<Point> cross_points(Circle c, Line l) {
  Point u = l.p2 - l.p1, v = l.p1 - c.o;
  Real a = norm2(u), b = dot(u, v) / a, t = (norm2(v) - c.r * c.r) / a;
  Real det = b * b - t;
  if (sgn(det) < 0) return {};               // no solution
  if (sgn(det) == 0) return {l.p1 - b * u};  // touch inner/outer
  return {l.p1 - (b + sqrt(det)) * u,        // properly intersect
          l.p1 - (b - sqrt(det)) * u};
}
vector<Point> cross_points(Line l, Circle c) { return cross_points(c, l); }

//-----------------------------------------------------------------------------
// Polygon
// assuming counterclockwise rotation
//-----------------------------------------------------------------------------
struct Polygon : vector<Point> {
  using vector<Point>::vector;
  int prev(int i) { return i ? i - 1 : (int)this->size() - 1; }
  int next(int i) { return (i + 1 == (int)this->size() ? 0 : i + 1); }
  bool is_convex() {
    for (int i = 0; i < (int)this->size(); i++)
      if (ccw((*this)[prev(i)], (*this)[i], (*this)[next(i)]) == CLOCKWISE)
        return false;
    return true;
  }
  Real area() {
    if (this->size() <= 2) return 0;
    Real a = cross(this->back(), (*this)[0]);
    for (int i = 0; i + 1 < (int)this->size(); i++)
      a += cross((*this)[i], (*this)[i + 1]);
    return a / 2;
  }
  int contains(Point p) {
    bool in = false;
    for (int i = 0; i < (int)this->size(); i++) {
      Point a = (*this)[i] - p, b = (*this)[next(i)] - p;
      if (a.y > b.y) std::swap(a, b);
      if (a.y <= 0 && 0 < b.y && cross(a, b) < 0) in = !in;
      if (!sgn(cross(a, b)) && sgn(dot(a, b)) <= 0)
        return ON;  // Point on the edge
    }
    return in ? IN : OUT;  // Point in:out the Polygon
  }
};

struct Convex : Polygon {
  using Polygon::Polygon;
  pair<Point, Point> farthest() {
    if (this->size() == 2) return {(*this)[0], (*this)[1]};
    int i = 0, j = 0;
    for (int k = 0; k < (int)this->size(); k++) {
      if ((*this)[i] < (*this)[k]) i = k;
      if ((*this)[k] <= (*this)[j]) j = k;
    }
    Real max_dist = 0;
    Point p, q;
    for (int si = i, sj = j; i != sj || j != si;) {
      if (max_dist < norm2((*this)[i] - (*this)[j])) {
        max_dist = norm2((*this)[i] - (*this)[j]);
        p = (*this)[i];
        q = (*this)[j];
      }
      int ni = next(i), nj = next(j);
      if (cross((*this)[ni] - (*this)[i], (*this)[nj] - (*this)[j]) <= 0)
        i = ni;
      else
        j = nj;
    }
    return {p, q};
  }
  Real diameter() {
    Point p, q;
    tie(p, q) = farthest();
    return dist(p, q);
  }
  Convex cut(Line l) {
    Convex g;
    for (int i = 0; i < (int)this->size(); i++) {
      Point p = (*this)[i], q = (*this)[next(i)];
      if (sgn(cross(l.p1 - p, l.p2 - p) >= 0)) g.push_back(p);
      if (sgn(cross(l.p1 - p, l.p2 - p)) * sgn(cross(l.p1 - q, l.p2 - q)) < 0) {
        Real a = cross(q - p, l.p2 - l.p1);
        Real b = cross(l.p1 - p, l.p2 - l.p1);
        g.push_back(p + b / a * (q - p));
      }
    }
    return g;
  }
};

Convex convex_hull(vector<Point> ps) {
  int n = ps.size(), k = 0;
  sort(ps.begin(), ps.end());
  Convex ch(2 * n);
  for (int i = 0; i < n; ch[k++] = ps[i++])
    while (k >= 2 && ccw(ch[k - 2], ch[k - 1], ps[i]) == CLOCKWISE) k--;
  for (int i = n - 2, t = k + 1; i >= 0; ch[k++] = ps[i--])
    while (k >= t && ccw(ch[k - 2], ch[k - 1], ps[i]) == CLOCKWISE) k--;
  ch.resize(k - 1);
  return ch;
}

}  // namespace geometry
