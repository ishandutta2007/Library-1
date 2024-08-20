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
template <class T, bool persistent= false, size_t NODE_SIZE= 1 << (20 + 2 * persistent)> class PiecewiseLinearConvex {
 using D= make_long_t<T>;
 struct Node {
  int ch[2]= {0, 0};
  T z= 0, x= 0, d= 0, a= 0;
  D s= 0;
  size_t sz= 0;
  friend std::ostream &operator<<(std::ostream &os, const Node &t) { return os << "{z:" << t.z << ",x:" << t.x << ",d:" << t.d << ",a:" << t.a << ",s:" << t.s << ",sz:" << t.sz << ",ch:(" << t.ch[0] << "," << t.ch[1] << ")}"; }
 };
 static inline size_t ni= 1;
 static inline Node *n= new Node[NODE_SIZE]{Node{}};
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
 static inline int create(T d, T x) { return n[ni].d= d, n[ni].x= x, n[ni].z= 0, ni++; }
 static inline bool lt(T a, T b) {
  if constexpr (std::is_floating_point_v<T>) return 1e-15 < b - a;
  else return a < b;
 }
 template <class Iter> static inline int build(Iter bg, Iter ed) {
  if (bg == ed) return 0;
  auto md= bg + (ed - bg) / 2;
  int t= create(md->first, md->second);
  return n[t].ch[0]= build(bg, md), n[t].ch[1]= build(md + 1, ed), update(t), t;
 }
 template <class Iter> static inline void dump(Iter itr, int t) {
  if (!t) return;
  push(t);
  size_t sz= n[n[t].ch[0]].sz;
  dump(itr, n[t].ch[0]), *(itr + sz)= {n[t].d, n[t].x}, dump(itr + sz + 1, n[t].ch[1]);
 }
 static inline void update(int t) {
  int l= n[t].ch[0], r= n[t].ch[1];
  n[t].sz= 1 + n[l].sz + n[r].sz, n[t].a= n[t].d + n[l].a + n[r].a, n[t].s= D(n[t].x) * n[t].d + n[l].s + n[r].s;
 }
 template <bool b= 1> static inline void prop(int &t, T v) {
  if constexpr (persistent && b) {
   if (!t) return;
   n[ni]= n[t], t= ni++;
  }
  n[t].z+= v, n[t].s+= D(v) * n[t].a, n[t].x+= v;
 }
 static inline void push(int t) {
  if (n[t].z != 0) prop(n[t].ch[0], n[t].z), prop(n[t].ch[1], n[t].z), n[t].z= 0;
 }
 template <bool r> static inline int join_(int t, int a, int b) {
  push(a);
  if constexpr (r) b= join<0>(b, t, n[a].ch[0]);
  else b= join<0>(n[a].ch[1], t, b);
  if constexpr (persistent) n[ni]= n[a], a= ni++;
  if (n[n[a].ch[r]].sz * 4 >= n[b].sz) return n[a].ch[!r]= b, update(a), a;
  return n[a].ch[!r]= n[b].ch[r], update(a), n[b].ch[r]= a, update(b), b;
 }
 template <bool b= 1> static inline int join(int l, int t, int r) {
  if constexpr (persistent && b) n[ni]= n[t], t= ni++;
  if (n[l].sz > n[r].sz * 4) return join_<0>(t, l, r);
  if (n[r].sz > n[l].sz * 4) return join_<1>(t, r, l);
  return n[t].ch[0]= l, n[t].ch[1]= r, update(t), t;
 }
 static inline std::array<int, 3> split(int t, T x) {
  if (!t) return {0, 0, 0};
  push(t);
  if (lt(n[t].x, x)) {
   auto [a, b, c]= split(n[t].ch[1], x);
   return {join(n[t].ch[0], t, a), b, c};
  } else if (lt(x, n[t].x)) {
   auto [a, b, c]= split(n[t].ch[0], x);
   return {a, b, join(c, t, n[t].ch[1])};
  }
  return {n[t].ch[0], t, n[t].ch[1]};
 }
 static inline int unite(int l, int r) {
  if (!l) return r;
  if (!r) return l;
  push(l);
  if constexpr (persistent) n[ni]= n[l], l= ni++;
  auto [a, b, c]= split(r, n[l].x);
  return n[l].d+= n[b].d, join<0>(unite(a, n[l].ch[0]), l, unite(n[l].ch[1], c));
 }
 static inline int insert(int t, T x, T d) {
  if (!t) return n[ni]= Node{{0, 0}, 0, x, d, d, D(x) * d, 1}, ni++;
  push(t);
  if constexpr (persistent) n[ni]= n[t], t= ni++;
  if (lt(x, n[t].x)) return join<0>(insert(n[t].ch[0], x, d), t, n[t].ch[1]);
  if (lt(n[t].x, x)) return join<0>(n[t].ch[0], t, insert(n[t].ch[1], x, d));
  return n[t].d+= d, update(t), t;
 }
 template <bool r> static inline std::pair<int, int> pop(int t) {
  if (push(t); !n[t].ch[r]) return {n[t].ch[!r], t};
  auto [a, s]= pop<r>(n[t].ch[r]);
  if constexpr (r) return {join(n[t].ch[!r], t, a), s};
  else return {join(a, t, n[t].ch[!r]), s};
 }
 template <bool g> static inline bool lgt(T a, T b) {
  if constexpr (g) return lt(b, a);
  else return lt(a, b);
 }
 template <bool r> static inline int cut(int t, T x) {
  if (!t) return t;
  if (push(t); lgt<r>(n[t].x, x)) return cut<r>(n[t].ch[!r], x);
  if (lgt<r>(x, n[t].x)) {
   if constexpr (r) return join(n[t].ch[0], t, cut<1>(n[t].ch[1], x));
   else return join(cut<0>(n[t].ch[0], x), t, n[t].ch[1]);
  }
  return n[t].ch[!r];
 }
 template <bool r> static inline D calc_y(int t, T x, T ol, D ou) {
  for (; t;) {
   if (push(t); lgt<r>(n[t].x, x)) t= n[t].ch[!r];
   else {
    ol+= n[n[t].ch[!r]].a, ou+= n[n[t].ch[!r]].s;
    if (!lgt<r>(x, n[t].x)) break;
    ol+= n[t].d, ou+= D(n[t].x) * n[t].d, t= n[t].ch[r];
   }
  }
  return D(x) * ol - ou;
 }
 template <bool r> static inline std::array<int, 3> split(int t, T p, T &ol, D &ou) {
  push(t);
  T s= ol + n[n[t].ch[!r]].a;
  if (lt(p, s)) {
   auto [a, b, c]= split<r>(n[t].ch[!r], p, ol, ou);
   if constexpr (r) return {a, b, join(c, t, n[t].ch[r])};
   else return {join(n[t].ch[r], t, a), b, c};
  }
  ol= s + n[t].d;
  if (lt(ol, p)) {
   ou+= n[n[t].ch[!r]].s + D(n[t].x) * n[t].d;
   auto [a, b, c]= split<r>(n[t].ch[r], p, ol, ou);
   if constexpr (r) return {join(n[t].ch[!r], t, a), b, c};
   else return {a, b, join(c, t, n[t].ch[!r])};
  }
  ou+= n[n[t].ch[!r]].s;
  return {n[t].ch[0], t, n[t].ch[1]};
 }
 template <bool l> static inline bool lte(T a, T b) {
  if constexpr (l) return lt(a, b);
  else return !lt(b, a);
 }
 template <bool l, bool r> static inline std::pair<int, int> split_cum(int t, T p, T &ol, D &ou) {
  push(t);
  T s= ol + n[n[t].ch[!r]].a;
  if (lte<l>(p, s)) {
   auto [c, b]= split_cum<l, r>(n[t].ch[!r], p, ol, ou);
   if constexpr (l) {
    if constexpr (r) return {join(c, t, n[t].ch[r]), b};
    else return {join(n[t].ch[r], t, c), b};
   } else return {c, b};
  }
  ol= s + n[t].d;
  if (lte<!l>(ol, p)) {
   ou+= n[n[t].ch[!r]].s + D(n[t].x) * n[t].d;
   auto [a, b]= split_cum<l, r>(n[t].ch[r], p, ol, ou);
   if constexpr (l) return {a, b};
   else {
    if constexpr (r) return {join(n[t].ch[!r], t, a), b};
    else return {join(a, t, n[t].ch[!r]), b};
   }
  }
  ou+= n[n[t].ch[!r]].s;
  return {n[t].ch[!r ^ l], t};
 }
 int mn, lr[2];
 bool bf[2];
 T o[2], rem, bx[2];
 D y;
 inline D calc_y(T x) {
  if (!mn) return 0;
  if (lt(x, n[mn].x)) return -calc_y<0>(lr[0], x, o[0], D(n[mn].x) * o[0]);
  if (lt(n[mn].x, x)) return calc_y<1>(lr[1], x, o[1], D(n[mn].x) * o[1]);
  return 0;
 }
 inline void slope_eval(bool neg) {
  T p= neg ? -rem : rem, ol= o[neg];
  if (p <= ol) o[neg]-= p, o[!neg]+= p, y+= D(n[mn].x) * rem;
  else {
   D ou= D(n[mn].x) * ol;
   auto [a, b, c]= neg ? split<1>(lr[neg], p, ol, ou) : split<0>(lr[neg], p, ol, ou);
   o[neg]= ol - p, ol-= n[b].d, ou+= D(n[b].x) * (o[!neg]= p - ol);
   if (neg) y-= ou, lr[!neg]= join(lr[!neg], mn, a), lr[neg]= c;
   else y+= ou, lr[!neg]= join(c, mn, lr[!neg]), lr[neg]= a;
   mn= b;
  }
  rem= 0;
 }
 template <bool l, bool neg> inline void slope_eval_cum() {
  T p= neg ? -rem : rem, ol= o[neg];
  if (lte<l>(p, ol)) o[neg]-= p, o[!neg]+= p, y+= D(n[mn].x) * rem;
  else {
   D ou= D(n[mn].x) * ol;
   auto [a, b]= split_cum<l, neg>(lr[neg], p, ol, ou);
   o[neg]= ol - p, ol-= n[b].d, ou+= D(n[b].x) * (o[!neg]= p - ol);
   if constexpr (l) lr[neg]= a;
   else {
    if constexpr (neg) lr[!neg]= join(lr[!neg], mn, a);
    else lr[!neg]= join(a, mn, lr[!neg]);
   }
   if constexpr (neg) y-= ou;
   else y+= ou;
   mn= b;
  }
  rem= 0;
 }
 template <bool r> void add_inf(T x0) {
  if (bf[r] && !lgt<r>(bx[r], x0)) return;
  if (assert(!bf[!r] || !lgt<r>(bx[!r], x0)), bf[r]= true, bx[r]= x0; !mn) return;
  if (lgt<r>(x0, n[mn].x)) return lr[r]= cut<r>(lr[r], x0), void();
  D q= n[lr[!r]].s + D(n[mn].x) * o[!r];
  T v= o[!r] + n[lr[!r]].a;
  lr[!r]= cut<r>(lr[!r], x0);
  if (!r) y-= q, rem+= v;
  else y+= q, rem-= v;
  if (lr[!r]) std::tie(lr[r], mn)= pop<!r>(lr[!r]), lr[!r]= 0;
  else mn= lr[r]= 0;
  o[r]= n[mn].d, o[!r]= 0;
 }
 inline void prop(T x) {
  if constexpr (persistent) mn= create(n[mn].d, n[mn].x);
  n[mn].x+= x;
 }
public:
 // f(x) := 0
 PiecewiseLinearConvex(): mn(0), lr{0, 0}, bf{0, 0}, o{0, 0}, rem(0), bx{0, 0}, y(0) {}
 //  f(x) := sum max(0, a(x-x0))
 PiecewiseLinearConvex(const std::vector<std::pair<T, T>> &ramps): PiecewiseLinearConvex() {
  int m= ramps.size();
  if (!m) return;
  std::vector<std::pair<T, T>> w(m);
  int s= 0, t= 0;
  for (auto [d, x]: ramps) {
   if (lt(d, 0)) y-= D(d) * x, rem+= d, d= -d;
   if (!lt(0, d)) continue;
   w[s++]= {d, x};
  }
  std::sort(w.begin(), w.begin() + s, [](auto a, auto b) { return a.second < b.second; });
  for (int i= 0; i < s; ++i) {
   if (t && !lt(w[t - 1].second, w[i].second) && !lt(w[i].second, w[t - 1].second)) w[t - 1].first+= w[i].first;
   else w[t++]= w[i];
  }
  mn= create(w[0].first, w[0].second), o[1]= n[mn].d, lr[1]= build(w.begin() + 1, w.begin() + t);
 }
 std::string info() {
  std::stringstream ss;
  if (ss << "\n rem:" << rem << ", y:" << y << ", mn:" << mn << ", lr:{" << lr[0] << ", " << lr[1] << "}\n bf[0]:" << bf[0] << ", bf[1]:" << bf[1] << ", bx[0]:" << bx[0] << ", bx[1]:" << bx[1] << "\n " << "o[0]:" << o[0] << ", o[1]:" << o[1] << "\n"; mn) {
   if (lr[0]) info(lr[0], 1, ss);
   ss << " ■ " << n[mn] << '\n';
   if (lr[1]) info(lr[1], 1, ss);
  }
  return ss.str();
 }
 template <class... Args> static inline void rebuild(Args &...plc) {
  static_assert(std::conjunction_v<std::is_same<PiecewiseLinearConvex, Args>...>);
  constexpr size_t m= sizeof...(Args);
  std::array<std::vector<std::pair<T, T>>, m> ls, rs;
  std::array<std::pair<T, T>, m> mns;
  int i= 0;
  (void)(int[]){(mns[i]= {n[plc.mn].d, n[plc.mn].x}, ls[i].resize(n[plc.lr[0]].sz), rs[i].resize(n[plc.lr[1]].sz), dump(ls[i].begin(), plc.lr[0]), dump(rs[i].begin(), plc.lr[1]), ++i)...};
  ni= 1, i= 0;
  (void)(int[]){((plc.mn ? (plc.mn= create(mns[i].first, mns[i].second)) : 0), plc.lr[0]= build(ls[i].begin(), ls[i].end()), plc.lr[1]= build(rs[i].begin(), rs[i].end()), ++i)...};
 }
 static inline void rebuild(std::vector<PiecewiseLinearConvex> &plcs) {
  size_t m= plcs.size();
  std::vector<std::vector<std::pair<T, T>>> ls(m), rs(m);
  std::vector<std::pair<T, T>> mns(m);
  for (int i= m; i--;) mns[i]= {n[plcs[i].mn].d, n[plcs[i].mn].x}, ls[i].resize(n[plcs[i].lr[0]].sz), rs[i].resize(n[plcs[i].lr[1]].sz), dump(ls[i].begin(), plcs[i].lr[0]), dump(rs[i].begin(), plcs[i].lr[1]);
  ni= 1;
  for (int i= m; i--;) (plcs[i].mn ? (plcs[i].mn= create(mns[i].first, mns[i].second)) : 0), plcs[i].lr[0]= build(ls[i].begin(), ls[i].end()), plcs[i].lr[1]= build(rs[i].begin(), rs[i].end());
 }
 static void reset() { ni= 1; }
 static bool pool_empty() {
  if constexpr (persistent) return ni >= NODE_SIZE * 0.8;
  else return ni + 1000 >= NODE_SIZE;
 }
 // f(x) += c
 void add_const(D c) { y+= c; }
 // f(x) += ax, /
 void add_linear(T a) { rem+= a; }
 //  f(x) += max(a(x-x0),b(x-x0)), (a < b)
 void add_max(T a, T b, T x0) {
  assert(lt(a, b));
  if (bf[0] && x0 <= bx[0]) y-= D(b) * x0, rem+= b;
  else if (bf[1] && bx[1] <= x0) y-= D(a) * x0, rem+= a;
  else if (T c= b - a; mn) {
   if (lt(n[mn].x, x0)) lr[1]= insert(lr[1], x0, c), y-= D(a) * x0, rem+= a;
   else if (lt(x0, n[mn].x)) lr[0]= insert(lr[0], x0, c), y-= D(b) * x0, rem+= b;
   else {
    if constexpr (persistent) mn= create(n[mn].d, n[mn].x);
    n[mn].d+= c, o[1]+= c, y-= D(a) * x0, rem+= a;
   }
  } else mn= create(c, x0), y-= D(a) * x0, rem+= a, o[0]= 0, o[1]= c;
 }
 // f(x) +=  max(0, a(x-x0))
 void add_ramp(T a, T x0) {
  if (lt(0, a)) add_max(0, a, x0);
  else if (lt(a, 0)) add_max(a, 0, x0);
 }
 // f(x) += a|x-x0|, \/
 void add_abs(T a, T x0) {
  if (assert(!lt(a, 0)); lt(0, a)) add_max(-a, a, x0);
 }
 // right=false : f(x) +=  inf  (x < x_0), right=true: f(x) += inf  (x_0 < x)
 void add_inf(bool right= false, T x0= 0) { return right ? add_inf<1>(x0) : add_inf<0>(x0); }
 // f(x) <- f(x-x0)
 void shift(T x0) {
  if (bx[0]+= x0, bx[1]+= x0, y-= D(rem) * x0; mn) prop(x0), prop(lr[0], x0), prop(lr[1], x0);
 }
 // rev=false: f(x) <- min_{y<=x} f(y), rev=true : f(x) <- min_{x<=y} f(y)
 void chmin_cum(bool rev= false) {
  if (bf[0] && bf[1] && !lt(bx[0], bx[1])) y+= D(rem) * bx[0], rem= 0;
  else if (bool r= lt(rem, 0); r || lt(0, rem)) {
   T u= (r ? -rem : rem) - o[r] - n[lr[r]].a;
   if (!lt(u, 0)) {
    if (r ^ rev) {
     if (lt(0, u) && bf[r]) {
      D q= n[lr[r]].s + D(n[mn].x) * o[r] + D(u) * bx[r];
      if (r ? y-= q : y+= q; mn) lr[!r]= join(lr[0], mn, lr[1]);
      o[!r]= u, rem= 0, mn= create(u, bx[r]), lr[!rev]= 0, o[!rev]= 0;
     }
    } else {
     assert(bf[r]);
     D q= n[lr[r]].s + D(n[mn].x) * o[r] + D(u) * bx[r];
     (r ? y-= q : y+= q), rem= 0, mn= lr[0]= lr[1]= 0, o[0]= o[1]= 0;
    }
    bf[!rev]= false;
    return;
   }
   if ((r ^ rev)) r ? slope_eval_cum<0, 1>() : slope_eval_cum<0, 0>();
   else r ? slope_eval_cum<1, 1>() : slope_eval_cum<1, 0>();
   if constexpr (persistent) mn= create(o[rev], n[mn].x);
   else n[mn].d= o[rev];
  } else if (mn) {
   if (!lt(0, o[rev])) {
    if (lr[rev]) std::tie(lr[rev], mn)= rev ? pop<0>(lr[rev]) : pop<1>(lr[rev]), o[rev]= n[mn].d;
    else mn= 0;
   } else {
    if constexpr (persistent) mn= create(o[rev], n[mn].x);
    else n[mn].d= o[rev];
   }
  }
  bf[!rev]= false, lr[!rev]= 0, o[!rev]= 0;
 }
 //  f(x) <- min_{lb<=y<=ub} f(x-y). (lb <= ub), \_/ -> \__/
 void chmin_slide_win(T lb, T ub) {
  assert(lb <= ub);
  if (bf[0] && bf[1] && !lt(bx[0], bx[1])) y+= D(rem) * bx[0], rem= 0;
  else {
   if (bool r= lt(rem, 0); r || lt(0, rem)) {
    T u= (r ? -rem : rem) - o[r] - n[lr[r]].a;
    if (lt(0, u)) {
     T b[2]= {lb, ub};
     if (bf[r]) {
      D q= n[lr[r]].s + D(n[mn].x) * o[r] + D(u) * bx[r];
      if (r ? y-= q : y+= q; mn) lr[!r]= join(lr[0], mn, lr[1]), prop<0>(lr[!r], b[!r]);
      lr[r]= 0, rem= 0, o[!r]= u, o[r]= 0, mn= create(u, bx[r] + b[!r]);
     } else {
      y-= D(rem) * b[!r];
      if (mn) prop(b[!r]), prop(lr[0], b[!r]), prop(lr[1], b[!r]);
     }
     bx[0]+= lb, bx[1]+= ub;
     return;
    }
    slope_eval(r);
   }
   if (mn) {
    if (!lt(0, o[0])) prop(ub);
    else if (!lt(0, o[1])) prop(lb);
    else lr[1]= join<0>(0, create(o[1], n[mn].x), lr[1]), prop(lb), n[mn].d= o[0], o[1]= 0;
    prop(lr[0], lb), prop(lr[1], ub);
   }
  }
  bx[0]+= lb, bx[1]+= ub;
 }
 D operator()(T x) { return assert(!bf[0] || bx[0] <= x), assert(!bf[1] || x <= bx[1]), calc_y(x) + D(rem) * x + y; }
 D min() {
  bool r= lt(rem, 0);
  if (!r && !lt(0, rem)) return y;
  T u= (r ? -rem : rem) - o[r] - n[lr[r]].a;
  if (lt(0, u)) {
   assert(bf[r]);
   D q= n[lr[r]].s + D(n[mn].x) * o[r] + D(u) * bx[r];
   return r ? y - q : y + q;
  }
  return slope_eval(r), y;
 }
 std::array<T, 2> argmin() {
  if (bool r= lt(rem, 0); r || lt(0, rem)) {
   if (lt(o[r] + n[lr[r]].a, (r ? -rem : rem))) {
    assert(bf[r]);
    return {bx[r], bx[r]};
   }
   slope_eval(r);
  }
  std::array<T, 2> ret= {bx[0], bx[1]};
  int t= mn;
  if (!t) return ret;
  bool r= lt(0, o[0]);
  if (r && lt(0, o[1])) ret[0]= ret[1]= n[t].x;
  else if (ret[!r]= n[t].x, t= lr[r]; t) {
   for (; push(t), n[t].ch[!r];) t= n[t].ch[!r];
   ret[r]= n[t].x;
  } else assert(bf[r]);
  return ret;
 }
 size_t size() { return n[lr[0]].sz + n[lr[1]].sz + !!mn; }
 PiecewiseLinearConvex &operator+=(const PiecewiseLinearConvex &g) { return *this= *this + g; }
 PiecewiseLinearConvex operator+(PiecewiseLinearConvex g) const {
  PiecewiseLinearConvex ret= *this;
  if (g.bf[0]) ret.add_inf(false, g.bx[0]);
  if (g.bf[1]) ret.add_inf(true, g.bx[1]);
  if (bf[0]) g.add_inf(false, bx[0]);
  if (bf[1]) g.add_inf(true, bx[1]);
  ret.y+= g.y, ret.rem+= g.rem;
  if (!g.mn) return ret;
  if (!ret.mn) return ret.mn= g.mn, ret.lr[0]= g.lr[0], ret.lr[1]= g.lr[1], ret.o[0]= g.o[0], ret.o[1]= g.o[1], ret;
  ret.y+= n[ret.lr[0]].s + D(n[ret.mn].x) * ret.o[0] + n[g.lr[0]].s + D(n[g.mn].x) * g.o[0], ret.rem-= ret.o[0] + n[ret.lr[0]].a + g.o[0] + n[g.lr[0]].a;
  int t= unite(join(ret.lr[0], ret.mn, ret.lr[1]), join(g.lr[0], g.mn, g.lr[1]));
  return std::tie(ret.lr[1], ret.mn)= pop<0>(t), ret.lr[0]= 0, ret.o[0]= 0, ret.o[1]= n[ret.mn].d, ret;
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
};