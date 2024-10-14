// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/665
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

// より厳しい制約: http://codeforces.com/contest/622/problem/F
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/enumerate_primes.hpp"
#include "src/Math/sample_points_shift.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 long long n, k;
 cin >> n >> k;
 auto pws= pow_table<Mint>(k + 1, k);
 for (int i= 0; i < k + 1; ++i) pws[i + 1]+= pws[i];
 cout << sample_points_shift<Mint>(pws, n) << '\n';
 return 0;
}