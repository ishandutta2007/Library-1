#pragma once
#include <algorithm>
#include <unordered_set>
#include "src/Geometry/Segment.hpp"
#include "src/Geometry/angle.hpp"
#include "src/Internal/ListRange.hpp"
namespace geo {
template <class K> class SegmentArrangement {
 vector<Point<K>> ps;
 vector<int> dst, pr, nx, icf, cmp, g, pos;
public:
 SegmentArrangement(const vector<Segment<K>> &ss) {
  for (const auto &s: ss) ps.push_back(s.p), ps.push_back(s.q);
  for (int i= ss.size(); i--;)
   for (int j= i; j--;)
    if (auto cp= cross_points(ss[i], ss[j]); cp.size()) ps.insert(ps.end(), cp.begin(), cp.end());
  sort(ps.begin(), ps.end()), ps.erase(unique(ps.begin(), ps.end()), ps.end());
  const int n= ps.size();
  unordered_set<int> st;
  for (const auto &s: ss) {
   vector<int> l;
   for (int i= n; i--;)
    if (s.on(ps[i])) l.push_back(i);
   sort(l.begin(), l.end(), [&](int i, int j) { return ps[i] < ps[j]; });
   for (int i= l.size(); --i;) {
    auto [u, v]= minmax(l[i], l[i - 1]);
    if (int a= (u << 16) | v; !st.count(a)) st.insert(a), dst.push_back(u), dst.push_back(v);
   }
  }
  const int m= dst.size();
  pos.resize(n + 1), g.resize(m), icf.assign(m, -1), pr.resize(m), nx.resize(m);
  for (int v: dst) ++pos[v];
  for (int v= 0; v < n; ++v) pos[v + 1]+= pos[v];
  for (int e= m; e--;) g[--pos[dst[e ^ 1]]]= e;
  vector<Point<K>> qs(m);
  for (int e= m; e--;) qs[e]= ps[dst[e]] - ps[dst[e ^ 1]];
  AngleComp<K> ac;
  auto cm= [&](int i, int j) { return ac(qs[i], qs[j]); };
  vector<int> nxi(m), pri(m);
  for (int v= n; v--;) {
   sort(g.begin() + pos[v], g.begin() + pos[v + 1], cm);
   for (int i= pos[v] + 1; i < pos[v + 1]; ++i) nxi[g[i - 1]]= g[i], pri[g[i]]= g[i - 1];
   nxi[g[pos[v + 1] - 1]]= g[pos[v]], pri[g[pos[v]]]= g[pos[v + 1] - 1];
  }
  for (int e= m; e--;) nx[e]= pri[e ^ 1], pr[e]= nxi[e] ^ 1;
  for (int e= m; e--;) {
   if (icf[e] != -1) continue;
   int f= cmp.size(), x= e;
   cmp.push_back(e);
   do {
    icf[x]= f, x= nx[x];
   } while (x != e);
  }
 }
 size_t vertex_size() const { return ps.size(); }
 size_t face_size() const { return cmp.size(); }
 const ListRange<int> out_edges(int v) const { return {g.begin() + pos[v], g.begin() + pos[v + 1]}; }
 Point<K> point(int v) const { return ps[v]; }
 int vertex(const Point<K> &p) const { return lower_bound(ps.begin(), ps.end(), p) - ps.begin(); }
 int to_v(int e) const { return dst[e]; }
 int from_v(int e) const { return dst[e ^ 1]; }
 int twin(int e) const { return e ^ 1; }
 int next(int e) const { return nx[e]; }
 int prev(int e) const { return pr[e]; }
 int incident_face(int e) const { return icf[e]; }
 int component_e(int f) const { return cmp[f]; }
 K area2(int f) const {
  K a= 0;
  for (int e= cmp[f], ne= nx[e];; ne= nx[e= ne]) {
   a+= cross(ps[dst[e]], ps[dst[ne]]);
   if (ne == cmp[f]) break;
  }
  return a;
 }
 K area(int f) const { return area2(f) / 2; }
 bool is_outside(int f) const { return area2(f) < 0; }
};
}