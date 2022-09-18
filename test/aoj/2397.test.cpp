#define PROBLEM \
  "https://onlinejudge.u-aizu.ac.jp/challenges/sources/JAG/Spring/2397"
//行列累乗のverify
#include <bits/stdc++.h>
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<int(1e9 + 9)>;
  using Vec = array<Mint, 75>;
  using Mat = SquareMatrix<Mint, 75>;
  long long W, H, N;
  for (int cnt = 0; cin >> W >> H >> N && W;) {
    pair<long long, int> obst[N];
    for (long long i = 0, x, y; i < N; i++)
      cin >> x >> y, obst[i] = make_pair(y - 1, x - 1);
    sort(obst, obst + N);
    Mat A;
    for (int i = 0; i < W; i++) {
      A[i][i] = 1;
      if (i) A[i][i - 1] = 1;
      if (i + 1 < W) A[i][i + 1] = 1;
    }
    Vec b;
    b[0] = 1;
    long long y = 0;
    for (int i = 0; i < N; i++) {
      b = A.pow(obst[i].first - y) * b;
      int j = i;
      while (j < N && obst[i].first == obst[j].first) b[obst[j++].second] = 0;
      i = j - 1;
      y = obst[i].first;
    }
    b = A.pow(H - 1 - y) * b;
    cout << "Case " << ++cnt << ": " << b[W - 1] << '\n';
  }
  return 0;
}