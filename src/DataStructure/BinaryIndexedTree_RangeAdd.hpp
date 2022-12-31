#pragma once
#include <bits/stdc++.h>
template <typename T> struct BinaryIndexedTree_RangeAdd {
 std::vector<T> dat1;
 std::vector<T> dat2;
 BinaryIndexedTree_RangeAdd(int n): dat1(n + 1, T(0)), dat2(n + 1, T(0)) {}
 void add_range(int l, int r, T w) {  // add w [l,r)
  for (int k= l + 1; k < (int)dat1.size(); k+= k & -k) dat1[k]-= w * l;
  for (int k= r + 1; k < (int)dat1.size(); k+= k & -k) dat1[k]+= w * r;
  for (int k= l + 1; k < (int)dat2.size(); k+= k & -k) dat2[k]+= w;
  for (int k= r + 1; k < (int)dat2.size(); k+= k & -k) dat2[k]-= w;
 }
 T sum(int x) {  // sum [0,x)
  T s= 0;
  for (int k= x; k > 0; k&= k - 1) s+= dat1[k];
  for (int k= x; k > 0; k&= k - 1) s+= dat2[k] * x;
  return s;
 }
 // sum [l,r)
 T sum(int l, int r) { return sum(r) - sum(l); }
 T operator[](size_t k) { return sum(k + 1) - sum(k); }
};
