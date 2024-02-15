#pragma once
#include "src/Internal/ListRange.hpp"
struct Edge {
 int s, d;
 Edge(int s= 0, int d= 0): s(s), d(d) {}
 Edge &operator--() { return --s, --d, *this; }
 int to(int u) const { return u ^ s ^ d; }
 bool operator<(const Edge &e) const { return s != e.s ? s < e.s : d < e.d; }
 friend std::istream &operator>>(std::istream &is, Edge &e) { return is >> e.s >> e.d, is; }
 friend std::ostream &operator<<(std::ostream &os, const Edge &e) { return os << '(' << e.s << ", " << e.d << ')'; }
};
struct Graph: public std::vector<Edge> {
 size_t n;
 Graph(size_t n= 0, size_t m= 0): vector(m), n(n) {}
 size_t vertex_size() const { return n; }
 size_t edge_size() const { return size(); }
 int add_vertex() { return n++; }
 int add_edge(int s, int d) { return emplace_back(s, d), size() - 1; }
 int add_edge(Edge e) { return add_edge(e.s, e.d); }
#define _ADJ_FOR(a, b) \
 for (auto [u, v]: *this) a; \
 for (size_t i= 0; i < n; ++i) p[i + 1]+= p[i]; \
 for (int i= size(); i--;) b;
#define _ADJ(a, b) \
 vector<int> p(n + 1), c(size() << !direct); \
 if (direct) { \
  _ADJ_FOR(++p[u], c[--p[(*this)[i].s]]= a) \
 } else { \
  _ADJ_FOR((++p[u], ++p[v]), (c[--p[(*this)[i].s]]= a, c[--p[(*this)[i].d]]= b)) \
 } \
 return {std::move(c), std::move(p)}
 CSRArray<int> adjacency_vertex(bool direct) const { _ADJ((*this)[i].d, (*this)[i].s); }
 CSRArray<int> adjacency_edge(bool direct) const { _ADJ(i, i); }
#undef _ADJ
#undef _ADJ_FOR
};