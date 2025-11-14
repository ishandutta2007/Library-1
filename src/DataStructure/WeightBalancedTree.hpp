#pragma once
#include <vector>
#include <array>
#include <tuple>
#include <string>
#include <cstddef>
#include <cassert>
#include "src/Internal/detection_idiom.hpp"
template <class M, bool reversible= false, bool persistent= false, size_t LEAF_SIZE= 1 << 20> class WeightBalancedTree {
 _DETECT_BOOL(semigroup, typename T::T, decltype(&T::op));
 _DETECT_BOOL(dual, typename T::T, typename T::E, decltype(&T::mp), decltype(&T::cp));
 _DETECT_BOOL(commute, typename T::commute);
 _DETECT_TYPE(nullptr_or_E, typename T::E, std::nullptr_t, typename T::E);
 _DETECT_TYPE(myself_or_T, typename T::T, T, typename T::T);
 struct NodeMB {
  std::array<int, 2> ch;
  size_t sz;
 };
 template <class D, bool du> struct NodeMD: NodeMB {};
 template <class D> struct NodeMD<D, 1>: NodeMB {
  typename M::E laz;
 };
 template <class D, bool sg, bool rev, bool com> struct NodeMS: NodeMD<D, dual_v<M>> {
  typename M::T sum;
 };
 template <class D, bool rev, bool com> struct NodeMS<D, 0, rev, com>: NodeMD<D, dual_v<M>> {};
 template <class D> struct NodeMS<D, 1, 1, 0>: NodeMD<D, dual_v<M>> {
  typename M::T sum, rsum;
 };
 using NodeM= NodeMS<void, semigroup_v<M>, reversible, commute_v<M>>;
 using T= typename myself_or_T<M>::type;
 using E= typename nullptr_or_E<M>::type;
 using WBT= WeightBalancedTree;
 static inline int nmi= 1, nli= 1;
 static constexpr size_t M_SIZE= LEAF_SIZE * (persistent ? 9 : 2);
 static constexpr size_t L_SIZE= persistent && (dual_v<M> || reversible) ? LEAF_SIZE * 9 : LEAF_SIZE;
 static inline NodeM *nm= new NodeM[M_SIZE];
 static inline T *nl= new T[L_SIZE];
 int root;
 static inline size_t msize(int i) {
  if constexpr (dual_v<M> || reversible) return nm[i].sz & 0x3fffffff;
  else return nm[i].sz;
 }
 static inline size_t size(int i) { return i < 0 ? 1 : msize(i); }
 static inline T sum(int i) { return i < 0 ? nl[-i] : nm[i].sum; }
 static inline T rsum(int i) { return i < 0 ? nl[-i] : nm[i].rsum; }
 template <bool sz= 1> static inline void update(int i) {
  auto t= nm + i;
  auto [l, r]= t->ch;
  if constexpr (sz) t->sz= size(l) + size(r);
  if constexpr (semigroup_v<M>) {
   t->sum= M::op(sum(l), sum(r));
   if constexpr (reversible && !commute_v<M>) t->rsum= M::op(rsum(r), rsum(l));
  }
 }
 static inline void map(T &v, E x, int sz) {
  if constexpr (std::is_invocable_r_v<void, decltype(M::mp), T &, E, int>) M::mp(v, x, sz);
  else M::mp(v, x);
 }
 static inline void propagate(int i, const E &x) {
  auto t= nm + i;
  if (t->sz >> 31) M::cp(t->laz, x);
  else t->laz= x;
  t->sz|= 0x80000000;
  if constexpr (semigroup_v<M>) {
   map(t->sum, x, t->sz & 0x3fffffff);
   if constexpr (reversible && !commute_v<M>) map(t->rsum, x, t->sz & 0x3fffffff);
  }
 }
 static inline void toggle(int i) {
  auto t= nm + i;
  if constexpr (semigroup_v<M> && !commute_v<M>) std::swap(t->sum, t->rsum);
  std::swap(t->ch[0], t->ch[1]), t->sz^= 0x40000000;
 }
 static inline void _push(NodeM *t, int &c) {
  if (c > 0) {
   if constexpr (persistent) nm[nmi]= nm[c], c= nmi++;
   if constexpr (dual_v<M>)
    if (t->sz >> 31) propagate(c, t->laz);
   if constexpr (reversible)
    if (t->sz & 0x40000000) toggle(c);
  } else if constexpr (dual_v<M>)
   if (t->sz >> 31) {
    if constexpr (persistent) nl[nli]= nl[-c], c= -nli++;
    map(nl[-c], t->laz, 1);
   }
 }
 static inline void push(int i) {
  if (auto t= nm + i; t->sz >> 30) {
   auto &[l, r]= t->ch;
   _push(t, l), _push(t, r), t->sz&= 0x3fffffff;
  }
 }
 template <bool r> static inline int join_(int t, int a, int b) {
  if constexpr (dual_v<M> || reversible) push(a);
  if constexpr (r) b= join(b, t, nm[a].ch[0]);
  else b= join(nm[a].ch[1], t, b);
  if constexpr (persistent) nm[nmi]= nm[a], a= nmi++;
  if (size(nm[a].ch[r]) * 4 >= msize(b)) return nm[a].ch[!r]= b, update(a), a;
  return nm[a].ch[!r]= nm[b].ch[r], update(a), nm[b].ch[r]= a, update(b), b;
 }
 template <bool b= 1> static inline int join(int l, int t, int r) {
  int lsz= size(l), rsz= size(r);
  if (lsz > rsz * 4) return join_<0>(t, l, r);
  if (rsz > lsz * 4) return join_<1>(t, r, l);
  return nm[t].ch= {l, r}, update(t), t;
 }
 static inline int merge(int l, int r) { return !l ? r : !r ? l : (++nmi, join(l, nmi - 1, r)); }
 static inline std::array<int, 3> split_(int i, size_t k) {
  if constexpr (dual_v<M> || reversible) push(i);
  auto [l, r]= nm[i].ch;
  size_t lsz= size(l);
  if (k == lsz) return {l, i, r};
  if constexpr (persistent) i= nmi++;
  if (k < lsz) {
   auto [a, b, c]= split_(l, k);
   return {a, b, join(c, i, r)};
  } else {
   auto [a, b, c]= split_(r, k - lsz);
   return {join(l, i, a), b, c};
  }
 }
 static inline std::pair<int, int> split(int i, size_t k) {
  if (k == 0) return {0, i};
  if (k >= size(i)) return {i, 0};
  auto [l, c, r]= split_(i, k);
  return {l, r};
 }
 template <class S> static inline int build(size_t l, size_t r, const S &bg) {
  if (r - l == 1) {
   if constexpr (std::is_same_v<S, T>) return nl[nli]= bg, -nli++;
   else return nl[nli]= *(bg + l), -nli++;
  }
  size_t m= (l + r) / 2, i= nmi++;
  return nm[i].ch= {build(l, m, bg), build(m, r, bg)}, update(i), i;
 }
 static inline void dump(int i, typename std::vector<T>::iterator it) {
  if (i < 0) *it= nl[-i];
  else {
   if constexpr (dual_v<M> || reversible) push(i);
   dump(nm[i].ch[0], it), dump(nm[i].ch[1], it + size(nm[i].ch[0]));
  }
 }
 static inline T prod(int i, size_t l, size_t r) {
  if (i < 0) return nl[-i];
  if (l <= 0 && msize(i) <= r) return nm[i].sum;
  if constexpr (dual_v<M> || reversible) push(i);
  auto [n0, n1]= nm[i].ch;
  size_t lsz= size(n0);
  if (r <= lsz) return prod(n0, l, r);
  if (lsz <= l) return prod(n1, l - lsz, r - lsz);
  return M::op(prod(n0, l, lsz), prod(n1, 0, r - lsz));
 }
 static inline void apply(int &i, size_t l, size_t r, const E &x) {
  if (i < 0) {
   if constexpr (persistent) nl[nli]= nl[-i], i= -nli++;
   map(nl[-i], x, 1);
   return;
  }
  if constexpr (persistent) nm[nmi]= nm[i], i= nmi++;
  if (l <= 0 && msize(i) <= r) return propagate(i, x);
  push(i);
  auto &[n0, n1]= nm[i].ch;
  size_t lsz= size(n0);
  if (r <= lsz) apply(n0, l, r, x);
  else if (lsz <= l) apply(n1, l - lsz, r - lsz, x);
  else apply(n0, l, lsz, x), apply(n1, 0, r - lsz, x);
  if constexpr (semigroup_v<M>) update<0>(i);
 }
 static inline void set_val(int &i, size_t k, const T &x) {
  if (i < 0) {
   if constexpr (persistent) nl[nli]= x, i= -nli++;
   else nl[-i]= x;
   return;
  }
  if constexpr (dual_v<M> || reversible) push(i);
  if constexpr (persistent) nm[nmi]= nm[i], i= nmi++;
  auto &[l, r]= nm[i].ch;
  size_t lsz= size(l);
  lsz > k ? set_val(l, k, x) : set_val(r, k - lsz, x);
  if constexpr (semigroup_v<M>) update<0>(i);
 }
 static inline void mul_val(int &i, size_t k, const T &x) {
  if (i < 0) {
   if constexpr (persistent) nl[nli]= M::op(nl[-i], x), i= -nli++;
   else nl[-i]= M::op(nl[-i], x);
   return;
  }
  if constexpr (dual_v<M> || reversible) push(i);
  if constexpr (persistent) nm[nmi]= nm[i], i= nmi++;
  auto &[l, r]= nm[i].ch;
  size_t lsz= size(l);
  lsz > k ? mul_val(l, k, x) : mul_val(r, k - lsz, x);
  update<0>(i);
 }
 static inline T get_val(int i, size_t k) {
  if (i < 0) return nl[-i];
  if constexpr (dual_v<M> || reversible) push(i);
  auto [l, r]= nm[i].ch;
  size_t lsz= size(l);
  return lsz > k ? get_val(l, k) : get_val(r, k - lsz);
 }
 static inline T &at_val(int i, size_t k) {
  if (i < 0) {
   if constexpr (persistent) return nl[nli++]= nl[-i];
   else return nl[-i];
  }
  if constexpr (dual_v<M> || reversible) push(i);
  if constexpr (persistent) nm[nmi]= nm[i], i= nmi++;
  auto [l, r]= nm[i].ch;
  size_t lsz= size(l);
  return lsz > k ? at_val(l, k) : at_val(r, k - lsz);
 }
 static inline WBT id_to_wbt(int t) {
  WBT ret;
  return ret.root= t, ret;
 }
public:
 WeightBalancedTree(): root(0) {}
 WeightBalancedTree(size_t n, const T &val= T()): root(n ? build(0, n, val) : 0) {}
 WeightBalancedTree(const T *bg, const T *ed): root(bg == ed ? 0 : build(0, ed - bg, bg)) {}
 WeightBalancedTree(const std::vector<T> &ar): WeightBalancedTree(ar.data(), ar.data() + ar.size()) {};
 WBT &operator+=(WBT rhs) { return root= merge(root, rhs.root), *this; }
 WBT operator+(WBT rhs) { return WBT(*this)+= rhs; }
 std::pair<WBT, WBT> split(size_t k) {
  assert(k <= size());
  auto [l, r]= split(root, k);
  return {id_to_wbt(l), id_to_wbt(r)};
 }
 std::tuple<WBT, WBT, WBT> split3(size_t a, size_t b) {
  assert(a < b), assert(b <= size());
  auto [tmp, r]= split(root, b);
  auto [l, c]= split(tmp, a);
  return {id_to_wbt(l), id_to_wbt(c), id_to_wbt(r)};
 }
 size_t size() const { return root ? size(root) : 0; }
 void insert(size_t k, const T &val) {
  auto [l, r]= split(root, k);
  nl[nli]= val, root= merge(merge(l, -nli++), r);
 }
 void push_back(const T &val) { nl[nli]= val, root= merge(root, -nli++); }
 void push_front(const T &val) { nl[nli]= val, root= merge(-nli++, root); }
 T erase(size_t k) {
  assert(k < size());
  auto [l, tmp]= split(root, k);
  auto [t, r]= split(tmp, 1);
  return root= merge(l, r), nl[-t];
 }
 T pop_back() {
  auto [l, t]= split(root, size() - 1);
  return root= l, nl[-t];
 }
 T pop_front() {
  auto [t, r]= split(root, 1);
  return root= r, nl[-t];
 }
 void set(size_t k, const T &val) { set_val(root, k, val); }
 void mul(size_t k, const T &val) {
  static_assert(semigroup_v<M> && commute_v<M>, "\"mul\" is not available\n");
  mul_val(root, k, val);
 }
 T get(size_t k) { return get_val(root, k); }
 T &at(size_t k) {
  static_assert(!semigroup_v<M>, "\"at\" is not available\n");
  return at_val(root, k);
 }
 template <class L= M> std::enable_if_t<semigroup_v<L>, T> operator[](size_t k) { return get(k); }
 template <class L= M> std::enable_if_t<!semigroup_v<L>, T> &operator[](size_t k) { return at(k); }
 T prod(size_t a, size_t b) {
  static_assert(semigroup_v<M>, "\"prod\" is not available\n");
  return prod(root, a, b);
 }
 void apply(size_t a, size_t b, E x) {
  static_assert(dual_v<M>, "\"apply\" is not available\n");
  apply(root, a, b, x);
 }
 void reverse() {
  static_assert(reversible, "\"reverse\" is not available\n");
  if (root <= 0) return;
  if constexpr (persistent) nm[nmi]= nm[root], root= nmi++;
  toggle(root);
 }
 void reverse(size_t a, size_t b) {
  static_assert(reversible, "\"reverse\" is not available\n");
  assert(a < b), assert(b <= size());
  if (b - a == 1) return;
  auto [tmp, r]= split(root, b);
  auto [l, c]= split(tmp, a);
  if constexpr (persistent) nm[nmi]= nm[c], c= nmi++;
  toggle(c);
  root= merge(merge(l, c), r);
 }
 std::vector<T> dump() {
  if (!root) return std::vector<T>();
  std::vector<T> ret(size());
  return dump(root, ret.begin()), ret;
 }
 void clear() { root= 0; }
 static void reset() { nmi= 1, nli= 1; }
 static std::string which_unavailable() {
  std::string ret= "";
  if constexpr (semigroup_v<M>) ret+= "\"at\" ";
  else ret+= "\"prod\" ";
  if constexpr (!semigroup_v<M> || !commute_v<M>) ret+= "\"mul\" ";
  if constexpr (!dual_v<M>) ret+= "\"apply\" ";
  if constexpr (!reversible) ret+= "\"reverse\" ";
  return ret;
 }
 static bool pool_empty() {
  if constexpr (persistent && (dual_v<M> || reversible)) return nmi + LEAF_SIZE >= M_SIZE || nli + LEAF_SIZE >= L_SIZE;
  else return nmi + 1000u >= M_SIZE || nli + 1000u >= L_SIZE;
 }
};