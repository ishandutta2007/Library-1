#pragma once
#include <bits/stdc++.h>
/**
 * @title 典型的な平方分割
 * @category データ構造
 * @see https://rian.hatenablog.jp/entry/2020/10/01/001253
 * @see https://maspypy.com/yukicoder-no-925-%e7%b4%b2%e6%98%9f-extra
 * set - O(B)
 * add, chmin, chmax, chclamp - O(B log B)
 * get - O(1)
 * count, sum - O(B + N/B log B)
 */

// BEGIN CUT HERE
template <class T, std::size_t B = 700>
class SortedPerBucket {
  static constexpr T INF = std::numeric_limits<T>::max() / 2;
  struct Dat {
    const std::size_t n;
    std::array<T, B> a, sorted;
    std::array<T, B + 1> acc;
    T add, lb, ub;
    Dat(std::size_t b)
        : n(b), a{0}, sorted{0}, acc{0}, add(0), lb(-INF), ub(INF) {}
    Dat(const T *bg, std::size_t b)
        : n(b), a{0}, acc{0}, add(0), lb(-INF), ub(INF) {
      for (int i = n; i--;) a[i] = *(bg + i);
      build();
    }
    inline bool eval() {
      if (add == 0 && lb == -INF && ub == INF) return false;
      for (auto &x : a) x = std::clamp(x, lb, ub) + add;
      return add = 0, lb = -INF, ub = INF, true;
    }
    inline void build() {
      sorted = a, std::sort(sorted.begin(), sorted.begin() + n);
      std::partial_sum(sorted.begin(), sorted.begin() + n, acc.begin() + 1);
    }
    inline std::size_t idx(T x) const {
      return std::lower_bound(sorted.begin(), sorted.begin() + n, x) -
             sorted.begin();
    }
    inline std::size_t count(T x) const {
      return x -= add, (x <= lb ? 0 : ub < x ? n : idx(x));
    }
    inline T sum() const {
      std::size_t l = idx(lb), u = idx(ub);
      return acc[u] - acc[l] + lb * l + ub * (n - u) + add * n;
    }
    inline T sum(T x) const {
      if (x -= add; x <= lb) return 0;
      if (ub < x) return sum();
      std::size_t l = idx(lb), u = idx(x);
      return acc[u] - acc[l] + lb * l + add * u;
    }
    inline T get(std::size_t k) const { return std::clamp(a[k], lb, ub) + add; }
  };
  const std::size_t n;
  std::vector<Dat> dat;
  template <class U, class All, class One>
  inline U fold(std::size_t l, std::size_t r, const All &all,
                const One &one) const {
    U ret = 0;
    if (std::size_t i = l / B, j = r / B, k = l % B, m = r % B; i < j) {
      if (k) {
        for (; k < dat[i].n; k++) ret += one(dat[i].get(k));
        i++;
      }
      for (; i < j; i++) ret += all(dat[i]);
      if (m)
        for (; m--;) ret += one(dat[j].get(m));
    } else
      for (; k < m; k++) ret += one(dat[i].get(k));
    return ret;
  }
  template <class All, class One>
  inline void update(std::size_t l, std::size_t r, const All &all,
                     const One &one) {
    if (std::size_t i = l / B, j = r / B, k = l % B, m = r % B; i < j) {
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
  SortedPerBucket(std::size_t n_) : n(n_) {
    for (int l = 0, r; l < n; l = r)
      r = std::min(l + B, n), dat.emplace_back(r - l);
  }
  SortedPerBucket(const std::vector<T> &a) : n(a.size()) {
    for (int l = 0, r; l < n; l = r)
      r = std::min(l + B, n), dat.emplace_back(a.data() + l, r - l);
  }
  // count i s.t. (l <= i < r) && (a[i] < ub)
  std::size_t count(std::size_t l, std::size_t r, T ub) const {
    return fold<std::size_t>(
        l, r, [&](const Dat &d) { return d.count(ub); },
        [&](T x) { return x < ub; });
  }
  // count i s.t. (l <= i < r) && (lb <= a[i] < ub)
  std::size_t count(std::size_t l, std::size_t r, T lb, T ub) const {
    return count(l, r, ub) - count(l, r, lb);
  }
  // sum a[i] s.t. (l <= i < r)
  T sum(std::size_t l, std::size_t r) const {
    return fold<T>(
        l, r, [&](const Dat &d) { return d.sum(); }, [&](T x) { return x; });
  }
  // sum a[i] s.t. (l <= i < r) && (a[i] < ub)
  T sum(std::size_t l, std::size_t r, T ub) const {
    return fold<T>(
        l, r, [&](const Dat &d) { return d.sum(ub); },
        [&](T x) { return x < ub ? x : 0; });
  }
  // sum a[i] s.t. (l <= i < r)  && (lb <= a[i] < ub)
  T sum(std::size_t l, std::size_t r, T lb, T ub) const {
    return sum(l, r, ub) - sum(l, r, lb);
  }
  void set(std::size_t k, T x) {
    std::size_t i = k / B, j = k % B;
    dat[i].eval(), dat[i].a[j] = x, dat[i].build();
  }
  T get(std::size_t k) const { return dat[k / B].get(k % B); }
  T operator[](std::size_t k) const { return get(k); }
  void add(std::size_t l, std::size_t r, T v) {
    update(
        l, r, [&](Dat &d) { d.add += v; }, [&](T &x) { x += v; });
  }
  void chmin(std::size_t l, std::size_t r, T ub) {
    auto f = [&](Dat &d) {
      T b = ub - d.add;
      d.ub = std::min(d.ub, b), d.lb = std::min(d.lb, b);
    };
    update(l, r, f, [&](T &x) { x = std::min(x, ub); });
  }
  void chmax(std::size_t l, std::size_t r, T lb) {
    auto f = [&](Dat &d) {
      T a = lb - d.add;
      d.lb = std::max(d.lb, a), d.ub = std::max(d.ub, a);
    };
    update(l, r, f, [&](T &x) { x = std::max(x, lb); });
  }
  void chclamp(std::size_t l, std::size_t r, T lb, T ub) {
    auto f = [&](Dat &d) {
      T a = lb - d.add, b = ub - d.add;
      d.ub = std::clamp(d.ub, a, b), d.lb = std::clamp(d.lb, a, b);
    };
    update(l, r, f, [&](T &x) { x = std::clamp(x, lb, ub); });
  }
};