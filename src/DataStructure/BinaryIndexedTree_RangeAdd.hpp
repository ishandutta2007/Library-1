#pragma once
#include <vector>
template <typename T> class BinaryIndexedTree_RangeAdd {
 std::vector<T> dat1, dat2;
public:
 BinaryIndexedTree_RangeAdd(int n): dat1(n + 1, T()), dat2(n + 1, T()) {}
 void add_range(int l, int r, T w) {  // add w [l,r)
  int n= dat1.size();
  for (int k= l + 1; k < n; k+= k & -k) dat1[k]-= w * l;
  for (int k= r + 1; k < n; k+= k & -k) dat1[k]+= w * r;
  for (int k= l + 1; k < n; k+= k & -k) dat2[k]+= w;
  for (int k= r + 1; k < n; k+= k & -k) dat2[k]-= w;
 }
 T sum(int x) const {  // sum [0,x)
  T s= 0;
  for (int k= x; k; k&= k - 1) s+= dat2[k];
  s*= x;
  for (int k= x; k; k&= k - 1) s+= dat1[k];
  return s;
 }
 T sum(int l, int r) const { return sum(r) - sum(l); }  // sum [l,r)
 T operator[](size_t k) const { return sum(k + 1) - sum(k); }
};
