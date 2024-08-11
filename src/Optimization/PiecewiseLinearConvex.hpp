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
public:
 using D= make_long_t<T>;
 struct Node {
  int ch[2];
  T z, x, d, a;
  D s;
  size_t sz;
  friend std::ostream &operator<<(std::ostream &os, const Node &t) { return os << "{z:" << t.z << ",x:" << t.x << ",d:" << t.d << ",a:" << t.a << ",s:" << t.s << ",sz:" << t.sz << ",ch:(" << t.ch[0] << "," << t.ch[1] << ")}"; }
 };
 static inline int ni= 1;
 static inline Node n[NODE_SIZE];
 static inline void info(int t, int d, std::stringstream &ss) {
  if (!t) return;
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
 std::vector<T> dump_xs() {
  std::vector<T> xs;
  if (bf[0]) xs.push_back(bx[0]);
  dump_xs(lr[0], xs);
  if (mn) xs.push_back(n[mn].x);
  dump_xs(lr[1], xs);
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
  if (mn) as.push_back(-o[0]), dump_slopes_l(lr[0], o[0], as), std::reverse(as.begin(), as.end()), as.push_back(o[1]), dump_slopes_r(lr[1], o[1], as);
  else as.push_back(0);
  for (auto &a: as) a+= rem;
  return as;
 }
 static inline void update(int t) {
  int l= n[t].ch[0], r= n[t].ch[1];
  n[t].sz= 1 + n[l].sz + n[r].sz, n[t].a= n[t].d + n[l].a + n[r].a, n[t].s= D(n[t].x) * n[t].d + n[l].s + n[r].s;
 }
 static inline void prop(int t, T v) { n[t].z+= v, n[t].s+= D(v) * n[t].a, n[t].x+= v; }
 static inline void push(int t) {
  if (n[t].z != 0) prop(n[t].ch[0], n[t].z), prop(n[t].ch[1], n[t].z), n[t].z= 0;
 }
 static inline int joinL(int l, int t, int r) {
  push(l);
  r= join(n[l].ch[1], t, r);
  if (n[n[l].ch[0]].sz * 4 >= n[r].sz) return n[l].ch[1]= r, update(l), l;
  return n[l].ch[1]= n[r].ch[0], update(l), n[r].ch[0]= l, update(r), r;
 }
 static inline int joinR(int l, int t, int r) {
  push(r);
  l= join(l, t, n[r].ch[0]);
  if (n[n[r].ch[1]].sz * 4 >= n[l].sz) return n[r].ch[0]= l, update(r), r;
  return n[r].ch[0]= n[l].ch[1], update(r), n[l].ch[1]= r, update(l), l;
 }
 static inline int join(int l, int t, int r) {
  if (n[l].sz > n[r].sz * 4) return joinL(l, t, r);
  if (n[r].sz > n[l].sz * 4) return joinR(l, t, r);
  return n[t].ch[0]= l, n[t].ch[1]= r, update(t), t;
 }
 static inline int insert(int t, T x, T d) {
  if (!t) return n[ni]= Node{{0, 0}, 0, x, d, d, D(x) * d, 1}, ni++;
  push(t);
  if (n[t].x == x) return n[t].d+= d, update(t), t;
  return x < n[t].x ? join(insert(n[t].ch[0], x, d), t, n[t].ch[1]) : join(n[t].ch[0], t, insert(n[t].ch[1], x, d));
 }
 static inline std::array<int, 2> popB(int t) {
  push(t);
  if (!n[t].ch[1]) return {n[t].ch[0], t};
  auto [a, b]= popB(n[t].ch[1]);
  return {join(n[t].ch[0], t, a), b};
 }
 static inline std::array<int, 2> popF(int t) {
  push(t);
  if (!n[t].ch[0]) return {t, n[t].ch[1]};
  auto [a, b]= popF(n[t].ch[0]);
  return {a, join(b, t, n[t].ch[1])};
 }
 template <bool r> static inline bool lt(T a, T b) {
  if constexpr (r) return b < a;
  else return a < b;
 }
 template <bool r> static inline int cut(int t, T x) {
  if (!t) return t;
  push(t);
  if (n[t].x == x) return n[t].ch[!r];
  if (lt<r>(n[t].x, x)) return cut<r>(n[t].ch[!r], x);
  if constexpr (r) return join(n[t].ch[0], t, cut<1>(n[t].ch[1], x));
  else return join(cut<0>(n[t].ch[0], x), t, n[t].ch[1]);
 }
 template <bool r> static inline D calc_y(int t, T x, T ol, D ou) {
  for (; t;) {
   if (push(t); lt<r>(n[t].x, x)) t= n[t].ch[!r];
   else {
    ol+= n[n[t].ch[!r]].a, ou+= n[n[t].ch[!r]].s;
    if (n[t].x == x) break;
    ol+= n[t].d, ou+= D(n[t].x) * n[t].d, t= n[t].ch[r];
   }
  }
  return D(x) * ol - ou;
 }
 int mn, lr[2];
 bool bf[2];
 T o[2], rem, bx[2];
 D y;
 inline D calc_y(T x) {
  if (!mn) return 0;
  if (n[mn].x == x) return 0;
  return x < n[mn].x ? -calc_y<0>(lr[0], x, o[0], D(n[mn].x) * o[0]) : calc_y<1>(lr[1], x, o[1], D(n[mn].x) * o[1]);
 }
 inline std::array<int, 3> splitR(int t, T p, T ol, D ou) {
  push(t);
  T s= ol + n[n[t].ch[0]].a;
  if (p < s) {
   auto [a, b, c]= splitR(n[t].ch[0], p, ol, ou);
   return {a, b, join(c, t, n[t].ch[1])};
  }
  T l= s + n[t].d;
  if (l < p) {
   auto [a, b, c]= splitR(n[t].ch[1], p, l, ou + n[n[t].ch[0]].s + D(n[t].x) * n[t].d);
   return {join(n[t].ch[0], t, a), b, c};
  }
  o[1]= l - p, o[0]= p - s;
  y+= D(n[t].x) * s - (ou + n[n[t].ch[0]].s);
  return {n[t].ch[0], t, n[t].ch[1]};
 }
 inline std::array<int, 3> splitL(int t, T p, T ol, D ou) {
  push(t);
  T s= ol + n[n[t].ch[1]].a;
  if (p < s) {
   auto [a, b, c]= splitL(n[t].ch[1], p, ol, ou);
   return {join(n[t].ch[0], t, a), b, c};
  }
  T l= s + n[t].d;
  if (l < p) {
   auto [a, b, c]= splitL(n[t].ch[0], p, l, ou + n[n[t].ch[1]].s + D(n[t].x) * n[t].d);
   return {a, b, join(c, t, n[t].ch[1])};
  }
  o[0]= l - p, o[1]= p - s;
  y-= D(n[t].x) * s - (ou + n[n[t].ch[1]].s);
  return {n[t].ch[0], t, n[t].ch[1]};
 }
 inline void slope_lr(bool r) {
  D p= n[lr[r]].s + D(n[mn].x) * o[r];
  T q= o[r] + n[lr[r]].a;
  if (r) {
   y-= p, rem+= q;
   if (lr[1]) {
    auto [a, b]= popB(lr[1]);
    lr[0]= join(lr[0], mn, a), mn= b, lr[1]= 0;
   }
  } else {
   y+= p, rem-= q;
   if (lr[0]) {
    auto [a, b]= popF(lr[0]);
    lr[1]= join(b, mn, lr[1]), mn= a, lr[0]= 0;
   }
  }
  o[!r]= n[mn].d, o[r]= 0;
 }
 inline void slope_eval() {
  if (rem == 0 || !mn) return;
  bool neg= rem < 0;
  T p= neg ? -rem : rem, ol= o[neg];
  if (p <= ol) {
   o[neg]-= p, o[!neg]+= p, y+= D(n[mn].x) * rem, rem= 0;
   return;
  }
  int t= lr[neg];
  if (ol + n[t].a < p) return slope_lr(neg);
  D ou= D(n[mn].x) * ol;
  if (neg) {
   auto [a, b, c]= splitR(t, p, ol, ou);
   y+= D(n[b].x) * rem, rem= 0;
   lr[0]= join(lr[0], mn, a), mn= b, lr[1]= c;
  } else {
   auto [a, b, c]= splitL(t, p, ol, ou);
   y+= D(n[b].x) * rem, rem= 0;
   lr[1]= join(c, mn, lr[1]), mn= b, lr[0]= a;
  }
 }
 template <bool r> void add_inf(T x0) {
  if (bf[r] && !lt<r>(bx[r], x0)) return;
  if (assert(!bf[!r] || !lt<r>(bx[!r], x0)), bf[r]= true, bx[r]= x0; !mn) return;
  if (lt<r>(x0, n[mn].x)) return lr[r]= cut<r>(lr[r], x0), void();
  D p= n[lr[!r]].s + D(n[mn].x) * o[!r];
  T q= o[!r] + n[lr[!r]].a;
  if (!r) {
   y-= p, rem+= q;
   lr[1]= cut<0>(lr[1], x0);
   if (lr[1]) {
    auto [a, b]= popB(lr[1]);
    lr[0]= a, mn= b, lr[1]= 0;
   } else mn= lr[0]= 0;
  } else {
   y+= p, rem-= q;
   lr[0]= cut<1>(lr[0], x0);
   if (lr[0]) {
    auto [a, b]= popF(lr[0]);
    lr[1]= b, mn= a, lr[0]= 0;
   } else mn= lr[1]= 0;
  }
  o[r]= n[mn].d, o[!r]= 0;
 }
 void add_r(int t) {
  if (t) push(t), add_r(n[t].ch[0]), add_max(0, n[t].d, n[t].x), add_r(n[t].ch[1]);
 }
 void add_l(int t) {
  if (t) push(t), add_l(n[t].ch[0]), add_max(-n[t].d, 0, n[t].x), add_l(n[t].ch[1]);
 }
public:
 PiecewiseLinearConvex(): mn(0), lr{0, 0}, bf{0, 0}, o{0, 0}, rem(0), y(0) {}
 std::string info() {
  std::stringstream ss;
  if (ss << "\n rem:" << rem << ", y:" << y << ", mn:" << mn << ", lr:{" << lr[0] << ", " << lr[1] << "}\n bf[0]:" << bf[0] << ", bf[1]:" << bf[1] << ", bx[0]:" << bx[0] << ", bx[1]:" << bx[1] << "\n " << "o[0]:" << o[0] << ", o[1]:" << o[1] << "\n"; mn) {
   if (lr[0]) info(lr[0], 1, ss);
   ss << " ■ " << n[mn] << '\n';
   if (lr[1]) info(lr[1], 1, ss);
  }
  return ss.str();
 }
 static void reset() { ni= 1; }
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
   if (n[mn].x == x0) n[mn].d+= b - a, o[1]+= b - a, y-= D(a) * x0, rem+= a;
   else {
    if (n[mn].x < x0) lr[1]= insert(lr[1], x0, b - a), y-= D(a) * x0, rem+= a;
    else lr[0]= insert(lr[0], x0, b - a), y-= D(b) * x0, rem+= b;
   }
  } else n[mn= ni++]= Node{{0, 0}, 0, x0, b - a, b - a, D(x0) * (b - a), 1}, y-= D(a) * x0, rem+= a, o[0]= 0, o[1]= b - a;
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
  if (bx[0]+= x0, bx[1]+= x0, y-= D(rem) * x0; mn) n[mn].x+= x0, prop(lr[0], x0), prop(lr[1], x0);
 }
 // rev=false: f(x) <- min_{y<=x} f(y), rev=true : f(x) <- min_{x<=y} f(y)
 void chmin_cum(bool rev= false) {
  if (bf[0] && bf[1] && bx[0] == bx[1]) y+= D(rem) * bx[0], rem= 0;
  else {
   if (rem != 0) {
    bool r= rem < 0;
    T u= (r ? -rem : rem) - o[r] - n[lr[r]].a;
    if (0 < u) {
     if (r ^ rev) {
      if (bf[r]) {
       D q= n[lr[r]].s + D(n[mn].x) * o[r] + D(u) * bx[r];
       if (r) {
        y-= q;
        if (mn) lr[!r]= join(lr[!r], mn, lr[r]);
       } else {
        y+= q;
        if (mn) lr[!r]= join(lr[r], mn, lr[!r]);
       }
       n[mn= ni++]= Node{{0, 0}, 0, bx[r], u, u, D(bx[r]) * u, 1};
       lr[r]= 0, o[!r]= u, o[r]= 0, rem= 0;
      }
     } else {
      assert(bf[r]);
      D q= n[lr[r]].s + D(n[mn].x) * o[r] + D(u) * bx[r];
      (r ? y-= q : y+= q), rem= 0, mn= lr[0]= lr[1]= 0, o[0]= o[1]= 0;
     }
     bf[!rev]= false;
     return;
    }
    slope_eval();
   }
   if (mn) {
    if (o[rev] != 0) n[mn].d= o[rev], o[!rev]= 0, lr[!rev]= 0;
    else if (lr[rev]) {
     if (rev) {
      auto [a, b]= popF(lr[1]);
      mn= a, lr[1]= b;
     } else {
      auto [a, b]= popB(lr[0]);
      lr[0]= a, mn= b;
     }
     lr[!rev]= 0, o[rev]= n[mn].d, o[!rev]= 0;
    }
   }
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
     n[mn].x+= ub;
    } else if (o[1] == 0) {
     n[mn].x+= lb;
    } else {
     int t= ni++;
     n[t]= Node{{0, 0}, 0, n[mn].x, o[1], 0, 0, 1};
     lr[1]= join(0, t, lr[1]);
     n[mn].d= o[0], o[1]= 0;
     n[mn].x+= lb;
    }
    prop(lr[0], lb), prop(lr[1], ub);
   }
  } else {
   bool r= rem > 0;
   T b[2]= {lb, ub};
   if (bf[!r]) {
    T p= r ? rem : -rem;
    if (mn) {
     if (r) lr[1]= join(0, mn, lr[1]);
     else lr[0]= join(lr[0], mn, 0);
    }
    n[mn= ni++]= Node{{0, 0}, 0, bx[!r], p, 0, 0, 1};
    y+= D(rem) * bx[!r], rem= 0, n[mn].x+= b[r], prop(lr[r], b[r]), o[r]= p, o[!r]= 0;
   } else if (y-= D(rem) * b[r]; mn) n[mn].x+= b[r], prop(lr[r], b[r]);
  }
  bx[0]+= lb, bx[1]+= ub;
 }
 D operator()(T x) { return assert(!bf[0] || bx[0] <= x), assert(!bf[1] || x <= bx[1]), calc_y(x) + D(rem) * x + y; }
 D min() {
  if (rem == 0) return y;
  bool r= rem < 0;
  T u= (r ? -rem : rem) - o[r] - n[lr[r]].a;
  if (0 < u) {
   assert(bf[r]);
   D q= n[lr[r]].s + D(n[mn].x) * o[r] + D(u) * bx[r];
   return r ? y - q : y + q;
  }
  return slope_eval(), y;
 }
 std::array<T, 2> argmin() {
  if (rem != 0) {
   if (bool r= rem < 0; o[r] + n[lr[r]].a < (r ? -rem : rem)) {
    assert(bf[r]);
    return {bx[r], bx[r]};
   }
   slope_eval();
  }
  std::array<T, 2> ret= {bx[0], bx[1]};
  int t= mn;
  if (!t) return ret;
  bool r= o[0] == 0;
  if (!r && o[1] != 0) ret[0]= ret[1]= n[t].x;
  else if (ret[r]= n[t].x, t= lr[!r]; t) {
   for (; push(t), n[t].ch[r];) t= n[t].ch[r];
   ret[!r]= n[t].x;
  } else assert(bf[!r]);
  return ret;
 }
 size_t size() { return n[lr[0]].sz + n[lr[1]].sz + !!mn; }
 PiecewiseLinearConvex &operator+=(const PiecewiseLinearConvex &g) {
  if (y+= g.y, rem+= g.rem; g.bf[0]) add_inf(false, g.bx[0]);
  if (g.bf[1]) add_inf(true, g.bx[1]);
  if (g.mn) add_l(g.lr[0]), add_r(g.lr[1]), add_max(-g.o[0], g.o[1], n[g.mn].x);
  return *this;
 }
};