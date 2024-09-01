// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc239/tasks/abc239_Ex
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc239/tasks/abc239_h
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/DirichletSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 int N, M;
 cin >> N >> M;
 auto hsum= [&](int) -> Mint { return N; };
 auto gsum= [&](int x) -> Mint { return max(0, N - x); };
 DirichletSeries<Mint> H(M, hsum), G(M, gsum);
 cout << (H / G).sum() << '\n';
 return 0;
}