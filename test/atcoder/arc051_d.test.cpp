#define PROBLEM "https://atcoder.jp/contests/arc051/tasks/arc051_d"

// double の verify

#include <bits/stdc++.h>
#include "src/DataStructure/LiChaoTree.hpp"

using namespace std;
signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int W, H;
  cin >> W >> H;
  long long a[W], b[H];
  for (int i = 0; i < W; i++) cin >> a[i];
  for (int i = 0; i < H; i++) cin >> b[i];
  long long adp[W][W + 1], bdp[H][H + 1];
  for (int i = 0; i < W; i++) {
    long long sum = 0;
    for (int j = i, cnt = 1; j >= 0; j--, cnt++) {
      sum += a[j];
      if (cnt == i + 1)
        adp[i][cnt] = sum;
      else
        adp[i][cnt] = max(adp[i - 1][cnt], sum);
    }
  }
  for (int i = 0; i < H; i++) {
    long long sum = 0;
    for (int j = i, cnt = 1; j >= 0; j--, cnt++) {
      sum += b[j];
      if (cnt == i + 1)
        bdp[i][cnt] = sum;
      else
        bdp[i][cnt] = max(bdp[i - 1][cnt], sum);
    }
  }
  vector<LiChaoTree<double, 1 << 21>> cht(W);
  for (int i = 0; i < W; i++)
    for (int j = 1; j <= i + 1; j++) cht[i].insert_line(-j, -adp[i][j]);
  int Q;
  cin >> Q;
  while (Q--) {
    int A, B;
    cin >> A >> B;
    A--, B--;
    long long ans = LLONG_MIN;
    for (int i = 1; i <= B + 1; i++)
      ans = max<double>(ans, round(1. * i * -cht[A].query(1. * bdp[B][i] / i)));
    cout << ans << '\n';
  }
  return 0;
}