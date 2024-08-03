#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <utility>
#include "src/Internal/long_traits.hpp"
namespace plc_internal {
#ifdef __LOCAL
static constexpr size_t __NODE_SIZE= 1 << 15;
#else
static constexpr size_t __NODE_SIZE= 1 << 22;
#endif
}
template <class T, size_t NODE_SIZE= plc_internal::__NODE_SIZE> class PiecewiseLinearConvex {
 using D= make_long_t<T>;
 struct Node {
  int ch[2], par;
  T z, x, d, a;
  D s;
  size_t sz;
  friend std::ostream &operator<<(std::ostream &os, const Node &t) { return os << "{z:" << t.z << ",x:" << t.x << ",d:" << t.d << ",a:" << t.a << ",s:" << t.s << ",sz:" << t.sz << ",ch:(" << t.ch[0] << "," << t.ch[1] << "),par:" << t.par << "}"; }
 };
 static inline int ni= 1;
 static inline Node n[NODE_SIZE];
 static inline void info(int t, int d, std::stringstream &ss) {
  if (!t) return;
  // push(t);
  info(n[t].ch[0], d + 1, ss);
  for (int i= 0; i < d; ++i) ss << "   ";
  ss << " ■ " << n[t] << '\n', info(n[t].ch[1], d + 1, ss);
 }
 static inline void dump_xs(int t, std::vector<T> &xs) {
  if (t) push(t), dump_xs(n[t].ch[0], xs), xs.push_back(n[t].x), dump_xs(n[t].ch[1], xs);
 }
 static inline void dump_slopes_l(int t, T ofs, std::vector<T> &as) {
  if (t) push(t), dump_slopes_l(n[t].ch[1], ofs, as), ofs+= n[n[t].ch[1]].a + n[t].d, as.push_back(-ofs), dump_slopes_l(n[t].ch[0], ofs, as);
 }
 static inline void dump_slopes_r(int t, T ofs, std::vector<T> &as) {
  if (t) push(t), dump_slopes_r(n[t].ch[0], ofs, as), ofs+= n[n[t].ch[0]].a + n[t].d, as.push_back(ofs), dump_slopes_r(n[t].ch[1], ofs, as);
 }
 static inline void update(int t) {
  n[t].sz= 1, n[t].a= n[t].d, n[t].s= D(n[t].x) * n[t].d;
  if (int l= n[t].ch[0]; l) n[t].sz+= n[l].sz, n[t].a+= n[l].a, n[t].s+= n[l].s;
  if (int r= n[t].ch[1]; r) n[t].sz+= n[r].sz, n[t].a+= n[r].a, n[t].s+= n[r].s;
 }
 static inline void prop(int t, T v) { n[t].z+= v, n[t].s+= D(v) * n[t].a, n[t].x+= v; }
 static inline void push(int t) {
  if (n[t].z != 0) {
   if (n[t].ch[0]) prop(n[t].ch[0], n[t].z);
   if (n[t].ch[1]) prop(n[t].ch[1], n[t].z);
   n[t].z= 0;
  }
 }
 static inline void rot(int t) {
  int p= n[t].par;
  if (bool d= n[p].ch[1] == t; (n[p].ch[d]= std::exchange(n[t].ch[!d], p))) n[n[p].ch[d]].par= p;
  if ((n[t].par= std::exchange(n[p].par, t))) n[n[t].par].ch[n[n[t].par].ch[1] == p]= t;
  update(p);
 }
 static inline void splay(int t) {
  for (int p= n[t].par; p; rot(t), p= n[t].par)
   if (n[p].par) rot(n[n[p].par].ch[n[p].ch[1] == t] == p ? p : t);
 }
 template <bool r> static inline bool lt(T a, T b) {
  if constexpr (r) return b < a;
  else return a < b;
 }
 template <bool r> inline D calc_y(int t, T x, T ol, T ou) {
  if (t) {
   for (int s;; t= s) {
    if (push(t); lt<r>(n[t].x, x)) s= n[t].ch[!r];
    else {
     ol+= n[n[t].ch[!r]].a, ou+= n[n[t].ch[!r]].s;
     if (n[t].x == x) break;
     ol+= n[t].d, ou+= D(n[t].x) * n[t].d, s= n[t].ch[r];
    }
    if (!s) break;
   }
   splay(t), splay(mn);
  }
  if constexpr (r) return D(x) * ol - ou;
  else return ou - D(x) * ol;
 }
 int mn;
 bool bf[2];
 T o[2], rem, bx[2];
 D y;
 D calc_y(T x) {
  if (!mn) return 0;
  if (n[mn].x == x) return 0;
  return push(mn), x < n[mn].x ? calc_y<0>(n[mn].ch[0], x, o[0], D(n[mn].x) * o[0]) : calc_y<1>(n[mn].ch[1], x, o[1], D(n[mn].x) * o[1]);
 }
 void slope_lr(bool r) {
  int t= mn;
  if (!t) return;
  for (; push(t), n[t].ch[r];) t= n[t].ch[r];
  D p= n[n[mn].ch[r]].s + D(n[mn].x) * o[r];
  T q= o[r] + n[n[mn].ch[r]].a;
  splay(mn= t), o[r]= 0, o[!r]= n[t].d, r ? (y-= p, rem+= q) : (y+= p, rem-= q);
 }
 void slope_eval() {
  if (rem == 0 || !mn) return;
  bool neg= rem < 0;
  T p= neg ? -rem : rem, ol= 0;
  D ou= 0;
  int t= mn;
  if (push(t), ol= o[neg]; p <= ol) {
   o[neg]-= p, o[!neg]+= p, y+= D(n[t].x) * rem, rem= 0;
   return;
  }
  if (ou+= D(n[t].x) * ol, t= n[t].ch[neg]; ol + n[t].a < p) return slope_lr(neg);
  for (T s, l;;) {
   if (push(t), s= ol + n[n[t].ch[!neg]].a, l= s + n[t].d; p < s) t= n[t].ch[!neg];
   else if (l < p) ol= l, ou+= n[n[t].ch[!neg]].s + D(n[t].x) * n[t].d, t= n[t].ch[neg];
   else {
    if (o[neg]= l - p, o[!neg]= p - s; neg) y+= D(n[t].x) * s - (ou + n[n[t].ch[!neg]].s);
    else y-= D(n[t].x) * s - (ou + n[n[t].ch[!neg]].s);
    break;
   }
  }
  splay(mn= t), y+= D(n[t].x) * rem, rem= 0;
 }
 template <bool r> void add_inf(T x0) {
  if (bf[r] && !lt<r>(bx[r], x0)) return;
  if (assert(!bf[!r] || !lt<r>(bx[!r], x0)), bf[r]= true, bx[r]= x0; !mn) return;
  if (slope_lr(!r); !lt<r>(x0, n[mn].x)) return mn= 0, void();
  int t= mn, s= t;
  for (; t;)
   if (push(t); lt<r>(x0, n[t].x)) s= t, t= n[t].ch[r];
   else t= n[t].ch[!r];
  splay(s), n[s].ch[r]= 0, splay(mn);
 }
 void add_r(int t) {
  if (t) push(t), add_r(n[t].ch[0]), add_max(0, n[t].d, n[t].x), add_r(n[t].ch[1]);
 }
 void add_l(int t) {
  if (t) push(t), add_l(n[t].ch[0]), add_max(-n[t].d, 0, n[t].x), add_l(n[t].ch[1]);
 }
public:
 PiecewiseLinearConvex(): mn(0), bf{0, 0}, rem(0), y(0) {}
 static void clear() { ni= 1; }
 std::string info() {
  std::stringstream ss;
  if (ss << "\n rem:" << rem << ", y:" << y << ", mn:" << mn << "\n bf[0]:" << bf[0] << ", bf[1]:" << bf[1] << ", bx[0]:" << bx[0] << ", bx[1]:" << bx[1] << "\n " << "o[0]:" << o[0] << ", o[1]:" << o[1] << "\n"; mn) info(mn, 0, ss);
  return ss.str();
 }
 std::vector<T> dump_xs() {
  std::vector<T> xs;
  if (bf[0]) xs.push_back(bx[0]);
  if (mn) dump_xs(mn, xs);
  if (bf[1]) xs.push_back(bx[1]);
  return xs;
 }
 std::vector<std::pair<T, D>> dump_xys() {
  auto xs= dump_xs();
  std::vector<std::pair<T, D>> xys(xs.size());
  for (int i= xs.size(); i--;) xys[i]= {xs[i], operator()(xs[i])};
  return xys;
 }
 std::vector<T> dump_slopes() {
  std::vector<T> as;
  if (mn) as.push_back(-o[0]), dump_slopes_l(n[mn].ch[0], o[0], as), std::reverse(as.begin(), as.end()), as.push_back(o[1]), dump_slopes_r(n[mn].ch[1], o[1], as);
  else as.push_back(0);
  for (auto &a: as) a+= rem;
  return as;
 }
 // f(x) += c
 void add_const(D c) { y+= c; }
 // f(x) += ax, /
 void add_linear(T a) { rem+= a; }
 //  f(x) += max(a(x-x0),b(x-x0)), (a < b)
 void add_max(T a, T b, T x0) {
  assert(a < b);
  if (bf[0] && x0 <= bx[0]) y-= D(b) * x0, rem+= b;
  else if (bf[1] && bx[1] <= x0) y-= D(a) * x0, rem+= a;
  else if (mn) {
   int t= mn;
   for (;;) {
    if (push(t); n[t].x == x0) {
     n[t].d+= b - a;
     break;
    }
    int &s= n[t].ch[n[t].x < x0];
    if (!s) {
     n[s= ni++]= Node{{0, 0}, t, 0, x0, b - a, b - a, D(x0) * (b - a), 1}, t= s;
     break;
    }
    t= s;
   }
   if (splay(t), splay(mn); x0 < n[mn].x) y-= D(b) * x0, rem+= b;
   else if (y-= D(a) * x0, rem+= a; x0 == n[mn].x) o[1]+= b - a;
  } else n[mn= ni++]= Node{{0, 0}, 0, 0, x0, b - a, b - a, D(x0) * (b - a), 1}, y-= D(a) * x0, rem+= a, o[0]= 0, o[1]= b - a;
 }
 // f(x) +=  max(0, a(x-x0))
 void add_ramp(T a, T x0) {
  if (a != 0) a > 0 ? add_max(0, a, x0) : add_max(a, 0, x0);
 }
 // f(x) += a|x-x0|, \/
 void add_abs(T a, T x0) {
  if (assert(a >= 0); a != 0) add_max(-a, a, x0);
 }
 // right=false : f(x) +=  inf  (x < x_0), right=true: f(x) += inf  (x_0 < x)
 void add_inf(bool right= false, T x0= 0) { return right ? add_inf<1>(x0) : add_inf<0>(x0); }
 // f(x) <- f(x-x0)
 void shift(T x0) {
  if (bx[0]+= x0, bx[1]+= x0, y-= D(rem) * x0; mn) n[mn].z+= x0, n[mn].x+= x0;
 }
 // rev=false: f(x) <- min_{y<=x} f(y), rev=true : f(x) <- min_{x<=y} f(y)
 void chmin_cum(bool rev= false) {
  if (bf[0] && bf[1] && bx[0] == bx[1]) y+= D(rem) * bx[0], rem= 0;
  else if (slope_eval(); rem == 0) {
   if (mn) n[mn].d= o[rev], o[!rev]= 0, n[mn].ch[!rev]= 0;
  } else if ((rem > 0) ^ rev) assert(bf[rev]), y+= D(rem) * bx[rev], rem= 0, mn= 0;
  else if (bf[!rev]) {
   T p= std::abs(rem);
   int t= ni++;
   n[t]= Node{{0, 0}, 0, 0, bx[!rev], p, p, D(bx[!rev]) * p, 1};
   if (mn) update(mn), n[t].ch[rev]= mn, n[mn].par= t;
   mn= t, o[rev]= p, o[!rev]= 0, y+= D(rem) * bx[!rev], rem= 0;
  }
  bf[!rev]= false;
 }
 //  f(x) <- min_{lb<=y<=ub} f(x-y). (lb <= ub), \_/ -> \__/
 void chmin_slide_win(T lb, T ub) {
  assert(lb <= ub);
  if (bf[0] && bf[1] && bx[0] == bx[1]) y+= D(rem) * bx[0], rem= 0;
  else if (slope_eval(); rem == 0) {
   if (mn) {
    if (o[0] == 0) {
     if (n[mn].z+= ub, n[mn].x+= ub; n[mn].ch[0]) prop(n[mn].ch[0], lb - ub);
    } else if (o[1] == 0) {
     if (n[mn].z+= lb, n[mn].x+= lb; n[mn].ch[1]) prop(n[mn].ch[1], ub - lb);
    } else {
     int r= n[mn].ch[1], t= ni++;
     n[t]= Node{{0, r}, mn, 0, n[mn].x, o[1], 0, 0, 1};
     if (update(t), prop(n[mn].ch[1]= t, ub - lb), n[mn].d= o[0], o[1]= 0, n[mn].z+= lb, n[mn].x+= lb; r) n[r].par= t;
    }
   }
  } else {
   bool r= rem > 0;
   T b[2]= {lb, ub};
   if (bf[!r]) {
    T p= r ? rem : -rem;
    int t= ni++;
    n[t]= Node{{0, 0}, 0, 0, bx[!r], p, p, D(bx[!r]) * p, 1};
    if (mn) update(mn), n[t].ch[r]= mn, n[mn].par= t;
    y+= D(rem) * bx[!r], rem= 0, mn= t, n[t].z+= b[r], n[t].x+= b[r], o[r]= p, o[!r]= 0;
   } else if (y-= D(rem) * b[r]; mn) n[mn].z+= b[r], n[mn].x+= b[r];
  }
  bx[0]+= lb, bx[1]+= ub;
 }
 D operator()(T x) { return assert(!bf[0] || bx[0] <= x), assert(!bf[1] || x <= bx[1]), calc_y(x) + D(rem) * x + y; }
 D min() { return slope_eval(), rem == 0 ? y : rem > 0 ? (assert(bf[0]), y + D(rem) * bx[0]) : (assert(bf[1]), y + D(rem) * bx[1]); }
 std::array<T, 2> argmin() {
  slope_eval();
  if (rem > 0) {
   assert(bf[0]);
   return {bx[0], bx[0]};
  }
  if (rem < 0) {
   assert(bf[1]);
   return {bx[1], bx[1]};
  }
  std::array<T, 2> ret= {bx[0], bx[1]};
  int t= mn;
  if (!t) return ret;
  bool r= o[0] == 0;
  if (!r && o[1] != 0) ret[0]= ret[1]= n[t].x;
  else if (ret[r]= n[t].x, t= n[t].ch[!r]; t) {
   for (; n[t].ch[r];) push(t), t= n[t].ch[r];
   splay(t), ret[!r]= n[t].x, splay(mn);
  } else assert(bf[!r]);
  return ret;
 }
 size_t size() { return mn ? update(mn), n[mn].sz : 0; }
 PiecewiseLinearConvex &operator+=(const PiecewiseLinearConvex &r) {
  if (y+= r.y, rem+= r.rem; r.bf[0]) add_inf(false, r.bx[0]);
  if (r.bf[1]) add_inf(true, r.bx[1]);
  if (r.mn) add_l(n[r.mn].ch[0]), add_r(n[r.mn].ch[1]), add_max(-r.o[0], r.o[1], n[r.mn].x);
  return *this;
 }
};
