#define PROBLEM "https://judge.yosupo.jp/problem/min_plus_convolution_convex_arbitrary"
#include <iostream>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M;
 cin >> N >> M;
 long long a[N], b[M];
 for (int i= 0; i < N; ++i) cin >> a[i];
 for (int j= 0; j < M; ++j) cin >> b[j];
 LiChaoTree lct([&](int i, int j) { return a[i - j] + b[j]; }, 0, N + M - 1);
 auto tree= lct.make_tree<MINIMIZE>();
 for (int j= 0; j < M; ++j) tree.insert(j, N + j, j);
 for (int i= 0; i < N + M - 1; ++i) cout << tree.query(i).first << " \n"[i == N + M - 2];
 return 0;
}