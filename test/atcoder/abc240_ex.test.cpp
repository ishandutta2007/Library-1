// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc240/tasks/abc240_Ex
// competitive-verifier: TLE 1
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/abc240/tasks/abc240_h

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "src/String/SuffixArray.hpp"
#include "src/DataStructure/SegmentTree.hpp"
using namespace std;
struct RMQ {
 using T= int;
 static T ti() { return -(1 << 30); }
 static T op(T a, T b) { return max(a, b); }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 string S;
 cin >> S;
 SuffixArray sa(S);
 LCPArray lcp(sa);
 int B= 0;
 for (int n= N; n >= B + 1;) n-= ++B;
 vector<pair<int, int>> subs;
 for (int l= 0; l < N; ++l)
  for (int n= 1; n <= B && l + n <= N; ++n) subs.emplace_back(l, n);
 auto cmp= [&](auto a, auto b) {
  int l= lcp(a.first, b.first);
  if (l < min(a.second, b.second)) return S[a.first + l] < S[b.first + l];
  return a.second == b.second ? a.first > b.first : a.second < b.second;
 };
 stable_sort(subs.begin(), subs.end(), cmp);
 SegmentTree<RMQ> seg(N + 1);
 seg.set(0, 0);
 for (auto [l, n]: subs) {
  int r= l + n;
  seg.set(r, max(seg[r], seg.prod(0, l + 1) + 1));
 }
 cout << seg.prod(0, N + 1) << '\n';
 return 0;
}