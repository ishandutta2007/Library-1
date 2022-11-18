#pragma once
#include <bits/stdc++.h>
/**
 * @title 重み付き最大マッチング(一般グラフ)
 * @category グラフ
 *   O(V^3)
 *  返り値:{マッチング数, 最大値, 各頂点の相方(いないなら-1）}
 */

// BEGIN CUT HERE
template <class cost_t = long long>
class MatchingWeighted {
  static constexpr cost_t INF = std::numeric_limits<cost_t>::max() / 2;
  struct E {
    int u, v;
    cost_t w;
  };
  int n, m, in;
  std::vector<std::vector<E>> G;
  std::vector<int> mt, slk, rt, par, isS, used;
  std::vector<std::vector<int>> fwr, blg;
  std::vector<cost_t> dual;
  std::queue<int> que;
  inline cost_t dist(const E &e) const { return dual[e.u] + dual[e.v] - e.w; }
  void recalc(int v, int i = 1) {
    for (slk[v] = 0; i <= n; i++)
      if (G[i][v].w && rt[i] != v && isS[rt[i]] == 1)
        if (!slk[v] || dist(G[i][v]) < dist(G[slk[v]][v])) slk[v] = i;
  }
  void push(int v) {
    if (v <= n) return que.push(v);
    for (int nxt : fwr[v]) push(nxt);
  }
  void set(int v, int r) {
    if (rt[v] = r; v > n)
      for (int nxt : fwr[v]) set(nxt, r);
  }
  int findeven(int b, int v, int p = 0) {
    if (p = std::find(fwr[b].begin(), fwr[b].end(), v) - fwr[b].begin(); p & 1)
      std::reverse(fwr[b].begin() + 1, fwr[b].end()), p = fwr[b].size() - p;
    return p;
  }
  void match(int u, int v) {
    if (mt[u] = G[u][v].v; u > n) {
      int x = blg[u][G[u][v].u], p = findeven(u, x);
      for (int i = 0; i < p; i++) match(fwr[u][i], fwr[u][i ^ 1]);
      match(x, v), rotate(fwr[u].begin(), fwr[u].begin() + p, fwr[u].end());
    }
  }
  bool path(const E &e) {
    if (int u = rt[e.u], v = rt[e.v], bu = u, bv = v, x; isS[v] == 1) {
      for (in++; bu; bu = rt[mt[bu]] ? rt[par[rt[mt[bu]]]] : 0) used[bu] = in;
      for (int i, id = n + 1; bv; bv = rt[mt[bv]] ? rt[par[rt[mt[bv]]]] : 0)
        if (used[bv] == in) {
          while (id <= m && rt[id]) id++;
          for (m += (id > m), i = 1; i <= m; i++) G[id][i].w = G[i][id].w = 0;
          std::fill_n(blg[id].begin() + 1, n, 0), mt[id] = mt[bv];
          for (fwr[id].clear(); u != bv; fwr[id].push_back(u), u = rt[par[u]])
            fwr[id].push_back(u), push(u = rt[mt[u]]);
          fwr[id].push_back(bv), std::reverse(fwr[id].begin(), fwr[id].end());
          for (; v != bv; fwr[id].push_back(v), v = rt[par[v]])
            fwr[id].push_back(v), push(v = rt[mt[v]]);
          isS[id] = 1, dual[id] = 0, set(id, id);
          for (int c : fwr[id]) {
            for (i = 1; i <= m; i++)
              if (!G[id][i].w || dist(G[c][i]) < dist(G[id][i]))
                G[id][i] = G[c][i], G[i][id] = G[i][c];
            for (i = 1; i <= n; i++)
              if (blg[c][i]) blg[id][i] = c;
          }
          return recalc(id), false;
        }
      for (bu = u, bv = v;; match(bv = x, bu = rt[par[x]]))
        if (x = rt[mt[bu]], match(bu, bv); !x)
          for (;; match(u = x, v = rt[par[x]]))
            if (x = rt[mt[v]], match(v, u); !x) return true;
    } else if (!isS[v])
      par[v] = e.u, isS[v] = 2, slk[v] = slk[x = rt[mt[v]]] = 0, isS[x] = 1,
      push(x);
    return false;
  }
  bool augment() {
    isS = slk = par = std::vector<int>(n * 2), que = std::queue<int>();
    for (int i = 1; i <= m; i++)
      if (rt[i] == i && !mt[i]) isS[i] = 1, push(i);
    if (que.empty()) return false;
    for (cost_t del = INF;; del = INF) {
      for (int v, i; !que.empty();)
        for (v = que.front(), que.pop(), i = 1; i <= n; i++)
          if (G[v][i].w && rt[i] != rt[v]) {
            if (!dist(G[v][i])) {
              if (path(G[v][i])) return true;
            } else if (isS[rt[i]] != 2)
              if (!slk[rt[i]] || dist(G[v][rt[i]]) < dist(G[slk[rt[i]]][rt[i]]))
                slk[rt[i]] = v;
          }
      for (int i = n + 1; i <= m; i++)
        if (rt[i] == i && isS[i] == 2 && del > dual[i] / 2) del = dual[i] / 2;
      for (int i = 1; i <= m; i++)
        if (rt[i] == i && slk[i] && isS[i] != 2)
          if (cost_t c = dist(G[slk[i]][i]) / (isS[i] + 1); del > c) del = c;
      for (int i = 1; i <= n; i++) {
        if (isS[rt[i]] == 1) {
          if ((dual[i] -= del) <= 0) return false;
        } else if (isS[rt[i]] == 2)
          dual[i] += del;
      }
      for (int i = n + 1; i <= m; i++)
        if (rt[i] == i && isS[i]) dual[i] += isS[i] == 1 ? 2 * del : -2 * del;
      for (int i = 1; i <= m; i++)
        if (rt[i] == i && slk[i] && rt[slk[i]] != i && !dist(G[slk[i]][i]))
          if (path(G[slk[i]][i])) return true;
      for (int b = n + 1, x, p, i, T, S; b <= m; b++)
        if (rt[b] == b && isS[b] == 2 && dual[b] == 0) {
          isS[x = blg[b][G[b][par[b]].u]] = 2, par[x] = par[b];
          for (int c : fwr[b]) set(c, c);
          for (p = findeven(b, x), i = 0, x = fwr[b].size(); i < p; i += 2)
            isS[S = fwr[b][i + 1]] = 1, isS[T = fwr[b][i]] = 2,
                               par[T] = G[S][T].u, slk[S] = slk[T] = 0, push(S);
          for (i = p + 1; i < x; i++) isS[T = fwr[b][i]] = 0, recalc(T);
          fwr[b].clear(), rt[b] = 0;
        }
    }
  }

 public:
  MatchingWeighted(int _n)
      : n(_n),
        m(n),
        in(0),
        G(n * 2, std::vector<E>(n * 2)),
        mt(n * 2),
        rt(n * 2),
        used(n * 2),
        fwr(n * 2),
        blg(n * 2, std::vector<int>(n * 2)),
        dual(n * 2) {
    for (int i = 1; i <= n; i++) rt[i] = i, blg[i][i] = i, dual[i] = INF;
    for (int i = 0; i <= n; i++)
      for (int j = 0; j <= n; j++) G[i][j] = E{i, j, 0};
  }
  void add_edge(int u, int v, cost_t w) {
    if (G[++u][++v].w < w * 2) G[u][v].w = G[v][u].w = w * 2;
  }
  std::tuple<int, cost_t, std::vector<int>> get_matching() {
    for (bool upd = augment(); upd;) upd = augment();
    std::vector<int> ret(n);
    int num = 0;
    cost_t sum = 0;
    for (int i = 1; i <= n; i++) ret[i - 1] = mt[i] - 1;
    for (int i = 0; i < n; i++)
      if (ret[i] > i) num++, sum += G[ret[i] + 1][i + 1].w;
    return std::make_tuple(num, sum / 2, ret);
  }
};