// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/3/DSL_3_D
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 128
#include <iostream>
#include <vector>
#include "src/DataStructure/SparseTable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, L;
 cin >> N >> L;
 vector<int> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 SparseTable st(a, [](int l, int r) { return min(l, r); });
 for (int i= 0; i + L <= N; i++) cout << st.prod(i, i + L) << " \n"[i + L == N];
 return 0;
}