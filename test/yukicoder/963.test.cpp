// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/963
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/FFT/sequences.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<1012924417>;
 int N;
 cin >> N;
 cout << alternating_permutation<Mint>(N)[N] * 2 << '\n';
 return 0;
}
