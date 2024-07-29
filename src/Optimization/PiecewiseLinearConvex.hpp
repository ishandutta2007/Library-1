#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include "src/Internal/long_traits.hpp"
template <class T> class PiecewiseLinearConvex {
public:
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
 static inline void prop(np t, T v) {
  if (t) t->z+= v, t->s+= D(v) * t->a, t->x+= v;
 }
 static inline void push(np t) {
  if (t->z) prop(t->ch[0], t->z), prop(t->ch[1], t->z), t->z= 0;
 }
 static inline void rot(np t) {
  np p= t->par;
  if (bool d= p->ch[1] == t; (p->ch[d]= std::exchange(t->ch[!d], p))) p->ch[d]->par= p;
  if ((t->par= std::exchange(p->par, t))) t->par->ch[t->par->ch[1] == p]= t;
  update(p);
 }
 static inline void splay(np t) {
  if (np p= t->par; p) do {
    if (p->par) push(p->par), push(p), push(t), rot(p->par->ch[p->ch[1] == t] == p ? p : t);
    else push(p), push(t);
    rot(t), p= t->par;
   } while (p);
  else push(t);
  update(t);
 }
 static inline void add_node(np t, T x, T d) {
  for (;;) {
   if (t->x == x) {
    t->d+= d;
    break;
   }
   push(t);
   np &n= t->ch[t->x < x];
   if (!n) {
    n= new Node{{nullptr, nullptr}, t, 0, x, d, d, D(x) * d, 1}, t= n;
    break;
   }
   t= n;
  }
  splay(t);
 }
 static inline D calc_y_r(np t, T x, T ol, D ou) {
  for (; t;) {
   push(t);
   if (x < t->x) t= t->ch[0];
   else {
    ol+= sl(t->ch[0]), ou+= sum(t->ch[0]);
    if (t->x == x) break;
    ol+= t->d, ou+= D(t->x) * t->d, t= t->ch[1];
   }
  }
  return D(x) * ol - ou;
 }
 static inline D calc_y_l(np t, T x, T ol, D ou) {
  for (; t;) {
   push(t);
   if (t->x < x) t= t->ch[1];
   else {
    ol+= sl(t->ch[1]), ou+= sum(t->ch[1]);
    if (t->x == x) break;
    ol+= t->d, ou+= D(t->x) * t->d, t= t->ch[0];
   }
  }
  return ou - D(x) * ol;
 }
 static inline T sl(np t) { return t ? t->a : 0; }
 static inline D sum(np t) { return t ? t->s : 0; }
 np m[2];
 bool bf[2];
 T o[2], rem, bx[2];
 D y;
 D calc_y(T x) {
  if (!m[0] && !m[1]) return 0;
  if (!m[0]) {
   if (x <= m[1]->x) return 0;
   return splay(m[1]), calc_y_r(m[1], x, 0, 0);
  }
  if (!m[1]) {
   if (m[0]->x <= x) return 0;
   return splay(m[0]), calc_y_l(m[0], x, 0, 0);
  }
  if (m[0] == m[1]) {
   if (m[0]->x == x) return 0;
   splay(m[0]);
   if (x < m[0]->x) {
    if (m[0]->ch[0]) return calc_y_l(m[0]->ch[0], x, o[0], D(m[0]->x) * o[0]);
    return D(m[0]->x - x) * o[0];
   } else {
    if (m[0]->ch[1]) return calc_y_r(m[0]->ch[1], x, o[1], D(m[0]->x) * o[1]);
    return D(x - m[0]->x) * o[1];
   }
  }
  if (x < m[0]->x) return splay(m[1]), calc_y_l(m[1]->ch[0], x, 0, 0);
  if (m[1]->x < x) return splay(m[0]), calc_y_r(m[0]->ch[1], x, 0, 0);
  return 0;
 }
 template <bool r> void slope_lr() {
  np t= m[r];
  if (!t) return;
  splay(t);
  T ol= m[0] == m[1] ? o[r] : t->d;
  if constexpr (r) y-= sum(t->ch[r]) + D(t->x) * ol, rem+= ol + sl(t->ch[r]);
  else y+= sum(t->ch[r]) + D(t->x) * ol, rem-= ol + sl(t->ch[r]);
  for (; t->ch[r];) push(t), t= t->ch[r];
  m[!r]= t, m[r]= nullptr;
 }
 void slope_eval() {
  if (rem == 0) return;
  bool neg= rem < 0;
  if (!m[neg]) return;
  T p= abs(rem), ol= 0;
  D ou= 0;
  np t= m[!neg];
  if (t) {
   splay(t);
   if (m[0] == m[1]) {
    if (ol= o[neg]; p == ol) {
     if (y+= D(t->x) * rem, rem= 0, t= t->ch[neg]; t)
      for (; t->ch[!neg];) t= t->ch[!neg];
     m[neg]= t;
     return;
    }
    if (p < ol) {
     o[neg]-= p, o[!neg]+= p, y+= D(t->x) * rem, rem= 0;
     return;
    }
    ou+= D(t->x) * ol;
   }
   t= t->ch[neg];
  } else t= m[neg], splay(t);
  if (ol + sl(t) < p) {
   if (neg) slope_lr<1>();
   else slope_lr<0>();
   return;
  }
  for (;;) {
   push(t);
   T s= ol + sl(t->ch[!neg]), l= s + t->d;
   if (p < s) t= t->ch[!neg];
   else if (l < p) ol= l, ou+= sum(t->ch[!neg]) + D(t->x) * t->d, t= t->ch[neg];
   else {
    y+= D(t->x) * rem, rem= 0;
    if (neg) y+= D(t->x) * s - (ou + sum(t->ch[!neg]));
    else y-= D(t->x) * s - (ou + sum(t->ch[!neg]));
    if (p == s) {
     for (m[neg]= t, t= t->ch[!neg]; t->ch[neg];) t= t->ch[neg];
     m[!neg]= t;
    } else if (p == l) {
     if (m[!neg]= t, t= t->ch[neg]; t)
      for (; t->ch[!neg];) t= t->ch[!neg];
     m[neg]= t;
    } else m[0]= m[1]= t, o[neg]= l - p, o[!neg]= p - s;
    break;
   }
  }
 }
 void add_r(np t) {
  if (t) push(t), add_r(t->ch[0]), add_max(0, t->d, t->x), add_r(t->ch[1]);
 }
 void add_l(np t) {
  if (t) push(t), add_l(t->ch[0]), add_max(-t->d, 0, t->x), add_l(t->ch[1]);
 }
public:
 PiecewiseLinearConvex(): m{nullptr, nullptr}, bf{0, 0}, rem(0), y(0) {}
 std::string info() {
  std::stringstream ss;
  ss << "\n rem:" << rem << ", y:" << y << ", m[0]:" << m[0] << ", m[1]:" << m[1] << "\n bf[0]:" << bf[0] << ", bf[1]:" << bf[1] << ", bx[0]:" << bx[0] << ", bx[1]:" << bx[1] << "\n " << "o[0]:" << o[0] << ", o[1]:" << o[1] << "\n";
  if (m[0]) splay(m[0]), info(m[0], 0, ss);
  else if (m[1]) splay(m[1]), info(m[1], 0, ss);
  return ss.str();
 }
 std::vector<T> dump_xs() {
  std::vector<T> xs;
  if (bf[0]) xs.push_back(bx[0]);
  if (m[0]) splay(m[0]), dump_xs(m[0], xs);
  else if (m[1]) splay(m[1]), dump_xs(m[1], xs);
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
  if (!m[0] && !m[1]) as.push_back(0);
  else if (m[0] == m[1]) splay(m[0]), as.push_back(-o[0]), dump_slopes_l(m[0]->ch[0], o[0], as), std::reverse(as.begin(), as.end()), as.push_back(o[1]), dump_slopes_r(m[0]->ch[1], o[1], as);
  else if (!m[1]) splay(m[0]), as.push_back(0), dump_slopes_l(m[0], 0, as), std::reverse(as.begin(), as.end());
  else if (!m[0]) splay(m[1]), as.push_back(0), dump_slopes_r(m[1], 0, as);
  else {
   splay(m[0]);
   np t= m[0]->ch[1];
   m[0]->ch[1]= nullptr, dump_slopes_l(m[0], 0, as), std::reverse(as.begin(), as.end()), m[0]->ch[1]= t, as.push_back(0), dump_slopes_r(t, 0, as);
  }
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
  else if (!m[0] && !m[1]) {
   np t= new Node{{nullptr, nullptr}, nullptr, 0, x0, b - a, b - a, D(x0) * (b - a), 1};
   if (a >= 0) {
    m[1]= t;
    y-= D(a) * x0, rem+= a;
   } else if (b <= 0) {
    m[0]= t;
    y-= D(b) * x0, rem+= b;
   } else m[0]= m[1]= t, o[0]= -a, o[1]= b;
  } else if (!m[0]) {
   splay(m[1]);
   if (m[1]->x <= x0) add_node(m[1], x0, b - a);
   else {
    np t= new Node{{nullptr, m[1]}, nullptr, 0, x0, b - a};
    m[1]->par= t;
    update(m[1]= t);
   }
   y-= D(a) * x0, rem+= a;
  } else if (!m[1]) {
   splay(m[0]);
   if (x0 <= m[0]->x) add_node(m[0], x0, b - a);
   else {
    np t= new Node{{m[0], nullptr}, nullptr, 0, x0, b - a};
    m[0]->par= t;
    update(m[0]= t);
   }
   y-= D(b) * x0, rem+= b;
  } else if (m[0] == m[1]) {
   splay(m[0]), add_node(m[0], x0, b - a);
   if (x0 < m[0]->x) y-= D(b) * x0, rem+= b;
   else {
    if (x0 == m[0]->x) o[1]+= b - a;
    y-= D(a) * x0, rem+= a;
   }
  } else if (x0 <= m[0]->x) splay(m[1]), add_node(m[1]->ch[0], x0, b - a), y-= D(b) * x0, rem+= b;
  else if (m[1]->x <= x0) splay(m[0]), add_node(m[0]->ch[1], x0, b - a), y-= D(a) * x0, rem+= a;
  else {
   splay(m[0]);
   np r= m[0]->ch[1], t= new Node{{m[0], r}, nullptr, 0, x0, b - a};
   r->par= m[0]->par= t;
   m[0]->ch[1]= nullptr;
   update(m[0]);
   update(t);
   m[0]= t;
   y-= D(b) * x0, rem+= b;
  }
 }
 // f(x) +=  max(0, a(x-x0))
 void add_ramp(T a, T x0) {
  if (a == 0) return;
  if (a > 0) add_max(0, a, x0);
  else add_max(a, 0, x0);
 }
 // f(x) += a|x-x0|, \/
 void add_abs(T a, T x0) {
  assert(a >= 0);
  if (a == 0) return;
  add_max(-a, a, x0);
 }
 // rev=false: f(x) <- min_{y<=x} f(y), rev=true : f(x) <- min_{x<=y} f(y)
 void chmin_cum(bool rev= false) {
  if (bf[0] && bf[1] && bx[0] == bx[1]) {
   y+= D(rem) * bx[0], rem= 0, bf[!rev]= false;
   return;
  }
  slope_eval();
  if (!rev) {
   if (rem > 0) {
    assert(bf[0]);
    y+= D(rem) * bx[0];
    m[1]= nullptr, rem= 0, bf[1]= false;
   } else if (rem < 0) {
    if (!bf[1]) return;
    m[1]= new Node{{nullptr, nullptr}, m[0], 0, bx[1], -rem, -rem, -D(bx[1]) * rem, 1};
    if (m[0]) splay(m[0]), m[0]->ch[1]= m[1], update(m[0]);
    bf[1]= false;
   } else {
    bf[1]= false;
    if (!m[1]) return;
    if (!m[0]) {
     m[1]= nullptr;
     return;
    }
    splay(m[0]);
    if (m[0] == m[1]) m[0]->d= o[0];
    m[0]->ch[1]= m[1]= nullptr;
    update(m[0]);
   }
  } else {
   if (rem < 0) {
    assert(bf[1]);
    y+= D(rem) * bx[1];
    m[0]= nullptr, rem= 0, bf[0]= false;
   } else if (rem > 0) {
    if (!bf[0]) return;
    m[0]= new Node{{nullptr, nullptr}, m[1], 0, bx[0], rem, rem, D(bx[0]) * rem, 1};
    if (m[1]) splay(m[1]), m[1]->ch[0]= m[0], update(m[1]);
    bf[0]= false;
   } else {
    bf[!rev]= false;
    if (!m[!rev]) return;
    if (!m[rev]) {
     m[!rev]= nullptr;
     return;
    }
    splay(m[rev]);
    if (m[0] == m[1]) m[rev]->d= o[rev];
    m[rev]->ch[!rev]= m[!rev]= nullptr;
    update(m[rev]);
   }
  }
 }
 //  f(x) <- min_{lb<=y<=ub} f(x-y). (lb <= ub), \_/ -> \__/
 void chmin_slide_win(T lb, T ub) {
  assert(lb <= ub);
  if (bf[0] && bf[1] && bx[0] == bx[1]) {
   y+= D(rem) * bx[0], rem= 0, bx[0]+= lb, bx[1]+= ub;
   return;
  }
  slope_eval();
  if (rem < 0) {
   if (bf[1]) {
    y+= D(rem) * bx[1];
    np t= new Node{{nullptr, nullptr}, m[0], 0, bx[1], -rem, -rem, -D(bx[1]) * rem, 1};
    if (m[0]) splay(m[0]), m[0]->ch[1]= t, update(m[0]);
    rem= 0, bx[1]+= ub, splay(m[0]= t), prop(t, lb);
   } else if (m[0]) splay(m[0]), prop(m[0], lb);
   if (bf[0]) bx[0]+= lb;
   y-= D(rem) * lb;
  } else if (rem > 0) {
   if (bf[0]) {
    y+= D(rem) * bx[0];
    np t= new Node{{nullptr, nullptr}, m[1], 0, bx[0], rem, rem, D(bx[1]) * rem, 1};
    if (m[1]) splay(m[1]), m[1]->ch[0]= t, update(m[1]);
    rem= 0, bx[0]+= lb, splay(m[1]= t), prop(t, ub);
   } else if (m[1]) splay(m[1]), prop(m[1], ub);
   if (bf[1]) bx[1]+= ub;
   y-= D(rem) * ub;
  } else {
   if (m[0]) {
    splay(m[0]);
    if (m[0] == m[1]) {
     np r= m[0]->ch[1];
     m[0]->ch[1]= nullptr;
     np t= new Node{{nullptr, r}, m[0], 0, m[0]->x, o[1], 0, 0, 1};
     if (r) r->par= t;
     update(t), prop(t, ub), m[0]->d= o[0], prop(m[0], lb), push(m[0]), m[0]->ch[1]= m[1]= t, update(m[0]);
    } else if (m[1]) {
     np t= m[0]->ch[1];
     m[0]->ch[1]= nullptr, prop(m[0], lb), push(m[0]), prop(t, ub), m[0]->ch[1]= t;
     update(m[0]);
     splay(m[1]);
    } else prop(m[0], lb);
   } else if (m[1]) splay(m[1]), prop(m[1], ub);
   if (bf[0]) bx[0]+= lb;
   if (bf[1]) bx[1]+= ub;
  }
 }
 // f(x) <- f(x-x0)
 void shift(T x0) {
  if (y-= D(rem) * x0; m[0]) {
   if (splay(m[0]), prop(m[0], x0); m[1]) splay(m[1]);
  } else if (m[1]) splay(m[1]), prop(m[1], x0);
  if (bf[0]) bx[0]+= x0;
  if (bf[1]) bx[1]+= x0;
 }
 // right=false : f(x) +=  inf  (x < x_0), right=true: f(x) += inf  (x_0 < x)
 void add_inf(bool right= false, T x0= 0) {
  if (right) {
   if (bf[1] && bx[1] <= x0) return;
   assert(!bf[0] || bx[0] <= x0);
   bf[1]= true, bx[1]= x0;
   if (!m[0] && !m[1]) return;
   slope_lr<0>();
   if (x0 <= m[1]->x) {
    m[1]= nullptr;
    return;
   }
   splay(m[1]);
   np t= m[1], s= t;
   for (; t;) {
    if (push(t); t->x < x0) s= t, t= t->ch[1];
    else t= t->ch[0];
   }
   splay(s), s->ch[1]= nullptr;
   update(s);
  } else {
   if (bf[0] && x0 <= bx[0]) return;
   assert(!bf[1] || x0 <= bx[1]);
   bf[0]= true, bx[0]= x0;
   if (!m[0] && !m[1]) return;
   slope_lr<1>();
   if (m[0]->x <= x0) {
    m[0]= nullptr;
    return;
   }
   splay(m[0]);
   np t= m[0], s= t;
   for (; t;) {
    push(t);
    if (x0 < t->x) s= t, t= t->ch[0];
    else t= t->ch[1];
   }
   splay(s), s->ch[0]= nullptr;
   update(s);
  }
 }
 D min() {
  slope_eval();
  if (rem > 0) {
   assert(bf[0]);
   return y + D(rem) * bx[0];
  }
  if (rem < 0) {
   assert(bf[1]);
   return y + D(rem) * bx[1];
  }
  return y;
 }
 std::pair<T, T> argmin() {
  slope_eval();
  if (rem > 0) {
   assert(bf[0]);
   return {bx[0], bx[0]};
  }
  if (rem < 0) {
   assert(bf[1]);
   return {bx[1], bx[1]};
  }
  assert(m[0] || bf[0]), assert(m[1] || bf[1]);
  return {m[0] ? m[0]->x : bx[0], m[1] ? m[1]->x : bx[1]};
 }
 D operator()(T x) {
  assert(!bf[0] || bx[0] <= x), assert(!bf[1] || x <= bx[1]);
  return calc_y(x) + D(rem) * x + y;
 }
 size_t size() {
  if (m[0]) return splay(m[0]), m[0]->sz;
  if (m[1]) return splay(m[1]), m[1]->sz;
  return 0;
 }
 PiecewiseLinearConvex &operator+=(const PiecewiseLinearConvex &r) {
  y+= r.y, rem+= r.rem;
  if (r.bf[0]) add_inf(false, r.bx[0]);
  if (r.bf[1]) add_inf(true, r.bx[1]);
  if (!r.m[1]) {
   if (!r.m[0]) return *this;
   splay(r.m[0]), add_l(r.m[0]);
  } else if (!r.m[0]) splay(r.m[1]), add_r(r.m[1]);
  else if (r.m[0] == r.m[1]) splay(r.m[0]), add_l(r.m[0]->ch[0]), add_r(r.m[0]->ch[1]), add_max(-r.o[0], r.o[1], r.m[0]->x);
  else {
   splay(r.m[0]);
   np t= r.m[0]->ch[1];
   add_r(t), r.m[0]->ch[1]= nullptr, add_l(r.m[0]), r.m[0]->ch[1]= t;
  }
  return *this;
 }
};