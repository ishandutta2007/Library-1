#pragma once
#include <vector>
template <typename T> struct BinaryIndexedTree_2D {
 const int h, w;
 std::vector<T> dat;
 BinaryIndexedTree_2D(int H, int W): h(H + 1), w(W + 1), dat(h * w, T()) {}
 void add(int y, int x, T v) {
  for (int i= y; i < h; i+= i & -i)
   for (int j= x, I= i * w; j < w; j+= j & -j) dat[I + j]+= v;
 }
 T sum(int y, int x) const {  // sum (0,y] * (0,x]
  T ret= 0;
  for (int i= y; i; i-= i & -i)
   for (int j= x, I= i * w; j; j-= j & -j) ret+= dat[I + j];
  return ret;
 }
 T sum(int sy, int sx, int ty, int tx) const {  // sum (sy,ty] * (sx,tx]
  return sum(ty, tx) - sum(ty, sx) - sum(sy, tx) + sum(sy, sx);
 }
};