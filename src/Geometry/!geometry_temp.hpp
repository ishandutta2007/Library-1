#pragma once
#include <bits/stdc++.h>
/**
 * @title 幾何テンプレ
 * @category 幾何
 */

// BEGIN CUT HERE

namespace geometry {

using Real = long double;
int sgn(Real x) {
  static constexpr Real EPS = 1e-8;
  return x < -EPS ? -1 : x > +EPS ? 1 : 0;
}
const Real PI = acos(-1.0);
Real radian_to_degree(Real r) { return (r * 180.0 / PI); }
Real degree_to_radian(Real d) { return (d * PI / 180.0); }
enum {
  COUNTER_CLOCKWISE = +1,
  CLOCKWISE = -1,
  ONLINE_BACK = +2,
  ONLINE_FRONT = -2,
  ON_SEGMENT = 0
};
enum { ON = 0, LEFT = +1, RIGHT = -1, IN = +2, OUT = -2 };
enum { DISJOINT = 0, TOUCH = 1, CROSSING = 2, OVERLAP = 3 };
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
Point rotate(Point p, Real theta) {
  return {cos(theta) * p.x - sin(theta) * p.y,
          sin(theta) * p.x + cos(theta) * p.y};
}
istream &operator>>(istream &is, Point &p) {
  is >> p.x >> p.y;
  return is;
}
ostream &operator<<(ostream &os, Point p) {
  os << p.x << " " << p.y;
  return os;
}
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
struct Segment;
struct Circle;
struct Polygon;
struct Convex;
struct Line {
  Point p1, p2;
  Line() {}
  Line(Point p, Point q) : p1(p), p2(q) {}
  Line(Real a, Real b, Real c) {  // ax+by+c=0
    if (!sgn(b)) {
      p1 = {-c / a, 1}, p2 = {-c / a, 0};
      if (sgn(a) < 0) swap(p1, p2);
    } else if (!sgn(a))
      p1 = {0, -c / b}, p2 = {1, -c / b};
    else
      p1 = {0, -c / b}, p2 = {1, -(c + a) / b};
    if (sgn(b) < 0) swap(p1, p2);
    // ax+by+c>0: left, ax+by+c=0: on, ax+by+c<0: right
  }
  Point &operator[](int i) {
    assert(0 <= i && i <= 1);
    return i ? p2 : p1;
  }
  bool operator==(Line l) const {
    return !sgn(cross(p1 - p2, l.p1 - l.p2)) && !sgn(cross(p1 - p2, l.p1 - p1));
  }
  // 1: left, 0: on, -1: right
  int where(Point p) { return sgn(cross(p1 - p, p2 - p)); }
  // return  a,b,c of ax+by+c=0
  tuple<Real, Real, Real> coef() {
    auto n = orth(p2 - p1);
    return make_tuple(n.x, n.y, -dot(n, p1));
  }
  Point project(Point p) {
    Point v = p2 - p1;
    return p1 + dot(p - p1, v) / dot(v, v) * v;
  }
  Point reflect(Point p) { return 2 * project(p) - p; }
  Line reflect(Line l) { return {reflect(l.p1), reflect(l.p2)}; }
  Segment reflect(Segment s);
  Circle reflect(Circle c);
  Polygon reflect(Polygon g);
  Convex reflect(Convex g);
};

Line bisector(Point p, Point q) {  // p on leftside
  Point m = (p + q) / 2;
  return {m, m + orth(q - p)};
}

struct Segment {
  Point p1, p2;
  Point &operator[](int i) {
    assert(0 <= i && i <= 1);
    return i ? p2 : p1;
  }
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

Line translate(Line l, Point v) { return {l.p1 + v, l.p2 + v}; }
Line rotate(Line l, Real theta) {
  return {rotate(l.p1, theta), rotate(l.p2, theta)};
}

Segment Line::reflect(Segment s) { return {reflect(s.p1), reflect(s.p2)}; }
Segment translate(Segment s, Point v) { return {s.p1 + v, s.p2 + v}; }
Segment rotate(Segment s, Real theta) {
  return {rotate(s.p1, theta), rotate(s.p2, theta)};
}

std::vector<Point> cross_points(Line l, Line m) {
  Real a = cross(m.p2 - m.p1, l.p2 - l.p1);
  Real b = cross(l.p1 - m.p1, l.p2 - l.p1);
  if (sgn(a)) return {m.p1 + b / a * (m.p2 - m.p1)};  // properly crossing
  if (!sgn(b)) return {m.p1, m.p2};                   // same line
  return {};                                          // disjoint parallel
}
std::vector<Point> cross_points(Line l, Segment s) {
  Real a = cross(s.p2 - s.p1, l.p2 - l.p1);
  Real b = cross(l.p1 - s.p1, l.p2 - l.p1);
  if (a < 0) a = -a, b = -b;
  if (sgn(b) < 0 || sgn(a - b) < 0) return {};        // no intersect
  if (sgn(a)) return {s.p1 + b / a * (s.p2 - s.p1)};  // properly crossing
  if (!sgn(b)) return {s.p1, s.p2};                   // same line
  return {};                                          // disjoint parallel
}
std::vector<Point> cross_points(Segment s, Line l) {
  return cross_points(l, s);
}
std::vector<Point> cross_points(Segment s, Segment t) {
  Real a = cross(s.p2 - s.p1, t.p2 - t.p1);
  Real b = cross(t.p1 - s.p1, t.p2 - t.p1);
  Real c = cross(s.p2 - s.p1, s.p1 - t.p1);
  if (a < 0) a = -a, b = -b, c = -c;
  if (sgn(b) < 0 || sgn(a - b) < 0 || sgn(c) < 0 || sgn(a - c) < 0)
    return {};                                        // disjoint
  if (sgn(a)) return {s.p1 + b / a * (s.p2 - s.p1)};  // properly crossing
  std::vector<Point> ps;                              // same line
  auto insert_if_possible = [&](Point p) {
    for (auto q : ps)
      if (sgn(dot(p - q, p - q)) == 0) return;
    ps.emplace_back(p);
  };
  if (sgn(dot(s.p1 - t.p1, s.p2 - t.p1)) <= 0) insert_if_possible(t.p1);
  if (sgn(dot(s.p1 - t.p2, s.p2 - t.p2)) <= 0) insert_if_possible(t.p2);
  if (sgn(dot(t.p1 - s.p1, t.p2 - s.p1)) <= 0) insert_if_possible(s.p1);
  if (sgn(dot(t.p1 - s.p2, t.p2 - s.p2)) <= 0) insert_if_possible(s.p2);
  return ps;
}

int intersect(Segment s, Segment t) {
  auto cp = cross_points(s, t);
  if (cp.size() == 0) return DISJOINT;
  if (cp.size() > 1) return OVERLAP;
  if ((cp[0] == s.p1 || cp[0] == s.p2 || cp[0] == t.p1 || cp[0] == t.p2))
    return TOUCH;
  return CROSSING;
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
  int where(Point p) {
    int s = sgn(norm2(p - o) - r * r);
    return s < 0 ? IN : s == 0 ? ON : OUT;
  }
  std::vector<Line> tangent(Point p) {
    Point u = p - o, v = orth(u);
    Real len = norm2(u) - r * r;
    if (sgn(len) < 0) return {};
    if (sgn(len) == 0) return {{p, p + v}};
    u *= r * r / norm2(u);
    v *= r * sqrt(len) / norm2(v);
    return {{p, o + u + v}, {p, o + u - v}};
  }
};

Circle Line::reflect(Circle c) { return {reflect(c.o), c.r}; }
Circle translate(Circle c, Point v) { return {c.o + v, c.r}; }
Circle rotate(Circle c, Real theta) { return {rotate(c.o, theta), c.r}; }

std::vector<Point> cross_points(Circle c, Circle d) {
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
std::vector<Point> cross_points(Circle c, Line l) {
  Point u = l.p2 - l.p1, v = l.p1 - c.o;
  Real a = norm2(u), b = dot(u, v) / a, t = (norm2(v) - c.r * c.r) / a;
  Real det = b * b - t;
  if (sgn(det) < 0) return {};               // no solution
  if (sgn(det) == 0) return {l.p1 - b * u};  // touch inner/outer
  return {l.p1 - (b + sqrt(det)) * u,        // properly intersect
          l.p1 - (b - sqrt(det)) * u};
}
std::vector<Point> cross_points(Line l, Circle c) { return cross_points(c, l); }
std::vector<Point> cross_points(Circle c, Segment s) {
  Point u = s.p2 - s.p1, v = s.p1 - c.o;
  Real a = norm2(u), b = dot(u, v) / a, t = (norm2(v) - c.r * c.r) / a;
  Real det = b * b - t;
  if (sgn(det) < 0) return {};  // no solution
  if (sgn(det) == 0 && sgn(-b) >= 0 && sgn(1 - (-b)) >= 0)
    return {s.p1 - b * u};  // touch
  Real t1 = -b - sqrt(det), t2 = -b + sqrt(det);
  std::vector<Point> qs;
  if (sgn(t1) >= 0 && sgn(1 - t1) >= 0) qs.emplace_back(s.p1 + t1 * u);
  if (sgn(t2) >= 0 && sgn(1 - t2) >= 0) qs.emplace_back(s.p1 + t2 * u);
  return qs;
}
std::vector<Point> cross_points(Segment s, Circle c) {
  return cross_points(c, s);
}

//-----------------------------------------------------------------------------
// Polygon
// assuming counterclockwise rotation
//-----------------------------------------------------------------------------
struct Polygon : std::vector<Point> {
  using std::vector<Point>::vector;
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
  int where(Point p) {
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
  bool contains(Segment s, int side = IN) {  // +2 for in, -2 for out
    int opp = side == IN ? OUT : IN;
    if (where(s.p1) == opp || where(s.p2) == opp) return false;
    std::vector<Point> ps = {s.p1, s.p2};
    for (int i = 0; i < (int)this->size(); i++)
      for (Point p : cross_points(s, Segment({(*this)[i], (*this)[next(i)]})))
        ps.emplace_back(p);
    int n = ps.size();
    sort(ps.begin(), ps.end());
    for (int i = 0; i + 1 < n; i++) ps.emplace_back((ps[i] + ps[i + 1]) / 2);
    for (Point p : ps)
      if (where(p) == opp) return false;
    return true;
  }
};

Polygon Line::reflect(Polygon g) {
  reverse(g.begin(), g.end());
  Polygon res;
  for (Point p : g) res.emplace_back(reflect(p));
  return res;
}
Polygon translate(Polygon g, Point v) {
  Polygon h(g.size());
  for (int i = 0; i < (int)g.size(); i++) h[i] = g[i] + v;
  return h;
}
Polygon rotate(Polygon g, Real theta) {
  Polygon h(g.size());
  for (int i = 0; i < (int)g.size(); i++) h[i] = rotate(g[i], theta);
  return h;
}

struct Convex : Polygon {
  using Polygon::Polygon;
  std::pair<Point, Point> farthest() {
    int u = 0, v = 1;
    Real best = -1;
    for (int i = 0, j = 1; i < (int)this->size(); ++i)
      while (true) {
        int k = next(j);
        Real len = norm2((*this)[j] - (*this)[i]);
        if (sgn(len - norm2((*this)[k] - (*this)[i])) <= 0)
          j = k;
        else {
          if (best < len) best = len, u = i, v = j;
          break;
        }
      }
    return std::make_pair((*this)[u], (*this)[v]);
  }
  Real diameter() {
    Point p, q;
    tie(p, q) = farthest();
    return dist(p, q);
  }
  Convex cut(Line l, int side = LEFT) {  // +1 for left, -1 for right
    Convex g;
    for (int i = 0; i < (int)this->size(); i++) {
      Point p = (*this)[i], q = (*this)[next(i)];
      if (l.where(p) * side >= 0) g.emplace_back(p);
      if (l.where(p) * l.where(q) < 0) {
        Real a = cross(q - p, l.p2 - l.p1);
        Real b = cross(l.p1 - p, l.p2 - l.p1);
        g.emplace_back(p + b / a * (q - p));
      }
    }
    return g;
  }
};

Convex Line::reflect(Convex g) {
  reverse(g.begin(), g.end());
  Convex res;
  for (Point p : g) res.emplace_back(reflect(p));
  return res;
}
Convex translate(Convex g, Point v) {
  Convex h(g.size());
  for (int i = 0; i < (int)g.size(); i++) h[i] = g[i] + v;
  return h;
}
Convex rotate(Convex g, Real theta) {
  Convex h(g.size());
  for (int i = 0; i < (int)g.size(); i++) h[i] = rotate(g[i], theta);
  return h;
}

Real dist(Polygon g, Point p) {
  if (g.where(p) != OUT) return 0;
  Real res = dist(Segment({g.back(), g[0]}), p);
  for (int i = 0; i + 1 < (int)g.size(); i++)
    res = min(res, dist(Segment({g[i], g[i + 1]}), p));
  return res;
}
Real dist(Point p, Polygon g) { return dist(g, p); }
Real dist(Polygon g, Line l) {
  Real res = dist(Segment({g.back(), g[0]}), l);
  for (int i = 0; i + 1 < (int)g.size(); i++)
    res = min(res, dist(Segment({g[i], g[i + 1]}), l));
  return res;
}
Real dist(Line l, Polygon g) { return dist(g, l); }
Real dist(Polygon g, Segment s) {
  if (g.where(s.p1) != OUT || g.where(s.p2) != OUT) return 0;
  Real res = dist(Segment({g.back(), g[0]}), s);
  for (int i = 0; i + 1 < (int)g.size(); i++)
    res = min(res, dist(Segment({g[i], g[i + 1]}), s));
  return res;
}
Real dist(Segment s, Polygon g) { return dist(g, s); }
Real dist(Polygon g, Polygon h) {
  Real res = dist(Segment({g.back(), g[0]}), h);
  for (int i = 0; i + 1 < (int)g.size(); i++)
    res = min(res, dist(Segment({g[i], g[i + 1]}), h));
  return res;
}

Convex convex_hull(std::vector<Point> ps) {
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
//-----------------------------------------------------------------------------
// visualizer
// to use https://csacademy.com/app/geometry_widget/
//-----------------------------------------------------------------------------
struct Visualizer {
  ofstream ofs;
  Visualizer(string s = "visualize.txt") : ofs(s) {
    ofs << fixed << setprecision(10);
  }
  Visualizer &operator<<(Point p) {
    ofs << p << endl;
    return *this;
  }
  Visualizer &operator<<(Line l) {
    Real A, B, C;
    tie(A, B, C) = l.coef();
    ofs << "Line " << A << " " << B << " " << C << endl;
    return *this;
  }
  Visualizer &operator<<(Segment s) {
    ofs << "Segment " << s.p1 << " " << s.p2 << endl;
    return *this;
  }
  Visualizer &operator<<(Circle c) {
    ofs << "Circle " << c.o << " " << c.r << endl;
    return *this;
  }
  Visualizer &operator<<(Polygon g) {
    ofs << "Polygon" << endl;
    for (Point p : g) ofs << p << endl;
    ofs << "..." << endl;
    return *this;
  }
};

}  // namespace geometry