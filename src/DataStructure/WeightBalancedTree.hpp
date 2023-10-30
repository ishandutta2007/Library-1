#pragma once
#include <vector>
#include <array>
#include <string>
#include <tuple>
#include <cstddef>
#include <cassert>
#include "src/Internal/HAS_CHECK.hpp"
template <class M, size_t NODE_SIZE= 1 << 22> class WeightBalancedTree {
 HAS_MEMBER(op);
 HAS_MEMBER(mp);
 HAS_MEMBER(cp);
 HAS_TYPE(T);
 HAS_TYPE(E);
 NULLPTR_OR(E);
 template <class L> static constexpr bool semigroup_v= std::conjunction_v<has_T<L>, has_op<L>>;
 template <class L> static constexpr bool dual_v= std::conjunction_v<has_T<L>, has_E<L>, has_mp<L>, has_cp<L>>;
 struct NodeB {
  size_t sz= 0;
 };
 template <class D, bool sg, bool du> struct NodeD: NodeB {
  inline size_t size() const { return this->sz; }
 };
 template <class D> struct NodeD<D, 1, 0>: NodeB {
  typename M::T val;
  inline size_t size() const { return this->sz; }
 };
 template <class D> struct NodeD<D, 0, 1>: NodeB {
  typename M::E laz;
  inline bool laz_flg() const { return this->sz >> 31; }
  inline size_t size() const { return this->sz & 0x7fffffff; }
 };
 template <class D> struct NodeD<D, 1, 1>: NodeB {
  typename M::T val;
  typename M::E laz;
  inline bool laz_flg() const { return this->sz >> 31; }
  inline size_t size() const { return this->sz & 0x7fffffff; }
 };
 using Node= NodeD<void, semigroup_v<M>, dual_v<M>>;
 using np= Node *;
 struct NodeM: Node {
  np ch[2];
  NodeM() {}
  NodeM(np l, np r): ch{l, r} {}
 };
 template <class D, bool sg, bool du> struct NodeLD: Node {};
 template <class D> struct NodeLD<D, 0, 1>: Node {
  typename M::T val;
 };
 template <class D> struct NodeLD<D, 0, 0>: Node {
  M val;
 };
 using NodeL= NodeLD<void, semigroup_v<M>, dual_v<M>>;
 using T= decltype(NodeL::val);
 using E= nullptr_or_E_t<M>;
 using WBT= WeightBalancedTree;
 static inline int nmi= 0, nli= 0;
 static inline NodeM nm[NODE_SIZE];
 static inline NodeL nl[NODE_SIZE];
 np root;
 static inline np &ch(np t, bool rig) { return ((NodeM *)t)->ch[rig]; }
 static inline np new_nm(np l, np r) { return &(nm[nmi++]= NodeM(l, r)); }
 static inline np new_nl(T x) { return nl[nli]= NodeL(), nl[nli].val= x, nl[nli].sz= 1, nl + nli++; }
 static inline np cp_nm(np &t) { return t= &(nm[nmi++]= NodeM(*((NodeM *)t))); }
 static inline np cp_nl(np &t) { return t= &(nl[nli++]= NodeL(*((NodeL *)t))); }
 static inline np cp_node(np &t) { return t->size() == 1 ? cp_nl(t) : cp_nm(t); }
 static inline void update(np t) {
  if constexpr (dual_v<M>) t->sz= (ch(t, 0)->size() + ch(t, 1)->size()) | (t->sz & 0x80000000);
  else t->sz= ch(t, 0)->size() + ch(t, 1)->size();
  if constexpr (semigroup_v<M>) t->val= M::op(ch(t, 0)->val, ch(t, 1)->val);
 }
 static inline T &reflect(np t) {
  if constexpr (dual_v<M> && !semigroup_v<M>)
   if (t->laz_flg()) M::mp(((NodeL *)t)->val, t->laz, 1), t->sz&= 0x7fffffff;
  return ((NodeL *)t)->val;
 }
 static inline void propagate(np t, const E &x) {
  if (t->laz_flg()) M::cp(t->laz, x);
  else t->laz= x;
  if constexpr (semigroup_v<M>) M::mp(t->val, x, t->size());
  t->sz|= 0x80000000;
 }
 static inline void push(np t) {
  if (t->laz_flg()) propagate(cp_node(ch(t, 0)), t->laz), propagate(cp_node(ch(t, 1)), t->laz), t->sz&= 0x7fffffff;
 }
 template <bool b> static inline np helper(std::array<np, 2> &m) {
  if constexpr (dual_v<M>) push(m[b]);
  np c;
  if constexpr (b) c= submerge({m[0], ch(m[1], 0)});
  else c= submerge({ch(m[0], 1), m[1]});
  if (ch(cp_nm(m[b]), b)->size() * 4 >= c->size()) return ch(m[b], !b)= c, update(m[b]), m[b];
  return ch(m[b], !b)= ch(c, b), update(ch(c, b)= m[b]), update(c), c;
 }
 static inline np submerge(std::array<np, 2> m) {
  if (m[0]->size() > m[1]->size() * 4) return helper<0>(m);
  if (m[1]->size() > m[0]->size() * 4) return helper<1>(m);
  auto t= new_nm(m[0], m[1]);
  return update(t), t;
 }
 static inline np merge(np l, np r) { return !l ? r : !r ? l : submerge({l, r}); }
 static inline std::pair<np, np> split(np t, size_t k) {
  if (!t) return {nullptr, nullptr};
  if (k == 0) return {nullptr, t};
  if (k >= t->size()) return {t, nullptr};
  if constexpr (dual_v<M>) push(t);
  auto l= ch(t, 0), r= ch(t, 1);
  if (size_t lsz= l->size(); k == lsz) return {l, r};
  else if (k < lsz) {
   auto [ll, lr]= split(l, k);
   return {ll, merge(lr, r)};
  } else {
   auto [rl, rr]= split(r, k - lsz);
   return {merge(l, rl), rr};
  }
 }
 template <class S> np build(size_t l, size_t r, const S &bg) {
  if (r - l == 1) {
   if constexpr (std::is_same_v<S, T>) return new_nl(bg);
   else return new_nl(*(bg + l));
  }
  size_t m= (l + r) / 2;
  auto t= new_nm(build(l, m, bg), build(m, r, bg));
  return update(t), t;
 }
 void dump(np t, typename std::vector<T>::iterator it) {
  if (t->size() == 1) *it= reflect(t);
  else {
   if constexpr (dual_v<M>) push(t);
   dump(ch(t, 0), it), dump(ch(t, 1), it + ch(t, 0)->size());
  }
 }
 T fold(np t, size_t l, size_t r) {
  if (l <= 0 && t->size() <= r) return t->val;
  if constexpr (dual_v<M>) push(t);
  size_t lsz= ch(t, 0)->size();
  if (r <= lsz) return fold(ch(t, 0), l, r);
  if (lsz <= l) return fold(ch(t, 1), l - lsz, r - lsz);
  return M::op(fold(ch(t, 0), l, lsz), fold(ch(t, 1), 0, r - lsz));
 }
 void apply(np &t, size_t l, size_t r, const E &x) {
  if (cp_node(t); l <= 0 && t->size() <= r) return propagate(t, x), void();
  push(t);
  size_t lsz= ch(t, 0)->size();
  if (r <= lsz) apply(ch(t, 0), l, r, x);
  else if (lsz <= l) apply(ch(t, 1), l - lsz, r - lsz, x);
  else apply(ch(t, 0), l, lsz, x), apply(ch(t, 1), 0, r - lsz, x);
  if constexpr (semigroup_v<M>) update(t);
 }
 void set_val(np &t, size_t k, const T &x) {
  if (t->size() == 1) return reflect(cp_nl(t))= x, void();
  if constexpr (dual_v<M>) push(t);
  size_t lsz= ch(cp_nm(t), 0)->size();
  lsz > k ? set_val(ch(t, 0), k, x) : set_val(ch(t, 1), k - lsz, x);
  if constexpr (semigroup_v<M>) update(t);
 }
 T get_val(np t, size_t k) {
  if (t->size() == 1) return reflect(t);
  if constexpr (dual_v<M>) push(t);
  size_t lsz= ch(t, 0)->size();
  return lsz > k ? get_val(ch(t, 0), k) : get_val(ch(t, 1), k - lsz);
 }
 T &at_val(np t, size_t k) {
  if (t->size() == 1) return reflect(cp_nl(t));
  if constexpr (dual_v<M>) push(t);
  size_t lsz= ch(cp_nm(t), 0)->size();
  return lsz > k ? at_val(ch(t, 0), k) : at_val(ch(t, 1), k - lsz);
 }
 static WBT id_to_wbt(np t) {
  WBT ret;
  return ret.root= t, ret;
 }
public:
 WeightBalancedTree(): root(nullptr) {}
 WeightBalancedTree(size_t n, T val= T()): root(build(0, n, val)) {}
 WeightBalancedTree(const T *bg, const T *ed): root(build(0, ed - bg, bg)) {}
 WeightBalancedTree(const std::vector<T> &ar): WeightBalancedTree(ar.data(), ar.data() + ar.size()){};
 WBT &operator+=(WBT rhs) { return root= merge(root, rhs.root), *this; }
 WBT operator+(WBT rhs) { return WBT(*this)+= rhs; }
 std::pair<WBT, WBT> split(size_t k) {
  assert(root);
  auto [l, r]= split(root, k);
  return {id_to_wbt(l), id_to_wbt(r)};
 }
 std::tuple<WBT, WBT, WBT> split3(size_t a, size_t b) {
  assert(root), assert(a <= b);
  auto [tmp, r]= split(root, b);
  auto [l, c]= split(tmp, a);
  return {id_to_wbt(l), id_to_wbt(c), id_to_wbt(r)};
 }
 size_t size() const { return root ? root->size() : 0; }
 void insert(size_t k, T val) {
  auto [l, r]= split(root, k);
  root= merge(merge(l, new_nl(val)), r);
 }
 void push_back(T val) { root= merge(root, new_nl(val)); }
 void push_front(T val) { root= merge(new_nl(val), root); }
 T erase(size_t k) {
  assert(k < size());
  auto [l, tmp]= split(root, k);
  auto [t, r]= split(tmp, 1);
  return root= merge(l, r), reflect(t);
 }
 T pop_back() {
  auto [l, t]= split(root, size() - 1);
  return root= l, reflect(t);
 }
 T pop_front() {
  auto [t, r]= split(root, 1);
  return root= r, reflect(t);
 }
 void set(size_t k, T val) { set_val(root, k, val); }
 T get(size_t k) { return get_val(root, k); }
 T &at(size_t k) {
  static_assert(!semigroup_v<M>, "\"at\" is not available\n");
  return at_val(root, k);
 }
 template <class L= M> std::enable_if_t<semigroup_v<L>, T> operator[](size_t k) { return get(k); }
 template <class L= M> std::enable_if_t<!semigroup_v<L>, T> &operator[](size_t k) { return at(k); }
 T fold(size_t a, size_t b) {
  static_assert(semigroup_v<M>, "\"fold\" is not available\n");
  return fold(root, a, b);
 }
 void apply(size_t a, size_t b, E x) {
  static_assert(dual_v<M>, "\"apply\" is not available\n");
  apply(root, a, b, x);
 }
 std::vector<T> dump() {
  if (!root) return std::vector<T>();
  std::vector<T> ret(size());
  return dump(root, ret.begin()), ret;
 }
 void clear() { root= nullptr; }
 static void reset() { nmi= 0, nli= 0; }
 void rebuild() {
  auto dmp= dump();
  reset(), *this= WBT(dmp);
 }
 static std::string which_available() {
  std::string ret= "";
  if constexpr (semigroup_v<M>) ret+= "\"fold\" ";
  else ret+= "\"at\" ";
  if constexpr (dual_v<M>) ret+= "\"apply\" ";
  return ret;
 }
 static double percentage_used() { return 100. * std::max(nmi, nli) / NODE_SIZE; }
};