#pragma once
#include <limits>
#include <algorithm>
#include <array>
#include <vector>
#include <numeric>
template <class T, size_t B= 700> class SortedPerBucket {
 static constexpr T INF= std::numeric_limits<T>::max() / 2;
 struct Dat {
  const size_t n;
  T a[B], sorted[B], acc[B + 1];
  T add, lb, ub;
  Dat(size_t b): n(b), a{0}, sorted{0}, acc{0}, add(0), lb(-INF), ub(INF) {}
  Dat(const T *bg, size_t b): n(b), a{0}, acc{0}, add(0), lb(-INF), ub(INF) {
   for (int i= n; i--;) a[i]= *(bg + i);
   build();
  }
  inline bool eval() {
   if (add == 0 && lb == -INF && ub == INF) return false;
   for (auto &x: a) x= std::clamp(x, lb, ub) + add;
   return add= 0, lb= -INF, ub= INF, true;
  }
  inline void build() { std::copy_n(a, B, sorted), std::sort(sorted, sorted + n), std::partial_sum(sorted, sorted + n, acc + 1); }
  inline size_t idx(T x) const { return std::lower_bound(sorted, sorted + n, x) - sorted; }
  inline size_t count(T x) const { return x-= add, (x <= lb ? 0 : ub < x ? n : idx(x)); }
  inline T sum() const {
   size_t l= idx(lb), u= idx(ub);
   return acc[u] - acc[l] + lb * l + ub * (n - u) + add * n;
  }
  inline T sum(T x) const {
   if (x-= add; x <= lb) return 0;
   if (ub < x) return sum();
   size_t l= idx(lb), u= idx(x);
   return acc[u] - acc[l] + lb * l + add * u;
  }
  inline T get(size_t k) const { return std::clamp(a[k], lb, ub) + add; }
 };
 const size_t n;
 std::vector<Dat> dat;
 template <class U, class All, class One> inline U fold(size_t l, size_t r, const All &all, const One &one) const {
  U ret= 0;
  if (size_t i= l / B, j= r / B, k= l % B, m= r % B; i < j) {
   if (k) {
    for (; k < dat[i].n; k++) ret+= one(dat[i].get(k));
    i++;
   }
   for (; i < j; i++) ret+= all(dat[i]);
   if (m)
    for (; m--;) ret+= one(dat[j].get(m));
  } else
   for (; k < m; k++) ret+= one(dat[i].get(k));
  return ret;
 }
 template <class All, class One> inline void update(size_t l, size_t r, const All &all, const One &one) {
  if (size_t i= l / B, j= r / B, k= l % B, m= r % B; i < j) {
   if (k) {
    for (dat[i].eval(); k < dat[i].n; k++) one(dat[i].a[k]);
    dat[i].build(), i++;
   }
   for (; i < j; i++) all(dat[i]);
   if (m) {
    for (dat[j].eval(); m--;) one(dat[j].a[m]);
    dat[j].build();
   }
  } else {
   for (dat[i].eval(); k < m; k++) one(dat[i].a[k]);
   dat[i].build();
  }
 }
public:
 SortedPerBucket(size_t n): n(n) {
  for (int l= 0, r; l < n; l= r) r= std::min(l + B, n), dat.emplace_back(r - l);
 }
 SortedPerBucket(const std::vector<T> &a): n(a.size()) {
  for (int l= 0, r; l < n; l= r) r= std::min(l + B, n), dat.emplace_back(a.data() + l, r - l);
 }
 // count i s.t. (l <= i < r) && (a[i] < ub)
 size_t count(size_t l, size_t r, T ub) const {
  return fold<size_t>(
      l, r, [&](const Dat &d) { return d.count(ub); }, [&](T x) { return x < ub; });
 }
 // count i s.t. (l <= i < r) && (lb <= a[i] < ub)
 size_t count(size_t l, size_t r, T lb, T ub) const { return count(l, r, ub) - count(l, r, lb); }
 // sum a[i] s.t. (l <= i < r)
 T sum(size_t l, size_t r) const {
  return fold<T>(
      l, r, [&](const Dat &d) { return d.sum(); }, [&](T x) { return x; });
 }
 // sum a[i] s.t. (l <= i < r) && (a[i] < ub)
 T sum(size_t l, size_t r, T ub) const {
  return fold<T>(
      l, r, [&](const Dat &d) { return d.sum(ub); }, [&](T x) { return x < ub ? x : 0; });
 }
 // sum a[i] s.t. (l <= i < r)  && (lb <= a[i] < ub)
 T sum(size_t l, size_t r, T lb, T ub) const { return sum(l, r, ub) - sum(l, r, lb); }
 void set(size_t k, T x) {
  auto [i, j]= div(k, B);
  dat[i].eval(), dat[i].a[j]= x, dat[i].build();
 }
 T get(size_t k) const { return dat[k / B].get(k % B); }
 T operator[](size_t k) const { return get(k); }
 void add(size_t l, size_t r, T v) {
  update(
      l, r, [&](Dat &d) { d.add+= v; }, [&](T &x) { x+= v; });
 }
 void chmin(size_t l, size_t r, T ub) {
  auto f= [&](Dat &d) {
   T b= ub - d.add;
   d.ub= std::min(d.ub, b), d.lb= std::min(d.lb, b);
  };
  update(l, r, f, [&](T &x) { x= std::min(x, ub); });
 }
 void chmax(size_t l, size_t r, T lb) {
  auto f= [&](Dat &d) {
   T a= lb - d.add;
   d.lb= std::max(d.lb, a), d.ub= std::max(d.ub, a);
  };
  update(l, r, f, [&](T &x) { x= std::max(x, lb); });
 }
 void chclamp(size_t l, size_t r, T lb, T ub) {
  auto f= [&](Dat &d) {
   T a= lb - d.add, b= ub - d.add;
   d.ub= std::clamp(d.ub, a, b), d.lb= std::clamp(d.lb, a, b);
  };
  update(l, r, f, [&](T &x) { x= std::clamp(x, lb, ub); });
 }
};