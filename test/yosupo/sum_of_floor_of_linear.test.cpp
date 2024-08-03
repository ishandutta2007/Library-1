// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/sum_of_floor_of_linear
#include <iostream>
#include "src/Math/AllPurposeEuclid.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using FST= FloorSumTable<long long, 0, 1>;
 FST::init();
 int T;
 cin >> T;
 while (T--) {
  long long N, M, A, B;
  cin >> N >> M >> A >> B;
  cout << FST::solve(N - 1, A, B, M, 0, 1)[0][1] << '\n';
 }
 return 0;
}