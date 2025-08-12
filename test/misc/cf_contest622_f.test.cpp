// competitive-verifier: PROBLEM https://codeforces.com/contest/622/problem/F
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <sstream>
#include <string>
#include <cassert>
#include "src/Math/ModInt.hpp"
#include "src/Math/sample_points_shift.hpp"
#include "src/NumberTheory/enumerate_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 int n, k;
 cin >> n >> k;
 auto y= pow_table<Mint>(k + 1, k);
 for (int i= 0; i < k + 1; ++i) y[i + 1]+= y[i];
 cout << sample_points_shift<Mint>(y, n) - y[0] << '\n';
 return 0;
}