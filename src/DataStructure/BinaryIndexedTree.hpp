#pragma once
#include <algorithm>
#include <vector>
template <typename T> class BinaryIndexedTree {
 std::vector<T> dat;
public:
 BinaryIndexedTree(int n): dat(n + 1, T()) {}
 BinaryIndexedTree(int n, T a): BinaryIndexedTree(std::vector<T>(n, a)) {}
 BinaryIndexedTree(const std::vector<T>& y): dat(y.size() + 1, 0) {
  for (int i= y.size(); i--;) dat[i + 1]= y[i];
  for (int i= 1, e= dat.size(), j; i < e; ++i)
   if ((j= i + (i & -i)) < e) dat[j]+= dat[i];
 }
 void add(int i, T a= 1) {
  for (++i; i < (int)dat.size(); i+= i & -i) dat[i]+= a;
 }
 T sum(int i) const {  // sum [0,i)
  T s= 0;
  for (; i; i&= i - 1) s+= dat[i];
  return s;
 }
 T sum(int l, int r) const { return sum(r) - sum(l); }  // sum [l,r)
 T operator[](int k) const { return sum(k + 1) - sum(k); }
 int find(T k) const {  // min { i : sum(i+1) > k } -> kth element(0-indexed)
  int i= 0;
  for (int p= 1 << (std::__lg(dat.size() - 1) + 1), e= dat.size(); p; p>>= 1)
   if (i + p < e && dat[i + p] <= k) k-= dat[i+= p];
  return i + 1 == (int)dat.size() ? -1 : i;  // -1 -> no solutions
 }
};