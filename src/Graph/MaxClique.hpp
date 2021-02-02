#pragma once
#include <bits/stdc++.h>
/**
 * @title 最大クリーク
 * @category グラフ
 * @see https://www.jstage.jst.go.jp/article/ipsjjip/25/0/25_667/_pdf
 * 最大独立集合は補グラフの最大クリーク
 * 最小頂点被覆は最大独立集合の補集合
 * 120頂点ぐらいはいける
 */

// BEGIN CUT HERE

class MaxClique {
  struct id_num {
    std::size_t id, num;
  };
  std::vector<std::bitset<128>> adj, buf;
  std::bitset<128> clique, cur;
  std::vector<int> deg;
  void dfs(std::vector<id_num>& rem) {
    if (clique.count() < cur.count()) clique = cur;
    std::sort(rem.begin(), rem.end(),
              [&](id_num l, id_num r) { return deg[l.id] > deg[r.id]; });
    std::size_t max_num = 1;
    for (auto& v : rem) {
      v.num = 0;
      while ((adj[v.id] & buf[v.num]).any()) v.num++;
      buf[v.num].set(v.id), max_num = std::max(max_num, v.num + 1);
    }
    for (int i = max_num - 1; i >= 0; i--) buf[i].reset();
    std::sort(rem.begin(), rem.end(),
              [&](id_num l, id_num r) { return l.num < r.num; });
    std::vector<id_num> nrem;
    while (!rem.empty() && rem.back().num + cur.count() >= clique.count()) {
      for (auto u : rem)
        if (adj[rem.back().id][u.id]) nrem.emplace_back(u), buf[0].set(u.id);
      for (auto u : nrem) deg[u.id] = (buf[0] & adj[u.id]).count();
      buf[0].reset(), cur.set(rem.back().id);
      dfs(nrem);
      nrem.clear(), cur.reset(rem.back().id), rem.pop_back();
    }
  }

 public:
  MaxClique(int n_) : adj(n_), buf(n_ + 1), deg(n_) {}
  void add_edge(int u, int v) { adj[u][v] = adj[v][u] = true; }
  std::vector<int> get_max_clique() {
    std::vector<id_num> nrem;
    for (int i = adj.size() - 1; i >= 0; i--)
      nrem.emplace_back(id_num{(std::size_t)i, 0}), deg[i] = adj[i].count();
    dfs(nrem);
    std::vector<int> ret;
    for (int i = 0, n = adj.size(); i < n; i++)
      if (clique[i]) ret.push_back(i);
    return ret;
  }
  std::vector<int> get_max_independent_set() {
    for (int i = adj.size() - 1; i >= 0; i--)
      for (int j = i - 1; j >= 0; j--) adj[i][j] = adj[j][i] = ~adj[j][i];
    return get_max_clique();
  }
  std::vector<int> get_min_vertex_cover() {
    get_max_independent_set();
    std::vector<int> ret;
    for (int i = 0, n = adj.size(); i < n; i++)
      if (!clique[i]) ret.push_back(i);
    return ret;
  }
};