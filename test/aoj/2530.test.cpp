#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2530"
// ライツアウト
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/GaussianElimination.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using GE = GaussianElimination;
  int R, C;
  cin >> R >> C;
  vector<vector<bool>> A(R * C, vector<bool>(R * C, 0));
  vector<bool> b(R * C);
  for (int i = 0; i < R; i++)
    for (int j = 0, x; j < C; j++) cin >> x, b[i * C + j] = x;
  for (int i = 0; i < R; i++)
    for (int j = 0; j < C; j++)
      for (int di = -1; di <= 1; di++)
        for (int dj = -1; dj <= 1; dj++)
          if (di != 0 || dj != 0)
            for (int ni = i, nj = j; 0 <= ni && ni < R && 0 <= nj && nj < C;
                 ni += di, nj += dj)
              A[i * C + j][ni * C + nj] = 1;
  auto [c, d] = GE::linear_equation(A, b);
  if (!c.empty())
    cout << ModInt<int(1e9 + 9)>(2).pow(d.size()) << endl;
  else
    cout << 0 << endl;
  return 0;
}