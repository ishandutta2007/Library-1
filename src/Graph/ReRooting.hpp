/**
 * @title 全方位木DP
 * @category グラフ
 * @see https://ei1333.hateblo.jp/entry/2018/12/21/004022
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename T, typename E = int>
struct ReRooting {
  struct Edge {
    int to;
    E data;
    T dp, ndp;
  };
  vector<vector<Edge>> adj;

 private:
  const function<T(T, T)> op;
  const T id;
  const function<T(T, E)> lift;
  vector<T> subdp, dp;

 private:
  void dfs_sub(int idx, int par) {
    for (auto &e : adj[idx]) {
      if (e.to == par) continue;
      dfs_sub(e.to, idx);
      subdp[idx] = op(subdp[idx], lift(subdp[e.to], e.data));
    }
  }
  void dfs_all(int idx, int par, const T &top) {
    T buff = id;
    for (int i = 0; i < (int)adj[idx].size(); i++) {
      auto &e = adj[idx][i];
      e.ndp = buff;
      e.dp = lift(par == e.to ? top : subdp[e.to], e.data);
      buff = op(buff, e.dp);
    }
    dp[idx] = buff;
    buff = id;
    for (int i = (int)adj[idx].size() - 1; i >= 0; i--) {
      auto &e = adj[idx][i];
      if (e.to != par) dfs_all(e.to, idx, op(e.ndp, buff));
      e.ndp = op(e.ndp, buff);
      buff = op(buff, e.dp);
    }
  }

 public:
  ReRooting(
      int N, const function<T(T, T)> &op_, T id_,
      const function<T(T, E)> &lift_
      = [](const T &dp, const E &dat) { return dp; })
      : adj(N), op(op_), id(id_), lift(lift_), subdp(N, id_), dp(N, id_) {}
  void add_edge(int src, int dst, E d = E()) {
    adj[src].emplace_back((Edge){dst, d, id, id});
  }
  vector<T> run() {
    dfs_sub(0, -1);
    dfs_all(0, -1, e);
    return dp;
  }
};
