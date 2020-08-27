/**
 * @title Binary-Indexed-Tree
 * @category データ構造
 *  O(logN)
 *  0-indexed
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

template <typename T>
struct BinaryIndexedTree {
  vector<T> dat;
  BinaryIndexedTree(int n) : dat(n + 1, 0) {}
  BinaryIndexedTree(int n, T a) : BinaryIndexedTree(vector<T>(n, a)) {}
  BinaryIndexedTree(vector<T> y) : dat(y.size() + 1) {
    for (int i = 0; i < y.size(); ++i) dat[i + 1] = y[i];
    for (int i = 1; i + (i & -i) < dat.size(); ++i) dat[i + (i & -i)] += dat[i];
  }
  void add(int i, T a = 1) {
    for (++i; i < (int)dat.size(); i += i & -i) dat[i] += a;
  }
  T sum(int i) {  // sum [0,i)
    T s = 0;
    for (; i > 0; i &= i - 1) s += dat[i];
    return s;
  }
  // min { i : sum(i+1) > k } -> kth element(0-indexed)
  int find(T k) const {
    int i = 0;
    for (int p = 1 << (__lg(dat.size() - 1) + 1); p > 0; p >>= 1)
      if (i + p < (int)dat.size() && dat[i + p] <= k) k -= dat[i += p];
    return i + 1 == (int)dat.size() ? -1 : i;  // -1 -> no solutions
  }
};