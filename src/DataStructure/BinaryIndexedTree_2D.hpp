#pragma once
#include <vector>
template <typename T> struct BinaryIndexedTree_2D {
 std::vector<std::vector<T>> dat;
 BinaryIndexedTree_2D(int H, int W): dat(std::vector(H + 1, std::vector(W + 1, T()))) {}
 void add(int y, int x, T v) {
  for (int i= y, e= dat.size(); i < e; i+= i & -i)
   for (int j= x, f= dat[i].size(); j < f; j+= j & -j) dat[i][j]+= v;
 }
 T sum(int y, int x) const {  // sum (0,y] * (0,x]
  T ret= 0;
  for (int i= y; i; i-= i & -i)
   for (int j= x; j; j-= j & -j) ret+= dat[i][j];
  return ret;
 }
 T sum(int sy, int sx, int ty, int tx) const {  // sum (sy,ty] * (sx,tx]
  return sum(ty, tx) - sum(ty, sx) - sum(sy, tx) + sum(sy, sx);
 }
};