// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/hafnian_of_matrix
// competitive-verifier: TLE 2
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Graph/UndirectedGraphSetPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 UndirectedGraphSetPowerSeries g(N);
 for (int i= 0; i < N; i++)
  for (int j= 0; j < N; j++) {
   int a;
   cin >> a;
   if (i < j) g.add_edge(i, j, a);
  }
 cout << g.perfect_matching<Mint>() << '\n';
 return 0;
}