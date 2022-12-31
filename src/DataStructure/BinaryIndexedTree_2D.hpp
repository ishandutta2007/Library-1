#pragma once
#include <bits/stdc++.h>
template <typename T> struct BinaryIndexedTree_2D {
 using vT= std::vector<T>;
 std::vector<vT> dat;
 BinaryIndexedTree_2D(int H, int W): dat(std::vector<vT>(H + 1, vT(W + 1, T(0)))) {}
 void add(int y, int x, T v) {
  for (int i= y; i < (int)dat.size(); i+= i & -i)
   for (int j= x; j < (int)dat[i].size(); j+= j & -j) dat[i][j]+= v;
 }
 T sum(int y, int x) {  // sum (0,y] * (0,x]
  T ret= 0;
  for (int i= y; i > 0; i-= i & -i)
   for (int j= x; j > 0; j-= j & -j) ret+= dat[i][j];
  return ret;
 }
 T sum(int sy, int sx, int ty, int tx) {  // sum (sy,ty] * (sx,tx]
  return sum(ty, tx) - sum(ty, sx) - sum(sy, tx) + sum(sy, sx);
 }
};