#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2397"

//行列の高速累乗のverify

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/Matrix.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<int(1e9 + 9)>;
  int W, N;
  long long H;
  int cnt = 0;
  while (cin >> W >> H >> N && W) {
    pair<long long, int> obst[N];
    for (int i = 0; i < N; i++) {
      int x;
      long long y;
      cin >> x >> y;
      obst[i] = make_pair(y - 1, x - 1);
    }
    sort(obst, obst + N);
    Matrix<Mint> A(W);
    for (int i = 0; i < W; i++) {
      A[i][i] = 1;
      if (i) A[i][i - 1] = 1;
      if (i + 1 < W) A[i][i + 1] = 1;
    }
    vector<Mint> b(W);
    b[0] = 1;
    long long y = 0;
    for (int i = 0; i < N; i++) {
      b = A.pow(obst[i].first - y) * b;
      int j = i;
      while (j < N && obst[i].first == obst[j].first) {
        b[obst[j++].second] = 0;
      }
      i = j - 1;
      y = obst[i].first;
    }
    b = A.pow(H - 1 - y) * b;
    cout << "Case " << ++cnt << ": " << b[W - 1] << endl;
  }
  return 0;
}