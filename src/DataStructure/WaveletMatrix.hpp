#pragma once
#include <bits/stdc++.h>
/**
 * @title Wavelet行列
 * @category データ構造
 * 事前に座圧して使う
 */

// BEGIN CUT HERE

struct WaveletMatrix {
  struct SuccinctIndexableDictionary {
    std::size_t length;
    std::size_t blocks;
    std::vector<unsigned> bit, sum;
    SuccinctIndexableDictionary() = default;
    SuccinctIndexableDictionary(std::size_t length)
        : length(length), blocks((length + 31) >> 5) {
      bit.assign(blocks, 0U);
      sum.assign(blocks, 0U);
    }
    void set(int k) { bit[k >> 5] |= 1U << (k & 31); }
    void build() {
      sum[0] = 0U;
      for (std::size_t i = 1; i < blocks; i++)
        sum[i] = sum[i - 1] + __builtin_popcount(bit[i - 1]);
    }
    bool operator[](int k) { return (bool((bit[k >> 5] >> (k & 31)) & 1)); }
    int rank(int k) {
      return (sum[k >> 5]
              + __builtin_popcount(bit[k >> 5] & ((1U << (k & 31)) - 1)));
    }
    int rank(bool val, int k) { return (val ? rank(k) : k - rank(k)); }
  };

 private:
  unsigned sigma, height;
  std::size_t length;
  std::vector<SuccinctIndexableDictionary> matrix;
  std::vector<int> mid;

 private:
  std::pair<int, int> succ(bool f, int l, int r, int level) {
    return {matrix[level].rank(f, l) + mid[level] * f,
            matrix[level].rank(f, r) + mid[level] * f};
  }

 public:
  WaveletMatrix() = default;
  WaveletMatrix(std::vector<int> v)
      : WaveletMatrix(v, *max_element(v.begin(), v.end()) + 1) {}
  WaveletMatrix(std::vector<int> v, unsigned s) : sigma(s), length(v.size()) {
    height = 32 - __builtin_clz(sigma);
    matrix.resize(height);
    mid.resize(height);
    std::vector<int> l(length), r(length);
    for (int level = height - 1; level >= 0; level--) {
      matrix[level] = SuccinctIndexableDictionary(length + 1);
      int left = 0, right = 0;
      for (std::size_t i = 0; i < length; i++) {
        if (((v[i] >> level) & 1))
          matrix[level].set(i), r[right++] = v[i];
        else
          l[left++] = v[i];
      }
      mid[level] = left;
      matrix[level].build();
      v.swap(l);
      for (int i = 0; i < right; i++) v[left + i] = r[i];
    }
  }
  // v[k]
  int access(int k) {
    int ret = 0;
    for (int level = height - 1; level >= 0; level--) {
      bool f = matrix[level][k];
      if (f) ret |= 1 << level;
      k = matrix[level].rank(f, k) + mid[level] * f;
    }
    return ret;
  }
  int operator[](int k) { return access(k); }
  // count i s.t. (0 <= i < r) && v[i] == x
  int rank(int x, int r) {
    int l = 0;
    for (int level = height - 1; level >= 0; level--)
      std::tie(l, r) = succ((x >> level) & 1, l, r, level);
    return r - l;
  }
  // k-th(0-indexed) smallest number in v[l,r)
  int kth_smallest(int l, int r, int k) {
    assert(0 <= k && k < r - l);
    int ret = 0;
    for (int level = height - 1; level >= 0; level--) {
      int cnt = matrix[level].rank(false, r) - matrix[level].rank(false, l);
      bool f = cnt <= k;
      if (f) ret |= 1 << level, k -= cnt;
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }
  // k-th(0-indexed) largest number in v[l,r)
  int kth_largest(int l, int r, int k) {
    return kth_smallest(l, r, r - l - k - 1);
  }
  // count i s.t. (l <= i < r) && (v[i] < upper)
  int range_freq(int l, int r, int upper) {
    upper = std::min(upper, (int)sigma);
    int ret = 0;
    for (int level = height - 1; level >= 0; level--) {
      bool f = ((upper >> level) & 1);
      if (f) ret += matrix[level].rank(false, r) - matrix[level].rank(false, l);
      std::tie(l, r) = succ(f, l, r, level);
    }
    return ret;
  }
  // count i s.t. (l <= i < r) && (lower <= v[i] < upper)
  int range_freq(int l, int r, int lower, int upper) {
    return range_freq(l, r, upper) - range_freq(l, r, lower);
  }
  // max v[i] s.t. (l <= i < r) && (v[i] < upper)
  int prev_value(int l, int r, int upper) {
    int cnt = range_freq(l, r, upper);
    return cnt == 0 ? -1 : kth_smallest(l, r, cnt - 1);
  }
  // std::min v[i] s.t. (l <= i < r) && (lower <= v[i])
  int next_value(int l, int r, int lower) {
    int cnt = range_freq(l, r, lower);
    return cnt == r - l ? -1 : kth_smallest(l, r, cnt);
  }
};
