#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2530"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "Math/BitMatrix.hpp"
#include "Math/ModInt.hpp"
#undef call_from_test

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  int R, C;
  cin >> R >> C;
  BitMatrix A(R * C, R * C);
  vector<int> b(R * C);
  for (int i = 0; i < R; i++)
    for (int j = 0; j < C; j++) cin >> b[i * C + j];
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < C; j++) {
      for (int di = -1; di <= 1; di++)
        for (int dj = -1; dj <= 1; dj++)
          if (di != 0 || dj != 0) {
            for (int ni = i, nj = j; 0 <= ni && ni < R && 0 <= nj && nj < C;
                 ni += di, nj += dj) {
              A[i * C + j][ni * C + nj] = 1;
            }
          }
    }
  }
  auto ans = BitMatrix::linear_equations(A, b);
  if (ans.first.size())
    cout << ModInt<int(1e9 + 9)>(2).pow(ans.second.size()) << endl;
  else
    cout << 0 << endl;
  return 0;
}