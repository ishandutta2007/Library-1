#pragma once
#include <algorithm>
#include <string>
#include <unordered_map>
#include "src/Internal/HAS_CHECK.hpp"
template <typename M= void, std::size_t NODE_SIZE= 303030 * 4> class EulerTourTree {
 HAS_MEMBER(op);
 HAS_MEMBER(ti);
 HAS_MEMBER(mapping);
 HAS_MEMBER(composition);
 HAS_TYPE(T);
 HAS_TYPE(E);
 template <class L> using monoid= std::conjunction<has_T<L>, has_op<L>, has_ti<L>>;
 template <class L> using dual= std::conjunction<has_T<L>, has_E<L>, has_mapping<L>, has_composition<L>>;
 using node_id= std::int_least32_t;
 using vertex_id= std::int_least32_t;
 template <class U= std::nullptr_t, class F= std::nullptr_t> struct Node_B {
  using T= U;
  using E= F;
  node_id ch[2], par;
  std::uint64_t flag;
 };
 template <bool mo_, bool du_, typename tEnable= void> struct Node_D: Node_B<> {};
 template <bool mo_, bool du_> struct Node_D<mo_, du_, typename std::enable_if_t<mo_ && !du_>>: Node_B<typename M::T> { typename M::T val= M::ti(), sum= M::ti(); };
 template <bool mo_, bool du_> struct Node_D<mo_, du_, typename std::enable_if_t<!mo_ && du_>>: Node_B<typename M::T, typename M::E> {
  typename M::T val;
  typename M::E lazy;
  bool lazy_flg;
 };
 template <bool mo_, bool du_> struct Node_D<mo_, du_, typename std::enable_if_t<mo_ && du_>>: Node_B<typename M::T, typename M::E> {
  typename M::T val= M::ti(), sum= M::ti();
  typename M::E lazy;
  bool lazy_flg;
 };
 using Node= Node_D<monoid<M>::value, dual<M>::value>;
public:
 using T= typename Node::T;
 using E= typename Node::E;
private:
 static inline Node n[NODE_SIZE];
 static inline node_id ni= 1;
 node_id new_edge(int s, int d, bool hi) {
  int i= ni++, ri= ni++;
  n[i].flag= (std::uint64_t(s) << 44) | (std::uint64_t(d) << 24) | hi;
  n[ri].flag= (std::uint64_t(d) << 44) | (std::uint64_t(s) << 24);
  return i;
 }
 static void pushup(node_id i) {
  n[i].flag&= 0xffffffffff00000f;
  n[i].flag|= ((n[i].flag >> 44) == ((n[i].flag >> 24) & 0xfffff)) << 4;
  n[i].flag&= -11ll, n[i].flag|= (n[i].flag << 1) & 0b1111;
  if constexpr (monoid<M>::value) n[i].sum= n[i].val;
  if (n[i].ch[0]) {
   n[i].flag+= (n[n[i].ch[0]].flag & 0xfffff0), n[i].flag|= n[n[i].ch[0]].flag & 0b1010;
   if constexpr (monoid<M>::value) n[i].sum= M::op(n[n[i].ch[0]].sum, n[i].sum);
  }
  if (n[i].ch[1]) {
   n[i].flag+= (n[n[i].ch[1]].flag & 0xfffff0), n[i].flag|= n[n[i].ch[1]].flag & 0b1010;
   if constexpr (monoid<M>::value) n[i].sum= M::op(n[i].sum, n[n[i].ch[1]].sum);
  }
 }
 inline void propagate(node_id i, const E &v) {
  n[i].lazy_flg ? (M::composition(n[i].lazy, v), v) : n[i].lazy= v;
  if ((n[i].flag >> 44) == ((n[i].flag >> 24) & 0xfffff)) M::mapping(n[i].val, v, 1);
  if constexpr (monoid<M>::value) M::mapping(n[i].sum, v, ((n[i].flag >> 4) & 0xfffff));
  n[i].lazy_flg= true;
 }
 inline void eval(node_id i) {
  if (!n[i].lazy_flg) return;
  if (n[i].ch[0]) propagate(n[i].ch[0], n[i].lazy);
  if (n[i].ch[1]) propagate(n[i].ch[1], n[i].lazy);
  n[i].lazy_flg= false;
 }
 inline int dir(node_id i) {
  if (n[i].par) {
   if (n[n[i].par].ch[0] == i) return 0;
   if (n[n[i].par].ch[1] == i) return 1;
  }
  return 2;
 }
 inline void rot(node_id x) {
  node_id p= n[x].par;
  int d= dir(x);
  if ((n[p].ch[d]= n[x].ch[!d])) n[n[p].ch[d]].par= p;
  n[x].ch[!d]= p, pushup(p), pushup(x), n[x].par= n[p].par;
  if ((d= dir(p)) < 2) n[n[p].par].ch[d]= x, pushup(n[p].par);
  n[p].par= x;
 }
 inline void splay(node_id i) {
  if constexpr (dual<M>::value) eval(i);
  for (int i_dir= dir(i), p_dir; i_dir < 2; rot(i), i_dir= dir(i)) {
   p_dir= dir(n[i].par);
   if constexpr (dual<M>::value) {
    if (p_dir < 2) eval(n[n[i].par].par);
    eval(n[i].par), eval(i);
   }
   if (p_dir < 2) rot(i_dir == p_dir ? n[i].par : i);
  }
 }
 inline node_id merge_back(node_id l, node_id r) {
  if (!l) return r;
  if (!r) return l;
  while (n[l].ch[1]) l= n[l].ch[1];
  return splay(l), n[n[r].par= l].ch[1]= r, pushup(l), l;
 }
 inline std::pair<node_id, node_id> split(node_id i) {
  splay(i);
  node_id l= n[i].ch[0];
  return n[i].ch[0]= n[l].par= 0, pushup(i), std::make_pair(l, i);
 }
 inline void reroot(node_id v) {
  auto p= split(v);
  merge_back(p.second, p.first), splay(v);
 }
 inline bool same_root(node_id i, node_id j) {
  if (i) splay(i);
  if (j) splay(j);
  while (n[i].par) i= n[i].par;
  while (n[j].par) j= n[j].par;
  return i == j;
 }
 node_id n_st;
 std::unordered_map<std::uint64_t, node_id> emp;
public:
 EulerTourTree() {}
 EulerTourTree(int N): n_st(ni) {
  ni+= N;
  for (int i= 0; i < N; i++) n[i + n_st].flag= 0x100001000000 * i;
 }
 const T &operator[](vertex_id x) { return get(x); }
 const T &get(vertex_id x) {
  static_assert(monoid<M>::value || dual<M>::value, "\"get\" is not available\n");
  return n[x + n_st].val;
 }
 void set(vertex_id x, T val) {
  static_assert(monoid<M>::value || dual<M>::value, "\"set\" is not available\n");
  splay(x+= n_st), n[x].val= val, pushup(x);
 }
 bool edge_exist(vertex_id x, vertex_id y) {
  if (x > y) std::swap(x, y);
  return emp.count(((long long)x << 32) | y);
 }
 void link(vertex_id x, vertex_id y, bool hi= true) {
  if (x > y) std::swap(x, y);
  int ei= new_edge(x, y, hi);
  emp.insert(std::make_pair(((long long)x << 32) | y, ei));
  x+= n_st, y+= n_st, reroot(x), reroot(y);
  n[n[x].par= ei].ch[0]= x, n[n[y].par= ei].ch[1]= y;
  pushup(ei), merge_back(ei, ei + 1);
 }
 void cut(vertex_id x, vertex_id y) {
  if (x > y) std::swap(x, y);
  int ei= emp[((long long)x << 32) | y], rei= ei + 1;
  emp.erase(((long long)x << 32) | y);
  auto [pl, pr]= split(ei);
  node_id left, center, right;
  if (pl && same_root(pl, rei)) {
   auto [ql, qr]= split(rei);
   left= ql, center= n[qr].ch[1], right= n[pr].ch[1], n[center].par= 0;
  } else {
   splay(ei), n[ei= n[ei].ch[1]].par= 0;
   auto [ql, qr]= split(rei);
   splay(pl), left= pl, right= n[qr].ch[1];
  }
  n[right].par= 0, merge_back(left, right);
 }
 bool connected(vertex_id x, vertex_id y) { return same_root(x + n_st, y + n_st); }
 void subedge_set(vertex_id x, bool val) {
  splay(x+= n_st);
  if (val) n[x].flag|= 0b0100;
  else n[x].flag&= -5ll;
  pushup(x);
 }
 std::size_t tree_size(vertex_id x) { return splay(x+= n_st), ((n[x].flag >> 4) & 0xfffff); }
 T fold_tree(vertex_id x) {
  static_assert(monoid<M>::value, "\"fold\" is not available\n");
  return splay(x+= n_st), n[x].sum;
 }
 T fold_subtree(vertex_id x, vertex_id par= -1) {
  if (par == -1) return fold_tree(x);
  cut(x, par);
  T ret= fold_tree(x);
  link(x, par);
  return ret;
 }
 void apply_tree(vertex_id x, E v) {
  static_assert(dual<M>::value, "\"apply\" is not available\n");
  splay(x+= n_st), propagate(x, v), eval(x);
 }
 void apply_subtree(vertex_id x, vertex_id par, E v) { cut(x, par), apply_tree(x, v), link(x, par); }
 static std::string which_available() {
  std::string ret= "";
  if constexpr (monoid<M>::value) ret+= "\"fold\" ";
  if constexpr (dual<M>::value) ret+= "\"apply\" ";
  return ret;
 }
 template <class Func> void hilevel_edges(vertex_id v, Func f) {
  splay(v+= n_st);
  while (v && (n[v].flag & 0b0010))
   while (1) {
    if (n[v].flag & 0b0001) {
     f((n[v].flag >> 44), ((n[v].flag >> 24) & 0xfffff)), splay(v), n[v].flag&= -2ll, pushup(v);
     break;
    } else v= n[v].ch[!(n[v].ch[0] && (n[n[v].ch[0]].flag & 0b0010))];
   }
 }
 template <class Func> int subedges(vertex_id v, Func f) {
  splay(v+= n_st);
  while (v && (n[v].flag & 0b1000))
   for (bool loop= true; loop;) {
    if (n[v].flag & 0b0100) {
     if (f(n[v].flag >> 44)) return 1;
     splay(v), loop= false;
    } else v= n[v].ch[!(n[v].ch[0] && (n[n[v].ch[0]].flag & 0b1000))];
   }
  return 0;
 }
};