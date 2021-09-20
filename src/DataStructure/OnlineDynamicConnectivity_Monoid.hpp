#pragma once
#include <bits/stdc++.h>

#include "src/DataStructure/EulerTourTree_Monoid.hpp"
/**
 * @title Euler-Tour-Tree(モノイド)
 * @category データ構造
 * @brief O(logN)
 */

// BEGIN CUT HERE

template <typename M>
struct OnlineDynamicConnectivity_Monoid {
  using T = typename M::T;
  int N;
  std::vector<EulerTourTree_Monoid<M>> ett;
  std::vector<std::vector<std::unordered_set<int>>> E;

  OnlineDynamicConnectivity_Monoid(int N) : N(N) {
    ett.emplace_back(N), E.emplace_back(N);
  }

  void link(int x, int y) {
    if (ett[0].connected(x, y)) {
      if (E[0][x].size() == 0) ett[0].subedge_set(x, 1);
      if (E[0][y].size() == 0) ett[0].subedge_set(y, 1);
      E[0][x].insert(y), E[0][y].insert(x);
    } else
      ett[0].link(x, y, true);
  }

  void replace(int x, int y, int level) {
    for (int k = 0; k < level; k++) ett[k].cut(x, y);
    for (int k = level, loop = true; k-- > 0 && loop;) {
      if (ett[k].tree_size(x) > ett[k].tree_size(y)) std::swap(x, y);
      ett[k].hilevel_edges(x,
                           [&](int s, int d) { ett[k + 1].link(s, d, true); });
      ett[k].subedges(x, [&](int s) {
        for (auto itr = E[k][s].begin(); itr != E[k][s].end();) {
          auto d = *itr;
          if (E[k][s].size() == 1) ett[k].subedge_set(s, 0);
          if (E[k][d].size() == 1) ett[k].subedge_set(d, 0);
          E[k][d].erase(s), itr = E[k][s].erase(itr);
          if (ett[k].connected(s, d)) {
            if (E[k + 1][s].size() == 0) ett[k + 1].subedge_set(s, 1);
            if (E[k + 1][d].size() == 0) ett[k + 1].subedge_set(d, 1);
            E[k + 1][s].insert(d), E[k + 1][d].insert(s);
          } else {
            for (int kk = k + 1; kk-- > 0;) ett[kk].link(s, d, kk == k);
            return loop = false, true;
          }
        }
        return false;
      });
    }
  }

  void cut(int x, int y) {
    for (int k = 0; k < ett.size(); k++)
      if (E[k][x].count(y)) {
        E[k][x].erase(y), E[k][y].erase(x);
        if (E[k][x].size() == 0) ett[k].subedge_set(x, 0);
        if (E[k][y].size() == 0) ett[k].subedge_set(y, 0);
        return;
      }
    for (int k = ett.size(); k-- > 0;)
      if (ett[k].edge_exist(x, y)) {
        if (k + 1 == ett.size()) ett.emplace_back(N), E.emplace_back(N);
        replace(x, y, k + 1);
      }
  }
  T operator[](int x) const { return ett[0][x]; }
  void set_val(int x, T val) { ett[0].set_val(x, val); }
  int size(int x) { return ett[0].tree_size(x); }
  long long fold(int x) { return ett[0].tree_fold(x); }
  bool connected(int x, int y) { return ett[0].connected(x, y); }
};