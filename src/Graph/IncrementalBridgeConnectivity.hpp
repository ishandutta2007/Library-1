#pragma once
#include <utility>
#include <vector>
class IncrementalBridgeConnectivity {
 std::vector<int> cp, bp, bbf, z;
 int t;
 inline int crt(int v) { return cp[v] < 0 ? v : cp[v]= crt(cp[v]); }
 inline int par(int v) { return bbf[v] < 0 ? -1 : leader(bbf[v]); }
public:
 IncrementalBridgeConnectivity(int n): cp(n, -1), bp(n, -1), bbf(n, -1), z(n), t(0) {}
 inline int leader(int v) { return bp[v] < 0 ? v : bp[v]= leader(bp[v]); }
 int size(int v) { return -bp[leader(v)]; }
 bool two_edge_connected(int u, int v) { return leader(u) == leader(v); }
 bool connected(int u, int v) { return crt(u) == crt(v); }
 void add_edge(int u, int v) {
  int a= crt(u= leader(u)), b= crt(v= leader(v));
  if (a == b)
   for (++t, a= u, b= v;;) {
    if (z[a] == t) {
     for (int w: {u, v})
      for (int p; w= leader(w), w != a; bp[a]+= bp[w], bp[w]= a, w= p)
       if (p= bbf[w], bbf[w]= bbf[a]; bp[a] > bp[w]) std::swap(w, a);
     return;
    }
    if (z[a]= t, a= par(a); b != -1) std::swap(a, b);
   }
  if (cp[a] < cp[b]) std::swap(u, v), cp[a]+= cp[b], cp[b]= a;
  else cp[b]+= cp[a], cp[a]= b;
  for (int p; u != -1; u= p) p= par(u), bbf[u]= v, v= u;
 }
};