#define PROBLEM "https://atcoder.jp/contests/agc035/tasks/agc035_b"
#include <bits/stdc++.h>
#include "Math/ModInt.hpp"
#include "Graph/LinearSystemIncidence.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = ModInt<2>;
  int N, M;
  cin >> N >> M;
  LinearSystemIncidence<Mint> lsi(N);
  vector<Mint> b(N, 0);
  pair<int, int> edges[M];
  for (int i = 0; i < M; i++) {
    int A, B;
    cin >> A >> B;
    edges[i] = make_pair(A, B);
    lsi.add_edge(--A, --B);
    b[B] += 1;
  }
  auto [isok, sol] = lsi.solve(b);
  if (isok) {
    for (int i = 0; i < M; i++) {
      auto [A, B] = edges[i];
      if (sol[i] == Mint(1))
        cout << A << " " << B << '\n';
      else
        cout << B << " " << A << '\n';
    }
  } else
    cout << -1 << '\n';
  return 0;
}