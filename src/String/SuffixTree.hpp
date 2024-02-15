#pragma once
#include "src/String/SuffixArray.hpp"
#include "src/Graph/HeavyLightDecomposition.hpp"
#include "src/Misc/CartesianTree.hpp"
struct SuffixTree {
 Graph graph;
 HeavyLightDecomposition tree;
 std::vector<std::tuple<int, int, int, int>> node;
 std::vector<int> suf;
 template <class String> SuffixTree(const SuffixArray<String> &sa, const LCPArray &lcp): suf(sa.size()) {
  const int n= sa.size();
  node.emplace_back(0, n, 0, 0);
  if (n == 1) {
   graph.add_edge(0, 1), graph.build(2, 0), tree= HeavyLightDecomposition(graph), node.emplace_back(0, 1, 0, 1), suf[0]= 1;
   return;
  }
  CartesianTree ct(lcp);
  auto dfs= [&](auto dfs, int p, int idx, int h) -> void {
   auto [l, r]= ct.range(idx);
   ++r;
   int hh= lcp[idx];
   if (h < hh) graph.add_edge(p, node.size()), p= node.size(), node.emplace_back(l, r, h, hh);
   auto [lch, rch]= ct.children(idx);
   if (lch == -1) {
    if (hh < n - sa[idx]) graph.add_edge(p, node.size()), suf[sa[idx]]= node.size(), node.emplace_back(idx, idx + 1, hh, n - sa[idx]);
    else suf[sa[idx]]= p;
   } else dfs(dfs, p, lch, hh);
   if (rch == -1) {
    if (hh < n - sa[idx + 1]) graph.add_edge(p, node.size()), suf[sa[idx + 1]]= node.size(), node.emplace_back(idx + 1, idx + 2, hh, n - sa[idx + 1]);
    else suf[sa[idx + 1]]= p;
   } else dfs(dfs, p, rch, hh);
  };
  if (int r= ct.root(); lcp[r] > 0) graph.add_edge(0, 1), node.emplace_back(0, n, 0, lcp[r]), dfs(dfs, 1, r, lcp[r]);
  else dfs(dfs, 0, r, 0);
  graph.n= node.size(), tree= HeavyLightDecomposition(graph.adjecency_vertex(1), 0);
 }
 int size() const { return node.size(); }
 auto &operator[](int i) const { return node[i]; }
 auto begin() const { return node.begin(); }
 auto end() const { return node.end(); }
 int substr(int l) const { return suf[l]; }
 int substr(int l, int n) const {
  for (int v= suf[l], u, w;; v= w)
   if (u= tree.head(v), w= tree.parent(u); w == -1 || std::get<3>(node[w]) < n) {
    int ok= tree.to_seq(v), ng= tree.to_seq(u) - 1;
    for (int m; ok - ng > 1;) m= (ok + ng) / 2, (n <= std::get<3>(node[tree.to_vertex(m)]) ? ok : ng)= m;
    return tree.to_vertex(ok);
   }
 }
 template <class String> std::string debug_output(const SuffixArray<String> &sa) const {
  std::string res= "\n";
  for (int i= 0; i < node.size(); ++i) {
   auto [l, r, h, hh]= node[i];
   res+= std::to_string(i) + ": (" + std::to_string(l) + "," + std::to_string(r) + "," + std::to_string(h) + "," + std::to_string(hh) + ") ";
   res+= sa.s.substr(sa[l] + h, hh - h);
   res+= "\n";
  }
  for (int i= 0; i < sa.size(); ++i) {
   res+= " " + sa.s.substr(sa[i]) + "\n";
  }
  return res;
 }
};