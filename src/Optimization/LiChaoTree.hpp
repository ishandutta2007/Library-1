#pragma once
#include <limits>
#include <algorithm>
#include <vector>
#include <tuple>
#include "src/Internal/function_type.hpp"
#include "src/Optimization/MinMaxEnum.hpp"
template <class F> class LiChaoTree {
 using A= argument_type_t<F>;
 static_assert(std::tuple_size_v<A> > 1);
 using T= std::tuple_element_t<0, A>;
 using P= other_than_first_argument_type_t<A>;
 using R= result_type_t<F>;
 F f;
 const T LB, UB;
 std::vector<P> ps;
 template <MinMaxEnum sgn, bool persistent> class LiChaoTreeInterface {
  LiChaoTree *ins;
  struct Node {
   int id= -1;
   Node *ch[2]= {nullptr, nullptr};
  } *root;
  static constexpr R ID= (sgn == MINIMIZE ? std::numeric_limits<R>::max() : std::numeric_limits<R>::lowest()) / 2;
  static inline bool cmp(R p, R n, int pi, int ni) {
   if constexpr (sgn == MINIMIZE) return p > n || (p == n && pi > ni);
   else return p < n || (p == n && pi > ni);
  }
  static inline bool same(T l, T r) {
   if constexpr (std::is_floating_point_v<T>) return std::abs(l - r) < 1e-9;
   else return l == r;
  }
  inline R eval(int id, T x) const { return id < 0 ? ID : std::apply(ins->f, std::tuple_cat(std::make_tuple(x), ins->ps[id])); }
  inline void addl(Node *&t, int id, T xl, T xr) {
   if (!t) return t= new Node{id}, void();
   bool bl= cmp(eval(t->id, xl), eval(id, xl), t->id, id), br= cmp(eval(t->id, xr), eval(id, xr), t->id, id);
   if (!bl && !br) return;
   if constexpr (persistent) t= new Node(*t);
   if (bl && br) return t->id= id, void();
   T xm= (xl + xr) / 2;
   if (cmp(eval(t->id, xm), eval(id, xm), t->id, id)) std::swap(t->id, id), bl= !bl;
   if (!same(xl, xm)) bl ? addl(t->ch[0], id, xl, xm) : addl(t->ch[1], id, xm, xr);
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
   if (!t) return {ID, -1};
   R a= eval(t->id, x);
   if (same(xl, xr)) return {a, t->id};
   T xm= (xl + xr) / 2;
   auto b= x < xm ? query(t->ch[0], x, xl, xm) : query(t->ch[1], x, xm, xr);
   return cmp(a, b.first, t->id, b.second) ? b : std::make_pair(a, t->id);
  }
 public:
  LiChaoTreeInterface()= default;
  LiChaoTreeInterface(LiChaoTree *ins): ins(ins), root(nullptr) {}
  template <class... Args> std::enable_if_t<sizeof...(Args) == std::tuple_size_v<P>, void> insert(Args &&...args) {
   static_assert(std::is_convertible_v<std::tuple<Args...>, P>);
   ins->ps.emplace_back(std::forward<Args>(args)...), addl(root, ins->ps.size() - 1, ins->LB, ins->UB);
  }
  // [l,r)
  template <class... Args> std::enable_if_t<sizeof...(Args) == std::tuple_size_v<P>, void> insert(T l, T r, Args &&...args) {
   static_assert(std::is_convertible_v<std::tuple<Args...>, P>);
   ins->ps.emplace_back(std::forward<Args>(args)...), adds(root, ins->ps.size() - 1, l, r, ins->LB, ins->UB);
  }
  std::pair<R, int> query(T x) const { return query(root, x, ins->LB, ins->UB); }
  const P &params(int id) const { return ins->ps[id]; }
 };
public:
 LiChaoTree(const F &f, T LB= -2e9, T UB= 2e9): f(f), LB(LB), UB(UB) {}
 template <MinMaxEnum sgn= MINIMIZE, bool persistent= false> LiChaoTreeInterface<sgn, persistent> make_tree() { return this; }
};