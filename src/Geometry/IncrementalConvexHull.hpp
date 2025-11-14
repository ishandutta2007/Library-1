#pragma once
#include <vector>
#include <algorithm>
#include <set>
#include "src/Geometry/Point.hpp"
namespace geo {
template <class K> class IncrementalConvexHull {
 using P= Point<K>;
 struct Lower {
  set<P> S;
  template <class A, class R> void insert(const P &p, const A &ad, const R &rm) {
   if (where(p) >= 0) return;
   S.insert(p);
   vector<P> l, r;
   auto st= S.find(p);
   for (auto it= st; it != S.begin();) {
    if (--it; l.empty()) {
     l.emplace_back(*it);
     continue;
    }
    if (sgn(cross(*it - p, l.back() - p)) > 0) break;
    rm(*it, l.back()), l.emplace_back(*it);
   }
   for (auto it= st; ++it != S.end();) {
    if (r.empty()) {
     r.emplace_back(*it);
     continue;
    }
    if (sgn(cross(r.back() - p, *it - p)) > 0) break;
    rm(r.back(), *it), r.emplace_back(*it);
   }
   if (l.size() > 1) S.erase(next(S.find(l.back())), S.find(p));
   if (l.size()) ad(l.back(), p);
   if (r.size() > 1) S.erase(next(S.find(p)), S.find(r.back()));
   if (r.size()) ad(p, r.back());
   if (l.size() && r.size()) rm(l[0], r[0]);
  }
  int where(const P &p) const {
   auto r= S.lower_bound(p);
   if (r == S.begin()) return S.size() && *r == p ? 0 : -1;
   if (r == S.end()) return -1;
   return sgn(cross(*prev(r) - p, *r - p));
  }
 } L, U;
 size_t m;
 make_long_t<K> a;
public:
 IncrementalConvexHull(): m(0), a(0) {}
 size_t edge_size() const { return m; }
 make_long_t<K> area() const { return a / 2; }
 // for integer
 make_long_t<K> area2() const { return a; }
 // +1: in, 0: on, -1: out
 int where(const P &p) const {
  int l= L.where(p), u= U.where(-p);
  return !l || !u ? 0 : min(l, u);
 }
 void insert(const P &p) {
  auto ad= [&](const P &q, const P &r) { a+= cross(q, r), ++m; };
  auto rm= [&](const P &q, const P &r) { a-= cross(q, r), --m; };
  L.insert(p, ad, rm), U.insert(-p, ad, rm);
 }
};
}