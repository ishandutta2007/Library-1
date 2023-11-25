#pragma once
#include <limits>
#include <algorithm>
#include <vector>
#include <tuple>
#include "src/Internal/function_traits.hpp"
#include "src/Optimization/MinMaxEnum.hpp"
template <class F, class T> class LiChaoTree {};
template <class F, class T, class... Prms> class LiChaoTree<F, std::tuple<T, Prms...>> {
 using R= result_type_t<F>;
 F f;
 const T LB, UB;
 std::vector<std::tuple<Prms...>> ps;
 template <MinMaxEnum sgn, bool persistent> class LiChaoTreeInterface {
  LiChaoTree *ins;
  struct Node {
   int id= -1;
   Node *ch[2]= {nullptr, nullptr};
  } *root;
  inline R eval(int id, T x) const { return std::apply(ins->f, std::tuple_cat(std::make_tuple(x), ins->ps[id])); }
  static inline bool cmp_res(const R &p, const R &n, int pi, int ni) {
   if constexpr (sgn == MINIMIZE) return p > n || (p == n && pi > ni);
   else return p < n || (p == n && pi > ni);
  }
  inline bool cmp(T x, int pi, int ni) const {
   if (pi == -1) return true;
   if (ni == -1) return false;
   return cmp_res(eval(pi, x), eval(ni, x), pi, ni);
  }
  static inline bool end(T l, T r) {
   if constexpr (std::is_floating_point_v<T>) return r - l < 1e-9;
   else return r - l == 1;
  }
  static inline T ub(T r) {
   if constexpr (std::is_floating_point_v<T>) return r;
   else return r - 1;
  }
  inline void addl(Node *&t, int id, T xl, T xr) {
   if (!t) return t= new Node{id}, void();
   T xr_= ub(xr);
   bool bl= cmp(xl, t->id, id), br= cmp(xr_, t->id, id);
   if (!bl && !br) return;
   if constexpr (persistent) t= new Node(*t);
   if (bl && br) return t->id= id, void();
   T xm= (xl + xr) / 2;
   if (cmp(xm, t->id, id)) std::swap(t->id, id), bl= !bl;
   if (!end(xl, xr)) bl ? addl(t->ch[0], id, xl, xm) : addl(t->ch[1], id, xm, xr);
  }
  inline void adds(Node *&t, int id, T l, T r, T xl, T xr) {
   if (r <= xl || xr <= l) return;
   if (l <= xl && xr <= r) return addl(t, id, xl, xr);
   if (!t) t= new Node;
   else if constexpr (persistent) t= new Node(*t);
   T xm= (xl + xr) / 2;
   adds(t->ch[0], id, l, r, xl, xm), adds(t->ch[1], id, l, r, xm, xr);
  }
  inline std::pair<R, int> query(const Node *t, T x, T xl, T xr) const {
   if (!t) return {R(), -1};
   if (end(xl, xr)) return {t->id == -1 ? R() : eval(t->id, x), t->id};
   T xm= (xl + xr) / 2;
   auto b= x < xm ? query(t->ch[0], x, xl, xm) : query(t->ch[1], x, xm, xr);
   if (t->id == -1) return b;
   R a= eval(t->id, x);
   return b.second != -1 && cmp_res(a, b.first, t->id, b.second) ? b : std::make_pair(a, t->id);
  }
 public:
  LiChaoTreeInterface()= default;
  LiChaoTreeInterface(LiChaoTree *ins): ins(ins), root(nullptr) {}
  void insert(const Prms &...args) { ins->ps.emplace_back(args...), addl(root, ins->ps.size() - 1, ins->LB, ins->UB); }
  // [l,r)
  void insert(T l, T r, const Prms &...args) {
   if (l < r) ins->ps.emplace_back(args...), adds(root, ins->ps.size() - 1, l, r, ins->LB, ins->UB);
  }
  std::pair<R, int> query(T x) const { return query(root, x, ins->LB, ins->UB); }
  const std::tuple<Prms...> &params(int id) const { return ins->ps[id]; }
 };
public:
 LiChaoTree(const F &f, T LB= -2e9, T UB= 2e9): f(f), LB(LB), UB(UB) {}
 template <MinMaxEnum sgn= MINIMIZE, bool persistent= false> LiChaoTreeInterface<sgn, persistent> make_tree() { return this; }
};
template <class F, class T, class U> LiChaoTree(F, T, U) -> LiChaoTree<F, argument_type_t<F>>;
template <class F, class T> LiChaoTree(F, T) -> LiChaoTree<F, argument_type_t<F>>;
template <class F> LiChaoTree(F) -> LiChaoTree<F, argument_type_t<F>>;