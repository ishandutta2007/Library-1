/**
 * @title Binary-Indexed-Tree(2次元)
 * @category データ構造
 * @brief O(logHW)
 * @brief 1-indexed
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct BinaryIndexedTree_2D {
  using vll = vector<long long>;
  vector<vll> dat;
  BinaryIndexedTree_2D(int H, int W) : dat(vector<vll>(H + 1, vll(W + 1, 0))) {}
  void add(int y, int x, long long v) {
    for (int i = y; i < dat.size(); i += i & -i)
      for (int j = x; j < dat[i].size(); j += j & -j) dat[i][j] += v;
  }
  long long sum(int y, int x) {  // sum (0,y] * (0,x]
    long long ret = 0;
    for (int i = y; i > 0; i -= i & -i)
      for (int j = x; j > 0; j -= j & -j) ret += dat[i][j];
    return ret;
  }
  long long sum(int sy, int sx, int ty, int tx) {  // sum (sy,ty] * (sx,tx]
    return sum(ty, tx) - sum(ty, sx) - sum(sy, tx) + sum(sy, sx);
  }
};