// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1939
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int N, M;
 cin >> N >> M;
 std::vector<Mint> f(N + 1);
 f[0]= 1;
 for (int i= 0; i < M; i++) {
  int l;
  cin >> l;
  f[l]= 1;
 }
 auto g= sfps::pow(f, N + 1, N);
 cout << g[N] / (N + 1) << '\n';
 return 0;
}