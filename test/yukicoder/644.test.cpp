// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/644
// competitive-verifier: TLE 0.5
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 int N, M;
 cin >> N >> M;
 auto phi= totient_table(N);
 Mint ans= 0;
 for (int i= 2; i <= N / M; i++) ans+= phi[i];
 for (int i= 1; i <= N - 2; i++) ans*= i;
 ans*= 2;
 cout << ans << '\n';
 return 0;
}