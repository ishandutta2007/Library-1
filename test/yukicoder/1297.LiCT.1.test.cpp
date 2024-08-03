// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1297
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
 cht2.insert(0, 0);
 for (long long i= 0;;) {
  long long a, b, c= C * 2 * i;
  cin >> a >> b, a+= a, b+= b;
  cht1.insert(a - c, cht2.query(-a - c).first + c * (i + 1) + b);
  if (++i == N) break;
  cht2.insert(i, cht1.query(i).first + c * i);
 }
 long long ans= (cht1.query(N).first + (N - 1) * N * C) / 2;
 cout << ans << '\n';
 return 0;
}