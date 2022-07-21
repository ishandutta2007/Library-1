#pragma once
#include <bits/stdc++.h>
/**
 * @title Wavelet行列
 * @category データ構造
 * 内部で座圧される
 * ついでに区間内の種類数を答えるクエリも
 * https://www.youtube.com/watch?v=Id7lmNxg21w
 */

// BEGIN CUT HERE

template <class T = std::int64_t>
class WaveletMatrix {
  struct SuccinctIndexableDictionary {
    std::size_t len, blocks, zeros;
    std::vector<unsigned> bit, sum;
    SuccinctIndexableDictionary() = default;
    SuccinctIndexableDictionary(std::size_t len)
        : len(len), blocks((len >> 5) + 1), bit(blocks, 0), sum(blocks, 0) {}
    void set(int k) { bit[k >> 5] |= 1U << (k & 31); }
    void build() {
      for (std::size_t i = 1; i < blocks; i++)
        sum[i] = sum[i - 1] + __builtin_popcount(bit[i - 1]);
      zeros = rank0(len);
    }
    bool operator[](int k) const { return (bit[k >> 5] >> (k & 31)) & 1; }
    std::size_t rank(std::size_t k) const {
      return (sum[k >> 5] +
              __builtin_popcount(bit[k >> 5] & ((1U << (k & 31)) - 1)));
    }
    std::size_t rank0(std::size_t k) const { return k - rank(k); }
  };
  std::size_t len, lg;
  std::vector<SuccinctIndexableDictionary> mat;
  std::vector<T> vec;

 public:
  WaveletMatrix() = default;
  WaveletMatrix(const std::vector<T> &v)
      : len(v.size()), lg(32 - __builtin_clz(len)), mat(lg, len), vec(v) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    std::vector<unsigned> cur(len), nex(len);
    for (int i = len; i--;)
      cur[i] = std::lower_bound(vec.begin(), vec.end(), v[i]) - vec.begin();
    for (auto h = lg; h--; cur.swap(nex)) {
      for (std::size_t i = 0; i < len; i++)
        if ((cur[i] >> h) & 1) mat[h].set(i);
      mat[h].build();
      std::array it{nex.begin(), nex.begin() + mat[h].zeros};
      for (std::size_t i = 0; i < len; i++) *it[mat[h][i]]++ = cur[i];
    }
  }
  // k-th(0-indexed) smallest number in v[l,r)
  T kth_smallest(int l, int r, int k) const {
    assert(k < r - l);
    std::size_t ret = 0;
    for (auto h = lg; h--;)
      if (auto l0 = mat[h].rank0(l), r0 = mat[h].rank0(r); k >= r0 - l0) {
        k -= r0 - l0, ret |= 1 << h;
        l += mat[h].zeros - l0, r += mat[h].zeros - r0;
      } else
        l = l0, r = r0;
    return vec[ret];
  }
  // k-th(0-indexed) largest number in v[l,r)
  T kth_largest(int l, int r, int k) const {
    return kth_smallest(l, r, r - l - k - 1);
  }
  // count i s.t. (l <= i < r) && (v[i] < ub)
  std::size_t count(int l, int r, T ub) const {
    std::size_t x = std::lower_bound(vec.begin(), vec.end(), ub) - vec.begin();
    if (x >= 1 << lg) return r - l;
    if (x == 0) return 0;
    std::size_t ret = 0;
    for (auto h = lg; h--;)
      if (auto l0 = mat[h].rank0(l), r0 = mat[h].rank0(r); (x >> h) & 1)
        ret += r0 - l0, l += mat[h].zeros - l0, r += mat[h].zeros - r0;
      else
        l = l0, r = r0;
    return ret;
  }
  // count i s.t. (l <= i < r) && (lb <= v[i] < ub)
  std::size_t count(int l, int r, T lb, T ub) const {
    return count(l, r, ub) - count(l, r, lb);
  }
};

class DQuery {
  std::vector<int> next;
  WaveletMatrix<int> wm;

 public:
  template <class T>
  DQuery(const std::vector<T> &v) : next(v.size(), -1) {
    std::map<T, int> mp;
    for (int i = v.size(); i--; mp[v[i]] = i)
      if (mp.count(v[i])) next[mp[v[i]]] = i;
    wm = WaveletMatrix(next);
  }
  std::size_t number_of_types(int l, int r) const { return wm.count(l, r, l); }
};
