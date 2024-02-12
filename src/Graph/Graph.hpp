#pragma once
#include "src/Internal/ListRange.hpp"
struct Graph {
 struct Edge {
  int s, d;
  int operator-(int v) const { return s ^ d ^ v; }
  friend std::ostream &operator<<(std::ostream &os, const Edge &e) { return os << '(' << e.s << ", " << e.d << ')'; }
 };
 size_t n;
 std::vector<Edge> es;
 std::vector<int> c, p;
 Graph(int n= 0): n(n) {}
 void add_edge(int u, int v) { es.push_back({u, v}); }
 void build(bool undirect) {
  if (p.assign(n + 1, 0), c.resize(es.size() << undirect); undirect) {
   for (auto [u, v]: es) ++p[u], ++p[v];
   for (int i= 0; i < n; ++i) p[i + 1]+= p[i];
   for (int i= es.size(); i--;) c[--p[es[i].s]]= i, c[--p[es[i].d]]= i;
  } else {
   for (auto [u, v]: es) ++p[u];
   for (int i= 0; i < n; ++i) p[i + 1]+= p[i];
   for (int i= es.size(); i--;) c[--p[es[i].s]]= i;
  }
 }
 size_t edge_size() const { return es.size(); }
 size_t vertex_size() const { return n; }
 Edge &operator[](int e) { return es[e]; }
 const Edge &operator[](int e) const { return es[e]; }
 ListRange<int> operator()(int u) { return {c.begin() + p[u], c.begin() + p[u + 1]}; }
 ConstListRange<int> operator()(int u) const { return {c.cbegin() + p[u], c.cbegin() + p[u + 1]}; }
 auto begin() { return es.begin(); }
 auto begin() const { return es.cbegin(); }
 auto end() { return es.end(); }
 auto end() const { return es.cend(); }
};