/**
 * @title 木DP
 * @category グラフ
 * @brief run(root):木dp
 * @brief run_rerooting():全方位木dp
 * @see https://ei1333.hateblo.jp/entry/2018/12/21/004022
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename T, typename E = int>
struct Tree_DP {
  struct Edge {
    int to;
    E data;
    T dp, ndp;
  };
  using F = function<T(T, T)>;
  using G = function<T(T, E)>;

 private:
  vector<vector<Edge>> adj;
  vector<T> subdp, dp;
  const T init;
  const F f;
  const G g;

 private:
  void dfs_sub(int idx, int par) {
    for (auto &e : adj[idx]) {
      if (e.to == par) continue;
      dfs_sub(e.to, idx);
      subdp[idx] = f(subdp[idx], g(subdp[e.to], e.data));
    }
  }

  void dfs_all(int idx, int par, const T &top) {
    T buff{init};
    for (int i = 0; i < (int)adj[idx].size(); i++) {
      auto &e = adj[idx][i];
      e.ndp = buff;
      e.dp = g(par == e.to ? top : subdp[e.to], e.data);
      buff = f(buff, e.dp);
    }
    dp[idx] = buff;
    buff = init;
    for (int i = (int)adj[idx].size() - 1; i >= 0; i--) {
      auto &e = adj[idx][i];
      if (e.to != par) dfs_all(e.to, idx, f(e.ndp, buff));
      e.ndp = f(e.ndp, buff);
      buff = f(buff, e.dp);
    }
  }

 public:
  Tree_DP(
      int V, const F &f, T init,
      const G &g = [](const T &dp, const E &dat) { return dp; })
      : adj(V), f(f), g(g), init(init), subdp(V, init), dp(V, init) {}

  void add_edge(int src, int dst, E d = E()) {
    adj[src].emplace_back((Edge){dst, d, init, init});
  }

  T run(int root) {
    dfs_sub(root, -1);
    return subdp[root];
  }

  vector<T> run_rerooting() {
    dfs_sub(0, -1);
    dfs_all(0, -1, init);
    return dp;
  }
};