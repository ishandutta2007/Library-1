// competitive-verifier: PROBLEM https://atcoder.jp/contests/agc038/tasks/agc038_c

#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/tables.hpp"
using namespace std;
// O(MAX_A log log MAX_A)

signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 const int MAX_A= 1e6;
 int N;
 cin >> N;
 vector<Mint> c(MAX_A + 1, 0);
 Mint sum= 0;
 for (int i= 0, A; i < N; i++) cin >> A, c[A]+= A, sum+= A;
 auto tmp= gcd_convolve(c, c);
 Mint sum2= 0;
 for (int d= 1; d <= MAX_A; d++) sum2+= tmp[d] / d;
 cout << (sum2 - sum) / 2 << '\n';
 return 0;
}