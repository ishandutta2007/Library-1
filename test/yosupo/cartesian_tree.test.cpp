// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/cartesian_tree
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Misc/CartesianTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> a(N);
 for (int i= 0; i < N; ++i) cin >> a[i];
 CartesianTree ct(a);
 for (int i= 0; i < N; ++i) cout << (ct.parent(i) == -1 ? i : ct.parent(i)) << " \n"[i == N - 1];
 return 0;
}