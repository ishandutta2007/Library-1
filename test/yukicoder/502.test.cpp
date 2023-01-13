#define PROBLEM "https://yukicoder.me/problems/no/502"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/polynomial_matrix_prod.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 static constexpr int MOD= 1e9 + 7;
 using Mint= ModInt<MOD>;
 long long n;
 cin >> n;
 if (n >= MOD) {
  cout << 0 << '\n';
 } else {
  cout << polynomial_matrix_prod<Mint>({{{1, 1}}}, n)[0][0] << '\n';
 }
 return 0;
}
