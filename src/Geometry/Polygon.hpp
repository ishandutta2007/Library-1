#pragma once
#include "src/Geometry/Segment.hpp"
namespace geo {
// build counterclockwise
template <class K> class Polygon {
 using P= Point<K>;
 make_long_t<K> a2= 0;
protected:
 vector<P> dat;
 void build() {
  if (dat.empty()) return;
  a2= cross(dat.back(), dat[0]);
  for (int i= this->size(); --i;) a2+= cross(dat[i - 1], dat[i]);
  if (a2 < 0) reverse(dat.begin(), dat.end()), a2= -a2;
 }
 template <int opp> inline bool contain(const Segment<K> &s) const {
  assert(dat.size());
  if (where(s.p) == opp || where(s.q) == opp) return false;
  vector<P> ps, qs;
  for (const auto &e: edges())
   if (auto cp= cross_points(s, e); cp.size()) ps.insert(ps.end(), cp.begin(), cp.end());
  if (ps.empty()) return true;
  sort(ps.begin(), ps.end()), ps.erase(unique(ps.begin(), ps.end()), ps.end());
  for (int i= ps.size(); --i;) qs.emplace_back((ps[i] + ps[i - 1]) / 2);
  for (const auto &q: qs)
   if (where(q) == opp) return false;
  return true;
 }
public:
 Polygon() {}
 Polygon(const vector<P> &ps): dat(ps) { build(); }
 inline int prev(int i) const { return i ? i - 1 : (int)this->size() - 1; }
 inline int next(int i) const { return i + 1 >= (int)this->size() ? 0 : i + 1; }
 const P &operator[](int i) const { return dat[i]; }
 auto begin() const { return dat.begin(); }
 auto end() const { return dat.end(); }
 size_t size() const { return dat.size(); }
 vector<Segment<K>> edges() const {
  vector<Segment<K>> es;
  for (int i= 0, e= dat.size(); i < e; ++i) es.emplace_back(dat[i], dat[next(i)]);
  return es;
 }
 // assuming no self-intersections
 bool is_convex() const {
  assert(dat.size());
  for (int i= dat.size(); i--;)
   if (P a= dat[i], b= a - dat[prev(i)], c= dat[next(i)] - a; sgn(cross(b, c)) < 0) return false;
  return true;
 }
 make_long_t<K> area() const { return a2 / 2; }
 // for integer
 make_long_t<K> area2() const { return a2; }
 // 1: in, 0: on, -1: out
 int where(const P &p) const {
  assert(dat.size());
  bool in= false;
  for (int i= dat.size(); i--;) {
   Point a= dat[i] - p, b= dat[next(i)] - p;
   if (a.y > b.y) swap(a, b);
   int s= sgn(cross(a, b));
   if (!s && sgn(dot(a, b)) <= 0) return 0;
   if (s < 0 && sgn(a.y) <= 0 && 0 < sgn(b.y)) in= !in;
  }
  return in ? 1 : -1;
 }
 bool in(const Segment<K> &s) const { return contain<-1>(s); }
 bool out(const Segment<K> &s) const { return contain<+1>(s); }
 friend ostream &operator<<(ostream &os, const Polygon &g) {
  for (int i= 0, e= g.size(); i < e; ++i) os << "--" << g[i] << "-";
  return os;
 }
 friend Visualizer &operator<<(Visualizer &vis, const Polygon &g) {
  vis.ofs << "Polygon" << '\n';
  for (const auto &p: g) vis << p;
  return vis.ofs << "..." << '\n', vis;
 }
};
template <class K> make_long_t<K> dist2(const Polygon<K> &g, const Point<K> &p) {
 if (g.where(p) != -1) return 0;
 make_long_t<K> ret= numeric_limits<make_long_t<K>>::max();
 for (const auto &e: g.edges()) ret= min(ret, dist2(e, p));
 return ret;
}
template <class K> make_long_t<K> dist2(const Point<K> &p, const Polygon<K> &g) { return dist2(g, p); }
template <class K> make_long_t<K> dist2(const Polygon<K> &g, const Line<K> &l) {
 make_long_t<K> ret= numeric_limits<make_long_t<K>>::max();
 for (const auto &e: g.edges()) ret= min(ret, dist2(e, l));
 return ret;
}
template <class K> make_long_t<K> dist2(const Line<K> &l, const Polygon<K> &g) { return dist2(g, l); }
template <class K> make_long_t<K> dist2(const Polygon<K> &g, const Segment<K> &s) {
 if (g.where(s.p) != -1 || g.where(s.q) != -1) return 0;
 make_long_t<K> ret= numeric_limits<make_long_t<K>>::max();
 for (const auto &e: g.edges()) ret= min(ret, dist2(e, s));
 return ret;
}
template <class K> make_long_t<K> dist2(const Segment<K> &s, const Polygon<K> &g) { return dist2(g, s); }
template <class K> make_long_t<K> dist2(const Polygon<K> &g, const Polygon<K> &h) {
 make_long_t<K> ret= numeric_limits<make_long_t<K>>::max();
 for (const auto &e: g.edges()) ret= min(ret, dist2(h, e));
 return ret;
}
template <class K> Polygon<K> Affine<K>::operator()(const Polygon<K> &g) {
 vector<Point<K>> ps;
 for (const auto &p: g) ps.emplace_back((*this)(p));
 return Polygon(ps);
}
}