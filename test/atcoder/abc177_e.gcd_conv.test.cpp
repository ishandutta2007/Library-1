// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc177/tasks/abc177_e
#include <iostream>
#include <vector>
#include <numeric>
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N;
 cin >> N;
 vector<long long> a(1000'010);
 int g= 0;
 for (int i= 0, A; i < N; ++i) cin >> A, ++a[A], g= gcd(g, A);
 if (g != 1) return cout << "not coprime" << '\n', 0;
 auto b= gcd_convolve(a, a);
 cout << (b[1] - a[1] == N * (N - 1) ? "pairwise" : "setwise") << " coprime" << '\n';
 return 0;
}