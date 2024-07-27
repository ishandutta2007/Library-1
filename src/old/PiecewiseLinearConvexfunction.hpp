#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <sstream>
#include <string>
#include <iostream>
#include <cassert>
template <std::size_t NODE_SIZE= 1 << 22> class PiecewiseLinearConvexfunction {
 using i64= long long;
 using i128= __int128_t;
 using node_id= int;
 static constexpr i64 INF= 1ll << 41;
 template <class Int> static inline std::string str(Int x) {
  if (x >= INF) return "inf";
  if (x <= -INF) return "-inf";
  std::stringstream ss;
  ss << x;
  return ss.str();
 }
 struct Node {
  node_id ch[2], par;
  int sz;
  i64 dx, slope, laz, x;
  i128 y;
 };
 friend std::ostream &operator<<(std::ostream &os, Node *t) {
  if (!t) return os << "nullptr";
  return os << "{dx:" << str(t->dx) << ",slope:" << str(t->slope) << ",x:" << str(t->x) << ",y:" << str(t->y) << ",laz:" << str(t->laz) << "}";
 }
 static inline Node ns[NODE_SIZE];
 static inline node_id ni= 1;
 static inline node_id new_node() { return ni++; }
 static inline node_id new_node(i64 dx, i64 slope) { return ns[ni].ch[0]= ns[ni].ch[1]= ns[ni].par= ns[ni].laz= 0, ns[ni].y= i128(ns[ni].x= ns[ni].dx= dx) * (ns[ni].slope= slope), ns[ni].sz= 1, ni++; }
 static inline void propagate(node_id i, i64 a) {
  if (i) ns[i].slope+= a, ns[i].laz+= a, ns[i].y+= i128(a) * ns[i].x;
 }
 static inline void push(node_id i) {
  if (ns[i].laz) propagate(ns[i].ch[0], ns[i].laz), propagate(ns[i].ch[1], ns[i].laz), ns[i].laz= 0;
 }
 static inline void update(node_id i) {
  ns[i].sz= 1, ns[i].x= ns[i].dx, ns[i].y= i128(ns[i].slope) * ns[i].dx;
  if (int j= ns[i].ch[0]; j) ns[i].sz+= ns[j].sz, ns[i].x+= ns[j].x, ns[i].y+= ns[j].y;
  if (int j= ns[i].ch[1]; j) ns[i].sz+= ns[j].sz, ns[i].x+= ns[j].x, ns[i].y+= ns[j].y;
 }
 static inline int dir(node_id i) { return ns[ns[i].par].ch[1] == i; }
 static inline void rot(node_id i) {
  node_id p= ns[i].par;
  int d= dir(i);
  if ((ns[p].ch[d]= ns[i].ch[!d])) ns[ns[p].ch[d]].par= p;
  ns[i].ch[!d]= p;
  if ((ns[i].par= ns[p].par)) ns[ns[p].par].ch[dir(p)]= i;
  ns[p].par= i, update(p);
 }
 static inline void splay(node_id i) {
  for (node_id p= ns[i].par; p; rot(i), p= ns[i].par)
   if (node_id pp= ns[p].par; pp) rot(dir(i) == dir(p) ? p : i);
  update(i);
 }
 static inline void slope_search(node_id &i, i64 k) {
  for (node_id s;; i= s) {
   push(i);
   i64 tmp= ns[i].slope;
   if (tmp == k) break;
   if (s= ns[i].ch[tmp < k]; !s) break;
  }
  splay(i);
 }
 static inline void x_search(node_id &i, i64 x) {
  for (bool c;; i= ns[i].ch[c]) {
   push(i);
   i64 l= ns[i].ch[0] ? ns[ns[i].ch[0]].x : 0, r= l + ns[i].dx;
   if (l <= x && x <= r) return splay(i);
   if ((c= (r < x))) x-= r;
  }
 }
 static inline void add(node_id i, i64 &x, i64 &p, PiecewiseLinearConvexfunction &f) {
  if (!i) return;
  push(i);
  add(ns[i].ch[0], x, p, f);
  f.add_ax_bx_c(0, ns[i].slope - p, x);
  x+= ns[i].dx, p= ns[i].slope;
  add(ns[i].ch[1], x, p, f);
 }
 static inline void debugoutput(node_id i, int d) {
  if (!i) return;
  push(i);
  debugoutput(ns[i].ch[0], d + 1);
  for (int i= 0; i < d; ++i) std::cerr << "   ";
  std::cerr << "■ " << (i ? ns + i : nullptr) << '\n';
  debugoutput(ns[i].ch[1], d + 1);
 }
 node_id root;
 i64 lx, lslope;
 i128 ly;
 void chmin_sliding_window(i64 a) {
  if (!a) return;
  assert(a > 0);
  if (root) {
   slope_search(root, 0);
   if (ns[root].slope) {
    node_id i= new_node(a, 0);
    bool c= ns[root].slope < 0;
    if ((ns[i].ch[c]= ns[root].ch[c])) ns[ns[i].ch[c]].par= i;
    update(i), ns[root].ch[c]= i, ns[i].par= root;
   } else ns[root].dx+= a;
   update(root);
  } else if (lslope >= 0) lx+= a;
  else root= new_node(a, 0);
 }
public:
 static inline void clear() { ni= 1; }
 void debugoutput() { debugoutput(root, 0); }
 i64 upper_bound() { return lx + (root ? ns[root].x : 0); }
 // f(x) := 0
 PiecewiseLinearConvexfunction(): root(0), lx(INF * 2), lslope(0), ly(0) {}
 // f(x) + c
 void add_const(i128 c) { ly+= c; }
 // f(x) + ax+c
 void add_linear(i64 a, i128 c= 0) { lslope+= a, ly+= i128(a) * lx + c, propagate(root, a); }
 // f(x-a)
 void shift(i64 a) { lx+= a; }
 // f(x) + a * max{x-c, 0}
 void add_relu(i64 a, i64 c) {
  if (!a) return;
  assert(a > 0);
  if (c < lx) {
   if (lslope < -INF) return add_linear(a, -i128(a) * c);
   node_id i= new_node(lx - c, lslope);
   if (root) x_search(root, 0), ns[root].ch[0]= i, ns[i].par= root, update(root);
   else root= i;
   ly-= ns[i].y, lx= c, propagate(root, a);
   return;
  }
  if (upper_bound() <= c) return;
  c-= lx, x_search(root, c);
  i64 l= ns[root].ch[0] ? ns[ns[root].ch[0]].x : 0, r= l + ns[root].dx;
  if (l == c) {
   node_id i= ns[root].ch[0];
   ns[root].ch[0]= 0, propagate(root, a), push(root), ns[root].ch[0]= i;
  } else if (r == c) {
   propagate(ns[root].ch[1], a);
  } else {
   node_id i= new_node(r - c, ns[root].slope);
   if ((ns[i].ch[1]= ns[root].ch[1])) ns[ns[i].ch[1]].par= i;
   update(i), propagate(i, a), ns[root].ch[1]= i, ns[i].par= root, ns[root].dx= c - l, update(root);
  }
 }
 // f(x) + a * min{x-c,0} + b * max{x-c,0}
 void add_ax_bx_c(i64 a, i64 b, i64 c) { assert(a <= b), add_linear(a, -i128(a) * c), add_relu(b - a, c); }
 // f(x) + a * |x-c|
 void add_abs(i64 a, i64 c) { add_ax_bx_c(-a, a, c); }
 // ∞ if x>a else f(x)
 void chinfty_right(i64 a= 0) {
  assert(lx <= a || lslope >= -INF);
  if (root) {
   if (a-= lx; ns[root].x <= a) return;
   x_search(root, a);
   i64 l= ns[root].ch[0] ? ns[ns[root].ch[0]].x : 0;
   if (l == a) root= ns[root].ch[0], ns[root].par= 0;
   else ns[root].dx= a - l, ns[root].ch[1]= 0, update(root);
  } else lx= a;
 }
 // ∞ if x<a else f(x)
 void chinfty_left(i64 a= 0) {
  assert(a <= upper_bound());
  if (a-= lx; a < 0) {
   if (lslope < -INF) return;
   node_id i= new_node(-a, lslope);
   if (root) x_search(root, 0), ns[root].ch[0]= i, ns[i].par= root, update(root);
   else root= i;
   ly-= ns[i].y;
  } else if (a > 0) {
   assert(root);
   x_search(root, a);
   i64 r= ns[root].dx;
   if (int i= ns[root].ch[0]; i) ly+= ns[i].y + i128(a - ns[i].x) * ns[root].slope, r+= ns[i].x;
   else ly+= i128(a) * ns[root].slope;
   if (r == a) root= ns[root].ch[1], ns[root].par= 0;
   else ns[root].dx= r - a, ns[root].ch[0]= 0, update(root);
  }
  lx+= a, lslope= -INF * 2;
 }
 // min_{y<=x} f(y)
 void cumulative_chmin() {
  assert(lslope <= 0);
  if (root) {
   slope_search(root, 0);
   if (ns[root].slope < 0) {
    if (!ns[root].ch[1]) ns[root].ch[1]= new_node();
    node_id i= ns[root].ch[1];
    ns[i].sz= 1, ns[i].x= ns[i].dx= INF * 2, ns[i].slope= ns[i].ch[0]= ns[i].ch[1]= ns[i].laz= ns[i].y= 0, ns[i].par= root;
   } else ns[root].ch[1]= 0, ns[root].dx= INF * 2, ns[root].slope= 0;
   update(root);
  } else if (lslope) root= new_node(INF * 2, 0);
  else lx= INF * 2;
 }
 // min_{y<=x ∧ y<=a} f(y)
 void cumulative_chmin_with_condition(i64 a) { chinfty_right(a), cumulative_chmin(); }
 // min_{y>=x} f(y)
 void cumulative_chmin_rev() {
  if (root) {
   slope_search(root, 0);
   i64 l= ns[root].ch[0] ? ns[ns[root].ch[0]].x : 0, r= l + ns[root].dx, x= ns[root].slope > 0 ? l : r;
   if (int i= ns[root].ch[0]; i) ly+= ns[i].y + i128(x - ns[i].x) * ns[root].slope;
   else ly+= i128(x) * ns[root].slope;
   if (ns[root].slope > 0) ns[root].ch[0]= 0, update(root);
   else root= ns[root].ch[1], ns[root].par= 0;
   lx+= x;
  }
  lslope= 0;
 }
 // min_{y>=x ∧ y>=a} f(y)
 void cumulative_chmin_rev_with_condition(i64 a) { chinfty_left(a), cumulative_chmin_rev(); }
 // min_{x-b<=y<=x-a} f(y)
 void chmin_sliding_window(i64 a, i64 b) { assert(a <= b), shift(a), chmin_sliding_window(b - a); }
 // inf_y { f(x-y) + ( a * min{y-c,0} + b * max{y-c,0} ) }
 void convex_convolution_with_ax_bx_c(i64 a, i64 b, i64 c) { assert(a <= b), shift(c), add_linear(-a), cumulative_chmin_rev(), add_linear(a), add_linear(-b), cumulative_chmin(), add_linear(b); }
 // inf_y { f(x-y) + a |y-c| }
 void convex_convolution_with_abs(i64 a, i64 c) { convex_convolution_with_ax_bx_c(-a, a, c); }
 std::array<i64, 2> argmin() {
  assert(lslope <= 0);
  if (!lslope) return std::array{-INF, lx};
  if (!root) return std::array{lx, lx};
  slope_search(root, 0);
  i64 l= lx + (ns[root].ch[0] ? ns[ns[root].ch[0]].x : 0), r= l + ns[root].dx;
  if (ns[root].slope == 0) return {l, r};
  return ns[root].slope < 0 ? std::array{r, r} : std::array{l, l};
 }
 i64 min() { return eval(argmin()[1]); }
 i64 eval(i64 x) {
  if (x < lx && lslope < -INF) return INF;
  if (x <= lx) return ly + i128(x - lx) * lslope;
  if (!root) return INF;
  if (x-= lx; ns[root].x < x) return INF;
  x_search(root, x);
  if (int i= ns[root].ch[0]; i) return ly + ns[i].y + i128(x - ns[i].x) * ns[root].slope;
  return ly + i128(x) * ns[root].slope;
 }
 i64 operator()(i64 x) { return eval(x); }
 int size() { return root ? ns[root].sz : 0; }
 // destructive
 PiecewiseLinearConvexfunction operator+(PiecewiseLinearConvexfunction &r) {
  if (size() > r.size()) std::swap(*this, r);
  if (lslope < -INF) r.chinfty_left(lx);
  else r.add_ax_bx_c(lslope, 0, lx);
  r.chinfty_right(upper_bound());
  long long x= lx, p= 0;
  add(root, x, p, r), r.add_const(ly);
  return r;
 }
 // destructive
 PiecewiseLinearConvexfunction &operator+=(PiecewiseLinearConvexfunction &r) { return *this= *this + r; }
};