// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/enumerate_primes
// competitive-verifier: TLE 3
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/NumberTheory/enumerate_primes.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, A, B;
 cin >> N >> A >> B;
 auto ps= enumerate_primes(N);
 int pi= ps.size();
 vector<int> ans;
 for (int i= 0; A * i + B < pi; i++) ans.push_back(ps[A * i + B]);
 int X= ans.size();
 cout << pi << " " << X << '\n';
 for (int i= 0; i < X; i++) cout << ans[i] << " \n"[i == X - 1];
 return 0;
}