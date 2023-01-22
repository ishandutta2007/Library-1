#pragma once
#include <vector>
#include <numeric>
#include <algorithm>
#include <limits>
#include <queue>
#include <cassert>
template <typename FlowAlgo> struct MaxFlow: public FlowAlgo {
 using FlowAlgo::FlowAlgo;
 using Edge= typename FlowAlgo::Edge;
 using flow_t= decltype(Edge::cap);
 int add_vertex() { return this->adj.resize(++this->n), this->n - 1; }
 std::vector<int> add_vertices(const std::size_t size) {
  std::vector<int> ret(size);
  std::iota(ret.begin(), ret.end(), this->n);
  return this->adj.resize(this->n+= size), ret;
 }
 struct EdgePtr {
  friend class MaxFlow;
  MaxFlow *ins;
  int v, e;
  bool bd;
  Edge &edge() { return ins->adj[v][e]; }
  Edge &rev() {
   Edge &e= edge();
   return ins->adj[e.dst][e.rev];
  }
  EdgePtr(MaxFlow *ins, int v, int e, bool d): ins(ins), v(v), e(e), bd(d) {}
 public:
  EdgePtr()= default;
  int src() const { return v; }
  int dst() { return edge().dst; }
  bool is_direct() const { return !bd; }
  flow_t flow() { return cap() - edge().cap; }
  flow_t cap() { return (edge().cap + rev().cap) / (1 + bd); }
  flow_t change_cap(flow_t new_cap, int s, int t) {
   assert(0 <= new_cap);
   Edge &e= edge(), &re= rev();
   flow_t diff= new_cap - cap(), ext= std::abs(flow()) - new_cap;
   if (ext <= 0) return e.cap+= diff, re.cap+= diff * bd, 0;
   int sr= src(), ds= dst();
   if (flow() < 0) std::swap(sr, ds);
   if (bd) {
    if (sr == src()) re.cap+= 2 * diff + e.cap, e.cap= 0;
    else e.cap+= 2 * diff + re.cap, re.cap= 0;
   } else re.cap+= diff;
   ext-= ins->maxflow(sr, ds, ext);
   if (ds != t) ins->maxflow(t, ds, ext);
   if (sr != s) ins->maxflow(sr, s, ext);
   return ext;
  }
 };
 EdgePtr add_edge(int src, int dst, flow_t cap, bool bidir= false) {
  assert(0 <= src && src < this->n), assert(0 <= dst && dst < this->n), assert(0 <= cap);
  int e= this->adj[src].size(), re= src == dst ? e + 1 : this->adj[dst].size();
  return this->adj[src].push_back(Edge{dst, re, cap}), this->adj[dst].push_back(Edge{src, e, cap * bidir}), this->m++, EdgePtr{this, src, e, bidir};
 }
 flow_t maxflow(int s, int t) { return maxflow(s, t, std::numeric_limits<flow_t>::max()); }
 flow_t maxflow(int s, int t, flow_t flow_lim) { return this->flow(s, t, flow_lim); }
 std::vector<bool> mincut(int s) {
  std::vector<bool> visited(this->n);
  static std::queue<int> que;
  for (que.push(s); !que.empty();) {
   s= que.front(), que.pop(), visited[s]= true;
   for (const auto &e: this->adj[s])
    if (e.cap && !visited[e.dst]) visited[e.dst]= true, que.push(e.dst);
  }
  return visited;
 }
};
template <typename FlowAlgo> class MaxFlowLowerBound: public FlowAlgo {
 using Edge= typename FlowAlgo::Edge;
 using flow_t= decltype(Edge::cap);
 std::vector<flow_t> in;
 int add_edge(int src, int dst, flow_t cap) {
  int e= this->adj[src].size(), re= src == dst ? e + 1 : this->adj[dst].size();
  return this->adj[src].push_back(Edge{dst, re, cap}), this->adj[dst].push_back(Edge{src, e, 0}), this->m++, re;
 }
public:
 MaxFlowLowerBound(std::size_t n= 0): FlowAlgo(n + 2), in(n) {}
 int add_vertex() { return this->adj.resize(++this->n), in.resize(this->n - 2, 0), this->n - 3; }
 std::vector<int> add_vertices(const std::size_t size) {
  std::vector<int> ret(size);
  return std::iota(ret.begin(), ret.end(), this->n - 2), this->adj.resize(this->n+= size), in.resize(this->n - 2, 0), ret;
 }
 struct EdgePtr {
  friend class MaxFlowLowerBound;
  MaxFlowLowerBound *ins;
  int v, e;
  flow_t u;
  const Edge &edge() { return ins->adj[v][e]; }
  const Edge &rev() {
   Edge &e= edge();
   return ins->adj[e.dst][e.rev];
  }
  EdgePtr(MaxFlowLowerBound *ins, int v, int e, flow_t u): ins(ins), v(v), e(e), u(u) {}
 public:
  EdgePtr()= default;
  int src() const { return v; }
  int dst() const { return edge().dst; }
  flow_t flow() const { return u - edge().cap; }
  flow_t lower() const { return flow() - rev().cap; }
  flow_t upper() const { return u; }
 };
 EdgePtr add_edge(int src, int dst, flow_t lower, flow_t upper) {
  assert(lower <= upper), src+= 2, dst+= 2, assert(0 <= src && src < this->n), assert(0 <= dst && dst < this->n), ++this->m;
  int e= this->adj[src].size(), re= src == dst ? e + 1 : this->adj[dst].size();
  if (lower * upper <= 0) this->adj[src].push_back(Edge{dst, re, upper}), this->adj[dst].push_back(Edge{src, e, -lower});
  else if (lower > 0) in[src - 2]-= lower, in[dst - 2]+= lower, this->adj[src].push_back(Edge{dst, re, upper - lower}), this->adj[dst].push_back(Edge{src, e, 0});
  else in[src - 2]-= upper, in[dst - 2]+= upper, this->adj[src].push_back(Edge{dst, re, 0}), this->adj[dst].push_back(Edge{src, e, upper - lower});
  return EdgePtr(this, src, e, upper);
 }
 flow_t maxflow(int s, int t) {
  static constexpr flow_t INF= std::numeric_limits<flow_t>::max();
  flow_t sum= 0;
  for (int i= this->n - 2; i--;) {
   if (in[i] > 0) add_edge(0, i + 2, in[i]), sum+= in[i];
   if (in[i] < 0) add_edge(i + 2, 1, -in[i]);
  }
  int re= add_edge(t+= 2, s+= 2, INF);
  if (this->flow(0, 1, INF) < sum) return -1;  // no solution
  return this->flow(s, t, INF) + this->adj[s][re].cap;
 }
};
template <class flow_t> struct Dinic {
 Dinic(std::size_t _n= 0): n(_n), m(0), adj(n) {}
protected:
 struct Edge {
  int dst, rev;
  flow_t cap;
 };
 int n, m;
 std::vector<std::vector<Edge>> adj;
 std::vector<int> level, iter;
 inline void levelize(int s, int t, int u= 0) {
  std::queue<int> que;
  for (que.push(s), level.assign(n, -1), level[s]= 0; !que.empty();) {
   u= que.front(), que.pop();
   for (auto &e: adj[u])
    if (e.cap > 0 && level[e.dst] < 0) {
     if (level[e.dst]= level[u] + 1; e.dst == t) return;
     que.push(e.dst);
    }
  }
 }
 inline flow_t dfs(int u, int s, flow_t cur, flow_t ret= 0, flow_t f= 0) {
  if (u == s) return cur;
  for (int &i= iter[u], ed= adj[u].size(); i < ed; i++) {
   Edge &e= adj[u][i], &re= adj[e.dst][e.rev];
   if (level[u] <= level[e.dst] || re.cap == 0) continue;
   if ((f= dfs(e.dst, s, std::min(cur - ret, re.cap))) <= 0) continue;
   if (e.cap+= f, re.cap-= f, ret+= f; ret == cur) return ret;
  }
  return level[u]= n, ret;
 }
 flow_t flow(int s, int t, flow_t lim, flow_t ret= 0) {
  assert(0 <= s && s < n), assert(0 <= t && t < n), assert(s != t);
  for (flow_t f; ret < lim; ret+= f) {
   if (levelize(s, t), level[t] == -1) break;
   if (iter.assign(n, 0); !(f= dfs(t, s, lim - ret))) break;
  }
  return ret;
 }
};
template <class flow_t, unsigned global_freq= 4, bool use_gap= true, bool freeze= false> struct PushRelabel {
 PushRelabel(std::size_t _n= 0): n(_n), m(0), adj(n) {}
protected:
 struct Edge {
  int dst, rev;
  flow_t cap;
 };
 int n, gap, m;
 struct {
  std::vector<int> ei, eh, oi, oh;
  int se, so;
  void init(int _n) { ei.resize(_n), eh.resize(_n), oi.resize(_n), oh.resize(_n), se= so= 0; };
  void clear() { se= so= 0; }
  inline bool empty() const { return se + so == 0; }
  void push(int i, int h) { (h & 1 ? tie(oi[so], oh[so++]) : tie(ei[se], eh[se++]))= {i, h}; }
  inline int highest() const { return max(se ? eh[se - 1] : -1, so ? oh[so - 1] : -1); }
  inline int pop() { return !se || (so && oh[so - 1] > eh[se - 1]) ? oi[--so] : ei[--se]; }
 } hque;
 std::vector<std::vector<Edge>> adj;
 std::vector<int> dist, dcnt;
 std::vector<flow_t> exc;
 inline void calc(int t) {
  if constexpr (global_freq != 0) relabel(t);
  for (int tick= m * global_freq; !hque.empty();) {
   int i= hque.pop(), dnxt= n * 2 - 1;
   if constexpr (use_gap)
    if (dist[i] > gap) continue;
   for (auto &e: adj[i])
    if (e.cap) {
     if (dist[e.dst] == dist[i] - 1) {
      if (push(i, e), exc[i] == 0) break;
     } else if (dist[e.dst] + 1 < dnxt) dnxt= dist[e.dst] + 1;
    }
   if (exc[i] > 0) {
    if constexpr (use_gap) {
     if (dnxt != dist[i] && dcnt[dist[i]] == 1 && dist[i] < gap) gap= dist[i];
     if (dnxt == gap) gap++;
     while (hque.highest() > gap) hque.pop();
     if (dnxt > gap) dnxt= n;
     if (dist[i] != dnxt) dcnt[dist[i]]--, dcnt[dnxt]++;
    }
    dist[i]= dnxt, hq_push(i);
   }
   if constexpr (global_freq != 0)
    if (--tick == 0) tick= m * global_freq, relabel(t);
  }
 }
 inline void hq_push(int i) {
  if constexpr (!use_gap) hque.push(i, dist[i]);
  else if (dist[i] < gap) hque.push(i, dist[i]);
 }
 inline void push(int i, Edge &e) {
  flow_t del= std::min(e.cap, exc[i]);
  if (exc[i]-= del, e.cap-= del, exc[e.dst]+= del, adj[e.dst][e.rev].cap+= del; 0 < exc[e.dst] && exc[e.dst] <= del) hq_push(e.dst);
 }
 inline void relabel(int t) {
  dist.assign(n, n), dist[t]= 0;
  static std::queue<int> q;
  q.push(t), hque.clear();
  if constexpr (use_gap) gap= 1, dcnt.assign(n + 1, 0);
  for (int now; !q.empty();) {
   now= q.front(), q.pop();
   if constexpr (use_gap) gap= dist[now] + 1, dcnt[dist[now]]++;
   if (exc[now] > 0) hque.push(now, dist[now]);
   for (const auto &e: adj[now])
    if (adj[e.dst][e.rev].cap && dist[e.dst] == n) dist[e.dst]= dist[now] + 1, q.push(e.dst);
  }
 }
 flow_t flow(int s, int t, flow_t lim, flow_t ret= 0) {
  assert(0 <= s && s < n), assert(0 <= t && t < n), assert(s != t), hque.init(n), exc.assign(n, 0), exc[s]+= lim, exc[t]-= lim, dist.assign(n, 0), dist[s]= n;
  if constexpr (use_gap) gap= 1, dcnt.assign(n + 1, 0), dcnt[0]= n - 1;
  for (auto &e: adj[s]) push(s, e);
  calc(t), ret= exc[t] + lim;
  if constexpr (!freeze) {
   exc[s]+= exc[t], exc[t]= 0;
   if constexpr (global_freq != 0) relabel(s);
   calc(s), assert(exc == std::vector<flow_t>(n, 0));
  }
  return ret;
 }
};