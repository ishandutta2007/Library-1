// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/zalgorithm
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <string>
#include <vector>
#include "src/String/z_algorithm.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string s;
 cin >> s;
 vector<int> ans= z_algorithm(s);
 int N= s.length();
 for (int i= 0; i < N; i++) cout << ans[i] << " \n"[i == N - 1];
 return 0;
}