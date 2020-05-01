/**
 * @title Binary-Indexed-Tree
 * @category データ構造
 * @brief O(logN)
 * @brief 0-indexed
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
    for (int i = 0; i < y.size(); ++i) dat[i + 1] = y[i];
    for (int i = 1; i + (i & -i) < dat.size(); ++i) dat[i + (i & -i)] += dat[i];
  }
  void add(int i, long long a = 1) {
    for (++i; i < dat.size(); i += i & -i) dat[i] += a;
  }
  long long sum(int i) {  // sum [0,i)
    long long s = 0;
    for (; i > 0; i &= i - 1) s += dat[i];
    return s;
  }
  // min { i : sum(i) > k } => kth element(0-indexed)
  int find(long long k) const {
    if (dat.back() <= k++) return -1;  // -1 => no solution
    int i = 0;
    for (int p = 1 << (__lg(dat.size() - 1) + 1); p > 0; p >>= 1)
      if (i + p < dat.size() && dat[i + p] < k) k -= dat[i += p];
    return i;
  }
};
