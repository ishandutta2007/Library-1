// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1297
// competitive-verifier: TLE 1
#include <iostream>
#include <array>
#include <algorithm>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N, C;
 cin >> N >> C;
 LiChaoTree lct([](long long x, long long a, long long b) { return (__int128_t)a * x + b; }, -1e17, 1e17);
 auto cht1= lct.make_tree<MINIMIZE>(), cht2= lct.make_tree<MINIMIZE>();
 cht1.insert(0, 0);
 for (long long i= 1;; ++i) {
  long long a, b, c= C * 2 * i;
  cin >> a >> b, a+= a, b+= b;
  cht2.insert(a - c, cht1.query(-a - c).first + c * (i - 1) + b);
  if (i == N) break;
  cht1.insert(i, cht2.query(i).first + c * (i + 1));
 }
 long long ans= (cht2.query(N).first + (N + 1) * N * C) / 2;
 cout << ans << '\n';
 return 0;
}