// competitive-verifier: PROBLEM https://atcoder.jp/contests/past202004-open/tasks/past202004_n
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// 双対
#include <iostream>
#include <algorithm>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct RAQ {
 using T= long long;
 using E= long long;
 static void mp(T &v, E x) { v+= x; }
 static void cp(E &f, E g) { f+= g; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 int N, Q;
 cin >> N >> Q;
 vector<array<int, 4>> query(N);
 for (auto &[x, y, D, C]: query) cin >> x >> y >> D >> C;
 vector<array<int, 2>> v(Q);
 for (int i= 0; i < Q; ++i) cin >> v[i][0] >> v[i][1];
 KDTree<int, 2, RAQ> kdt(v);
 for (auto [x, y, D, C]: query) kdt.apply_cuboid(x, x + D, y, y + D, C);
 for (auto [A, B]: v) cout << kdt.get(A, B) << '\n';
 return 0;
}