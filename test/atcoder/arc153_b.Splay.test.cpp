#define PROBLEM "https://atcoder.jp/contests/arc153/tasks/arc153_b"

// reverse の verify

#include <iostream>
#include <vector>
#include <numeric>
#include "src/DataStructure/SplayTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Splay= SplayTree<int, true>;
 int H, W;
 cin >> H >> W;
 vector<string> A(H);
 for (int i= 0; i < H; ++i) cin >> A[i];
 Splay row, col;
 {
  vector<int> v(H);
  iota(v.begin(), v.end(), 0);
  row= Splay(v);
 }
 {
  vector<int> v(W);
  iota(v.begin(), v.end(), 0);
  col= Splay(v);
 }
 int Q;
 cin >> Q;
 while (Q--) {
  int a, b;
  cin >> a >> b;
  row.reverse(0, a), row.reverse(a, H);
  col.reverse(0, b), col.reverse(b, W);
 }
 auto row_= row.dump();
 auto col_= col.dump();
 for (int i= 0; i < H; ++i) {
  for (int j= 0; j < W; ++j) {
   cout << A[row_[i]][col_[j]];
  }
  cout << '\n';
 }
 return 0;
}