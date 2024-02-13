#pragma once
#include "src/Internal/ListRange.hpp"
struct Edge {
 int s, d;
 Edge(int s= 0, int d= 0): s(s), d(d) {}
 Edge &operator--() { return --s, --d, *this; }
 int operator-(int v) const { return s ^ d ^ v; }
 friend std::istream &operator>>(std::istream &is, Edge &e) { return is >> e.s >> e.d, is; }
 friend std::ostream &operator<<(std::ostream &os, const Edge &e) { return os << '(' << e.s << ", " << e.d << ')'; }
};
struct Graph: public std::vector<Edge> {
 std::vector<int> c, p;
 using std::vector<Edge>::vector;
 void build(int n, bool direct) {
  if (p.assign(n + 1, 0), c.resize(size() << !direct); direct) {
   for (auto [u, v]: *this) ++p[u];
   for (int i= 0; i < n; ++i) p[i + 1]+= p[i];
   for (int i= size(); i--;) c[--p[(*this)[i].s]]= i;
  } else {
   for (auto [u, v]: *this) ++p[u], ++p[v];
   for (int i= 0; i < n; ++i) p[i + 1]+= p[i];
   for (int i= size(); i--;) c[--p[(*this)[i].s]]= i, c[--p[(*this)[i].d]]= i;
  }
 }
 size_t edge_size() const { return size(); }
 size_t vertex_size() const { return p.size() - 1; }
 ListRange<int> operator()(int u) { return {c.begin() + p[u], c.begin() + p[u + 1]}; }
 ConstListRange<int> operator()(int u) const { return {c.cbegin() + p[u], c.cbegin() + p[u + 1]}; }
};