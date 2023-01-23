#pragma once
#include "src/Graph/StronglyConnectedComponents.hpp"
struct TwoSatisfiability {
private:
 int sz;
 StronglyConnectedComponents scc;
public:
 TwoSatisfiability(int n): sz(n), scc(2 * n) {}
 void add_if(int u, int v) {
  scc.add_edge(u, v);
  scc.add_edge(neg(v), neg(u));
 }                                                   // u -> v <=> !v -> !u
 void add_or(int u, int v) { add_if(neg(u), v); }    // u or v <=> !u -> v
 void add_nand(int u, int v) { add_if(u, neg(v)); }  // u nand v <=> u -> !v
 void set_true(int u) { scc.add_edge(neg(u), u); }   // u <=> !u -> u
 void set_false(int u) { scc.add_edge(u, neg(u)); }  // !u <=> u -> !u
 inline int neg(int x) { return x >= sz ? x - sz : x + sz; }
 std::vector<short> solve() {
  std::vector<int> I= scc.get_SCC().second;
  std::vector<short> ret(sz);
  for (int i= 0; i < sz; i++) {
   if (I[i] == I[neg(i)]) return {};
   ret[i]= I[i] > I[neg(i)];
  }
  return ret;
 }
};
