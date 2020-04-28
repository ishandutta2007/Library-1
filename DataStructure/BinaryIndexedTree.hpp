/**
 * @title Binary-Indexed-Tree
 * @category データ構造
 * @brief O(logN)
 * @brief 0-indexed
 * @brief lower_bound(a): min { k : sum(k) >= a }
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct BinaryIndexedTree {
  vector<long long> dat;
  BinaryIndexedTree(int n) : dat(n + 1, 0) {}
  BinaryIndexedTree(int n, long long a)
      : BinaryIndexedTree(vector<long long>(n, a)) {}
  BinaryIndexedTree(vector<long long> y) : dat(y.size() + 1) {
    for (int k = 0; k < y.size(); ++k) dat[k + 1] = y[k];
    for (int k = 1; k + (k & -k) < dat.size(); ++k) dat[k + (k & -k)] += dat[k];
  }
  void add(int k, long long a) {
    for (++k; k < dat.size(); k += k & -k) dat[k] += a;
  }
  // sum [0,k)
  long long operator[](int k) {
    long long s = 0;
    for (; k > 0; k &= k - 1) s += dat[k];
    return s;
  }
  int lower_bound(long long a) const {
    int k = 0;
    for (int p = 1 << (__lg(dat.size() - 1) + 1); p > 0; p >>= 1)
      if (k + p < dat.size() && dat[k + p] < a) a -= dat[k += p];
    return k;
  }
};
