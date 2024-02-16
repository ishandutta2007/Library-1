#pragma once
#include "src/Internal/ListRange.hpp"
struct Edge: std::pair<int, int> {
 using std::pair<int, int>::pair;
 Edge &operator--() { return --first, --second, *this; }
 int to(int v) const { return first ^ second ^ v; }
 friend std::istream &operator>>(std::istream &is, Edge &e) { return is >> e.first >> e.second, is; }
};
struct Graph: std::vector<Edge> {
 size_t n;
 Graph(size_t n= 0, size_t m= 0): n(n), vector(m) {}
 size_t vertex_size() const { return n; }
 size_t edge_size() const { return size(); }
 size_t add_vertex() { return n++; }
 size_t add_edge(int s, int d) { return emplace_back(s, d), size() - 1; }
 size_t add_edge(Edge e) { return emplace_back(e), size() - 1; }
#define _ADJ_FOR(a, b) \
 for (auto [u, v]: *this) a; \
 for (int i= 0; i < n; ++i) p[i + 1]+= p[i]; \
 for (int i= size(); i--;) b;
#define _ADJ(a, b) \
 vector<int> p(n + 1), c(size() << !direct); \
 if (direct) { \
  _ADJ_FOR(++p[u], c[--p[(*this)[i].first]]= a) \
 } else { \
  _ADJ_FOR((++p[u], ++p[v]), (c[--p[(*this)[i].first]]= a, c[--p[(*this)[i].second]]= b)) \
 } \
 return {std::move(c), std::move(p)}
 CSRArray<int> adjacency_vertex(bool direct) const { _ADJ((*this)[i].second, (*this)[i].first); }
 CSRArray<int> adjacency_edge(bool direct) const { _ADJ(i, i); }
#undef _ADJ
#undef _ADJ_FOR
};