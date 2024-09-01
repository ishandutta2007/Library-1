// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2114
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 256
#include <iostream>
#include <unordered_map>
#include <array>
#include <vector>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
#include "src/Misc/compress.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, K;
 cin >> N >> M >> K;
 unordered_map<int, array<vector<int>, 2>> mp;
 for (int i= 0; i < N; ++i) {
  int B;
  cin >> B;
  mp[B % K][N >= M].push_back(B / K);
 }
 for (int i= 0; i < M; ++i) {
  int R;
  cin >> R;
  mp[R % K][N < M].push_back(R / K);
 }
 long long ans= 0;
 for (auto &&[_, arr]: mp) {
  auto &&[A, B]= arr;
  if (A.size() > B.size()) return cout << -1 << '\n', 0;
  vector<int> vec(A);
  for (int i: B) vec.push_back(i);
  auto id= compress(vec);
  int n= vec.size();
  vector<int> a(n), b(n);
  for (int i: A) ++a[id(i)];
  for (int i: B) ++b[id(i)];
  PiecewiseLinearConvex<long long> f;
  f.add_inf(true);
  for (int i= 0; i < n; ++i) {
   f.add_inf(true);
   f.add_linear(a[i] - b[i]);
   long long c= vec[i + 1] - vec[i];
   if (i < n - 1) f.chmin_slide_win(-c, c);
  }
  ans-= f.min().value();
 }
 cout << ans << '\n';
 return 0;
}