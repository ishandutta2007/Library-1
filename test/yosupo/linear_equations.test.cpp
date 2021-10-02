#define PROBLEM "https://judge.yosupo.jp/problem/system_of_linear_equations"
#include <bits/stdc++.h>

#include "src/Math/LUDecomposition.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<998244353>;
  int N, M;
  cin >> N >> M;
  vector<vector<Mint>> A(N, vector<Mint>(M));
  vector<Mint> b(N);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++) cin >> A[i][j];
  for (int i = 0; i < N; i++) cin >> b[i];
  LUDecomposition lu(A);
  auto res = lu.linear_equations(b);
  if (res.empty()) {
    cout << "-1" << '\n';
    return 0;
  }
  auto ker = lu.kernel();
  std::cout << ker.size() << "\n";
  for (int j = 0; j < M; j++) {
    cout << (j ? " " : "") << res[j];
  }
  cout << '\n';
  for (int i = 0; i < ker.size(); i++) {
    for (int j = 0; j < M; j++) {
      cout << (j ? " " : "") << ker[i][j];
    }
    cout << '\n';
  }
  return 0;
}