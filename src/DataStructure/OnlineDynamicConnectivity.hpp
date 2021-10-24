#pragma once
#include <bits/stdc++.h>

#include "src/DataStructure/EulerTourTree.hpp"
/**
 * @title Online-Dynamic-Connectivity
 * @category データ構造
 * @brief link,cut: O(log^2 N)
 * @brief connected: O(log N)
 */

// BEGIN CUT HERE

template <typename M = void>
class OnlineDynamicConnectivity {
  using T = typename EulerTourTree<M>::T;
  using E = typename EulerTourTree<M>::E;
  int N;
  std::vector<EulerTourTree<M>> ett;
  std::vector<std::vector<std::unordered_set<int>>> adj;
  void replace(int x, int y, int level) {
    for (int k = 0; k < level; k++) ett[k].cut(x, y);
    for (int k = level, loop = true; k-- > 0 && loop;) {
      if (ett[k].tree_size(x) > ett[k].tree_size(y)) std::swap(x, y);
      ett[k].hilevel_edges(x,
                           [&](int s, int d) { ett[k + 1].link(s, d, true); });
      ett[k].subedges(x, [&](int s) {
        for (auto itr = adj[k][s].begin(); itr != adj[k][s].end();) {
          auto d = *itr;
          if (adj[k][s].size() == 1) ett[k].subedge_set(s, 0);
          if (adj[k][d].size() == 1) ett[k].subedge_set(d, 0);
          adj[k][d].erase(s), itr = adj[k][s].erase(itr);
          if (ett[k].connected(s, d)) {
            if (adj[k + 1][s].size() == 0) ett[k + 1].subedge_set(s, 1);
            if (adj[k + 1][d].size() == 0) ett[k + 1].subedge_set(d, 1);
            adj[k + 1][s].insert(d), adj[k + 1][d].insert(s);
          } else {
            for (int kk = k + 1; kk-- > 0;) ett[kk].link(s, d, kk == k);
            return loop = false, true;
          }
        }
        return false;
      });
    }
  }

 public:
  OnlineDynamicConnectivity(int N) : N(N) {
    ett.emplace_back(N), adj.emplace_back(N);
  }
  void link(int x, int y) {
    if (ett[0].connected(x, y)) {
      if (adj[0][x].size() == 0) ett[0].subedge_set(x, 1);
      if (adj[0][y].size() == 0) ett[0].subedge_set(y, 1);
      adj[0][x].insert(y), adj[0][y].insert(x);
    } else
      ett[0].link(x, y, true);
  }
  void cut(int x, int y) {
    for (int k = 0; k < ett.size(); k++)
      if (adj[k][x].count(y)) {
        adj[k][x].erase(y), adj[k][y].erase(x);
        if (adj[k][x].size() == 0) ett[k].subedge_set(x, 0);
        if (adj[k][y].size() == 0) ett[k].subedge_set(y, 0);
        return;
      }
    for (int k = ett.size(); k-- > 0;)
      if (ett[k].edge_exist(x, y)) {
        if (k + 1 == ett.size()) ett.emplace_back(N), adj.emplace_back(N);
        replace(x, y, k + 1);
      }
  }
  const T& operator[](int x) { return ett[0][x]; }
  void set_val(int x, T val) { ett[0].set_val(x, val); }
  int size(int x) { return ett[0].tree_size(x); }
  T fold(int x) { return ett[0].fold_tree(x); }
  void apply(int x, E v) { return ett[0].apply_tree(x, v); }
  bool connected(int x, int y) { return ett[0].connected(x, y); }
};
