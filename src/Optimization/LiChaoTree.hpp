#pragma once
#include <limits>
#include <algorithm>
#include "src/Optimization/MinMaxEnum.hpp"
template <typename T, MinMaxEnum obj= MINIMIZE> class LiChaoTree {
 struct Line {
  T a, b;
  inline T get(T x) const { return a * x + b; }
 };
 struct Node {
  Line f;
  Node *ch[2]= {nullptr, nullptr};
 } *root;
 const T L, U, INF;
 static inline int node_count;
 int sgn(const T &x) const {
  if constexpr (std::is_floating_point_v<T>) {
   static constexpr T EPS= 1e-10;
   return x < -EPS ? -1 : x > +EPS ? 1 : 0;
  } else return x < 0 ? -1 : x > 0 ? 1 : 0;
 }
 void addl(Node *&t, Line f, const T &x_l, const T &x_r) {
  if (!t) return t= new Node{f}, void();
  int dif_l= sgn(t->f.get(x_l) - f.get(x_l)), dif_r= sgn(t->f.get(x_r) - f.get(x_r));
  if (dif_l <= 0 && dif_r <= 0) return;
  if (dif_l >= 0 && dif_r >= 0) return t->f= f, void();
  T x_m= (x_l + x_r) / 2;
  int dif_m= sgn(t->f.get(x_m) - f.get(x_m));
  if (dif_m > 0) std::swap(t->f, f), dif_l= -dif_l;
  if (sgn(x_l - x_m) == 0) return;
  if (dif_l > 0) addl(t->ch[0], f, x_l, x_m);
  if (dif_l < 0) addl(t->ch[1], f, x_m, x_r);
 }
 void adds(Node *&t, const Line &f, const T &l, const T &r, const T &x_l, const T &x_r) {
  if (sgn(x_r - l) <= 0 || 0 <= sgn(x_l - r)) return;
  if (0 <= sgn(x_l - l) && sgn(x_r - r) <= 0) return addl(t, f, x_l, x_r);
  if (t && sgn(t->f.get(x_l) - f.get(x_l)) <= 0 && sgn(t->f.get(x_r) - f.get(x_r)) <= 0) return;
  if (!t) t= new Node{Line{0, INF}};
  T x_m= (x_l + x_r) / 2;
  adds(t->ch[0], f, l, r, x_l, x_m), adds(t->ch[1], f, l, r, x_m, x_r);
 }
 inline T query(const Node *t, const T &x_l, const T &x_r, const T &x) const {
  if (!t) return INF;
  if (sgn(x_l - x_r) == 0) return t->f.get(x);
  T x_m= (x_l + x_r) / 2;
  return std::min(t->f.get(x), (sgn(x - x_m) < 0 ? query(t->ch[0], x_l, x_m, x) : query(t->ch[1], x_m, x_r, x)));
 }
public:
 LiChaoTree(T l= -2e9, T u= 2e9, T inf= std::numeric_limits<T>::max() / 2): root{nullptr}, L(l), U(u), INF(inf) {}
 T get_inf() { return INF; }
 // ax+b
 void insert_line(T a, T b) { addl(root, Line{a * obj, b * obj}, L, U); }
 // ax+b for x in [l,r)
 void insert_segment(T l, T r, T a, T b) { adds(root, Line{a * obj, b * obj}, l, r, L, U); }
 T query(T x) const { else return query(root, L, U, x) * obj; }
};
