// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/tkppc6-1/tasks/tkppc6_1_n
// range min, KDT だと TLE
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include "src/DataStructure/SegmentTree_2D.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct RmQ {
 using T= long long;
 static T ti() { return 1ll << 60; }
 static T op(T l, T r) { return min(l, r); }
};
signed main(stringstream& scin, stringstream& scout) {
 int N, K;
 scin >> N >> K;
 vector<array<int, 2>> v(N);
 for (int i= 0; i < N; ++i) v[i]= {i / K, i % K};
 static constexpr long long INF= 1ll << 60;
 SegmentTree_2D<int, RmQ> seg(v);
 seg.set((N - 1) / K, (N - 1) % K, 0);
 int A[N - 1], B[N - 1], C[N - 1];
 for (int i= 0; i + 1 < N; ++i) scin >> A[i] >> B[i] >> C[i];
 for (int i= N - 1; i--;) {
  int h= i / K, w= i % K;
  long long dp;
  if (w + B[i] < K) dp= seg.prod(h, h + A[i] + 1, w, w + B[i] + 1);
  else dp= min(seg.prod(h, h + A[i] + 1, w, K + 1), seg.prod(h + 1, h + A[i] + 2, 0, w + B[i] - K + 1));
  seg.set(h, w, dp + C[i]);
 }
 long long ans= seg.get(0, 0);
 scout << (ans >= INF ? -1 : ans) << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5 2\n1 1 3\n1 0 2\n0 1 3\n5 1 1\n", "4\n"));
 assert(test(TEST::main, "3 2\n0 0 6\n1 1 3\n", "-1\n"));
 assert(test(TEST::main, "10 4\n5 3 892311419\n3 1 517634394\n9 2 78211774\n5 2 247254316\n3 3 238811568\n3 0 654514160\n2 0 680020633\n7 2 683514900\n9 2 540304649\n", "892311419\n"));
 return 0;
}