// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/static_range_inversions_query
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Misc/compress.hpp"
#include "src/Misc/Mo.hpp"
#include "src/DataStructure/BinaryIndexedTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 int A[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 vector<int> v(A, A + N);
 auto id= compress(v);
 for (int i= 0; i < N; i++) A[i]= id(A[i]);
 Mo mo;
 for (int q= 0; q < Q; q++) {
  int l, r;
  cin >> l >> r;
  mo.query(l, r);
 }
 BinaryIndexedTree<long long> bit(N + 1);
 long long inv= 0, total= 0;
 auto addl= [&](int i) { inv+= bit.sum(A[i]), bit.add(A[i], 1), total++; };
 auto addr= [&](int i) { inv+= total - bit.sum(A[i] + 1), bit.add(A[i], 1), total++; };
 auto erasel= [&](int i) { inv-= bit.sum(A[i]), bit.add(A[i], -1), total--; };
 auto eraser= [&](int i) { inv-= total - bit.sum(A[i] + 1), bit.add(A[i], -1), total--; };
 long long ans[Q];
 auto out= [&](int q) { ans[q]= inv; };
 mo.run(addl, addr, erasel, eraser, out);
 for (int q= 0; q < Q; q++) cout << ans[q] << "\n";
 return 0;
}