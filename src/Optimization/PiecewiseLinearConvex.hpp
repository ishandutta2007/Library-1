#pragma once
#include <vector>
#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "src/Internal/long_traits.hpp"
template <class T> class PiecewiseLinearConvex {
 using D= make_long_t<T>;
 struct Node {
  Node *ch[2], *par;
  T z, x, d, a;
  D s;
  size_t sz;
  friend std::ostream &operator<<(std::ostream &os, const Node &t) { return os << "{z:" << t.z << ",x:" << t.x << ",d:" << t.d << ",a:" << t.a << ",s:" << t.s << ",sz:" << t.sz << ",ch:(" << t.ch[0] << "," << t.ch[1] << "),par:" << t.par << "}"; }
 };
 using np= Node *;
 static inline void info(np t, int d, std::stringstream &ss) {
  if (!t) return;
  push(t), info(t->ch[0], d + 1, ss);
  for (int i= 0; i < d; ++i) ss << "   ";
  ss << " ■ " << *t << '\n', info(t->ch[1], d + 1, ss);
 }
 static inline void dump_xs(np t, std::vector<T> &xs) {
  if (t) push(t), dump_xs(t->ch[0], xs), xs.push_back(t->x), dump_xs(t->ch[1], xs);
 }
 static inline void dump_slopes_l(np t, T ofs, std::vector<T> &as) {
  if (t) push(t), dump_slopes_l(t->ch[1], ofs, as), ofs+= sl(t->ch[1]) + t->d, as.push_back(-ofs), dump_slopes_l(t->ch[0], ofs, as);
 }
 static inline void dump_slopes_r(np t, T ofs, std::vector<T> &as) {
  if (t) push(t), dump_slopes_r(t->ch[0], ofs, as), ofs+= sl(t->ch[0]) + t->d, as.push_back(ofs), dump_slopes_r(t->ch[1], ofs, as);
 }
 static inline void update(np t) {
  t->sz= 1, t->a= t->d, t->s= D(t->x) * t->d;
  if (np l= t->ch[0]; l) t->sz+= l->sz, t->a+= l->a, t->s+= l->s;
  if (np r= t->ch[1]; r) t->sz+= r->sz, t->a+= r->a, t->s+= r->s;
 }
 static inline void prop(np t, T v) { t->z+= v, t->s+= D(v) * t->a, t->x+= v; }
 static inline void push(np t) {
  if (t->z) {
   if (np l= t->ch[0]; l) prop(l, t->z);
   if (np r= t->ch[1]; r) prop(r, t->z);
   t->z= 0;
  }
 }
 static inline void rot(np t) {
  np p= t->par;
  if (bool d= p->ch[1] == t; (p->ch[d]= std::exchange(t->ch[!d], p))) p->ch[d]->par= p;
  if ((t->par= std::exchange(p->par, t))) t->par->ch[t->par->ch[1] == p]= t;
  update(p);
 }
 static inline void splay(np t) {
  for (np p= t->par; p; rot(t), p= t->par)
   if (p->par) rot(p->par->ch[p->ch[1] == t] == p ? p : t);
  update(t);
 }
 static inline void splay_p(np t) {
  if (np p= t->par; p) do {
    if (p->par) push(p->par), push(p), push(t), rot(p->par->ch[p->ch[1] == t] == p ? p : t);
    else push(p), push(t);
    rot(t), p= t->par;
   } while (p);
  else push(t);
  update(t);
 }
 static inline T sl(np t) { return t ? t->a : 0; }
 static inline D sum(np t) { return t ? t->s : 0; }
 template <bool r> static inline bool lt(T a, T b) {
  if constexpr (r) return b < a;
  else return a < b;
 }
 template <bool r> static inline D calc_y(np t, T x, T ol, T ou) {
  for (np n;; t= n) {
   push(t);
   if (lt<r>(t->x, x)) n= t->ch[!r];
   else {
    ol+= sl(t->ch[!r]), ou+= sum(t->ch[!r]);
    if (t->x == x) break;
    ol+= t->d, ou+= D(t->x) * t->d, n= t->ch[r];
   }
   if (!n) break;
  }
  splay(t);
  if constexpr (r) return D(x) * ol - ou;
  else return ou - D(x) * ol;
 }
 np mn;
 bool bf[2];
 T o[2], rem, bx[2];
 D y;
 D calc_y(T x) {
  if (!mn) return 0;
  if (mn->x == x) return 0;
  splay_p(mn);
  if (x < mn->x) return mn->ch[0] ? calc_y<0>(mn->ch[0], x, o[0], D(mn->x) * o[0]) : D(mn->x - x) * o[0];
  else return mn->ch[1] ? calc_y<1>(mn->ch[1], x, o[1], D(mn->x) * o[1]) : D(x - mn->x) * o[1];
 }
 template <bool r> void slope_lr() {
  np t= mn;
  if (!t) return;
  splay_p(t);
  T ol= o[r];
  if constexpr (r) y-= sum(t->ch[r]) + D(t->x) * ol, rem+= ol + sl(t->ch[r]);
  else y+= sum(t->ch[r]) + D(t->x) * ol, rem-= ol + sl(t->ch[r]);
  for (; t->ch[r];) push(t), t= t->ch[r];
  mn= t, o[r]= 0, o[!r]= t->d;
 }
 void slope_eval() {
  if (rem == 0 || !mn) return;
  bool neg= rem < 0;
  T p= neg ? -rem : rem, ol= 0;
  D ou= 0;
  np t= mn;
  if (ol= o[neg]; p <= ol) {
   o[neg]-= p, o[!neg]+= p, y+= D(t->x) * rem, rem= 0;
   return;
  }
  splay_p(t);
  ou+= D(t->x) * ol;
  t= t->ch[neg];
  if (ol + sl(t) < p) return neg ? slope_lr<1>() : slope_lr<0>();
  for (;;) {
   push(t);
   T s= ol + sl(t->ch[!neg]), l= s + t->d;
   if (p < s) t= t->ch[!neg];
   else if (l < p) ol= l, ou+= sum(t->ch[!neg]) + D(t->x) * t->d, t= t->ch[neg];
   else {
    y+= D(t->x) * rem, rem= 0;
    if (neg) y+= D(t->x) * s - (ou + sum(t->ch[!neg]));
    else y-= D(t->x) * s - (ou + sum(t->ch[!neg]));
    mn= t, o[neg]= l - p, o[!neg]= p - s;
    break;
   }
  }
 }
 template <bool r> void add_inf(T x0) {
  if (bf[r] && !lt<r>(bx[r], x0)) return;
  assert(!bf[!r] || !lt<r>(bx[!r], x0));
  bf[r]= true, bx[r]= x0;
  if (!mn) return;
  slope_lr<!r>();
  if (!lt<r>(x0, mn->x)) {
   mn= nullptr;
   return;
  }
  splay_p(mn);
  np t= mn, s= t;
  for (; t;)
   if (push(t); lt<r>(x0, t->x)) s= t, t= t->ch[r];
   else t= t->ch[!r];
  splay(s), s->ch[r]= nullptr;
  update(s);
 }
 void add_r(np t) {
  if (t) push(t), add_r(t->ch[0]), add_max(0, t->d, t->x), add_r(t->ch[1]);
 }
 void add_l(np t) {
  if (t) push(t), add_l(t->ch[0]), add_max(-t->d, 0, t->x), add_l(t->ch[1]);
 }
public:
 PiecewiseLinearConvex(): mn(nullptr), bf{0, 0}, rem(0), y(0) {}
 std::string info() {
  std::stringstream ss;
  ss << "\n rem:" << rem << ", y:" << y << ", mn:" << mn << "\n bf[0]:" << bf[0] << ", bf[1]:" << bf[1] << ", bx[0]:" << bx[0] << ", bx[1]:" << bx[1] << "\n " << "o[0]:" << o[0] << ", o[1]:" << o[1] << "\n";
  if (mn) splay_p(mn), info(mn, 0, ss);
  return ss.str();
 }
 std::vector<T> dump_xs() {
  std::vector<T> xs;
  if (bf[0]) xs.push_back(bx[0]);
  if (mn) splay_p(mn), dump_xs(mn, xs);
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
  if (mn) splay_p(mn), as.push_back(-o[0]), dump_slopes_l(mn->ch[0], o[0], as), std::reverse(as.begin(), as.end()), as.push_back(o[1]), dump_slopes_r(mn->ch[1], o[1], as);
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
   np t= mn;
   for (splay_p(t);;) {
    if (t->x == x0) {
     t->d+= b - a;
     break;
    }
    push(t);
    np &n= t->ch[t->x < x0];
    if (!n) {
     n= new Node{{nullptr, nullptr}, t, 0, x0, b - a, b - a, D(x0) * (b - a), 1}, t= n;
     break;
    }
    t= n;
   }
   if (splay(t); x0 < mn->x) y-= D(b) * x0, rem+= b;
   else if (y-= D(a) * x0, rem+= a; x0 == mn->x) o[1]+= b - a;
  } else mn= new Node{{nullptr, nullptr}, nullptr, 0, x0, b - a, b - a, D(x0) * (b - a), 1}, y-= D(a) * x0, rem+= a, o[0]= 0, o[1]= b - a;
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
  if (bx[0]+= x0, bx[1]+= x0, y-= D(rem) * x0; mn) splay_p(mn), prop(mn, x0);
 }
 // rev=false: f(x) <- min_{y<=x} f(y), rev=true : f(x) <- min_{x<=y} f(y)
 void chmin_cum(bool rev= false) {
  if (bf[0] && bf[1] && bx[0] == bx[1]) y+= D(rem) * bx[0], rem= 0;
  else if (slope_eval(); rem == 0) {
   if (mn) splay_p(mn), mn->d= o[rev], o[!rev]= 0, mn->ch[!rev]= nullptr, update(mn);
  } else if ((rem > 0) ^ rev) {
   assert(bf[rev]);
   y+= D(rem) * bx[rev];
   rem= 0, mn= nullptr;
  } else if (bf[!rev]) {
   T p= std::abs(rem);
   np t= new Node{{nullptr, nullptr}, mn, 0, bx[!rev], p, p, D(bx[!rev]) * p, 1};
   if (mn) splay_p(mn), mn->ch[!rev]= t, update(mn);
   mn= t, o[rev]= p, o[!rev]= 0;
  }
  bf[!rev]= false;
 }
 //  f(x) <- min_{lb<=y<=ub} f(x-y). (lb <= ub), \_/ -> \__/
 void chmin_slide_win(T lb, T ub) {
  assert(lb <= ub);
  if (bf[0] && bf[1] && bx[0] == bx[1]) y+= D(rem) * bx[0], rem= 0;
  else if (slope_eval(); rem == 0) {
   if (mn) {
    splay_p(mn);
    if (o[0] == 0) {
     if (np l= mn->ch[0]; l) prop(l, lb - ub), update(mn);
     prop(mn, ub);
    } else if (o[1] == 0) {
     if (np r= mn->ch[1]; r) prop(r, ub - lb), update(mn);
     prop(mn, lb);
    } else {
     np r= mn->ch[1], t= new Node{{nullptr, r}, mn, 0, mn->x, o[1], 0, 0, 1};
     if (r) r->par= t;
     update(t), prop(mn->ch[1]= t, ub - lb), mn->d= o[0], o[1]= 0, update(mn), prop(mn, lb);
    }
   }
  } else {
   bool r= rem > 0;
   T b[2]= {lb, ub};
   if (bf[!r]) {
    y+= D(rem) * bx[!r];
    T p= r ? rem : -rem;
    np t= new Node{{nullptr, nullptr}, nullptr, 0, bx[!r], p, p, D(bx[!r]) * p, 1};
    if (mn) splay_p(mn), t->ch[r]= mn, mn->par= t, update(t);
    rem= 0, mn= t, prop(t, b[r]), o[r]= p, o[!r]= 0;
   } else if (y-= D(rem) * b[r]; mn) splay_p(mn), prop(mn, b[r]);
  }
  bx[0]+= lb, bx[1]+= ub;
 }
 D operator()(T x) {
  assert(!bf[0] || bx[0] <= x), assert(!bf[1] || x <= bx[1]);
  return calc_y(x) + D(rem) * x + y;
 }
 D min() {
  if (slope_eval(); rem == 0) return y;
  return rem > 0 ? (assert(bf[0]), y + D(rem) * bx[0]) : (assert(bf[1]), y + D(rem) * bx[1]);
 }
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
  np t= mn;
  if (!t) return ret;
  splay_p(t);
  bool r= o[0] == 0;
  if (!r && o[1] != 0) ret[0]= ret[1]= t->x;
  else if (ret[r]= t->x, t= t->ch[!r]; t) {
   for (; t->ch[r];) push(t), t= t->ch[r];
   splay(t), ret[!r]= t->x;
  } else assert(bf[!r]);
  return ret;
 }
 size_t size() { return mn ? splay_p(mn), mn->sz : 0; }
 PiecewiseLinearConvex &operator+=(const PiecewiseLinearConvex &r) {
  y+= r.y, rem+= r.rem;
  if (r.bf[0]) add_inf(false, r.bx[0]);
  if (r.bf[1]) add_inf(true, r.bx[1]);
  if (r.mn) splay_p(r.mn), add_l(r.mn->ch[0]), add_r(r.mn->ch[1]), add_max(-r.o[0], r.o[1], r.mn->x);
  return *this;
 }
};