#define PROBLEM "https://yukicoder.me/problems/no/2458"
#include <iostream>
#include <array>
#include <algorithm>
#include "src/Optimization/LiChaoTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 LiChaoTree lct([](long long x, long long a, long long b) { return a * x + b; });
 auto cht= lct.make_tree<MAXIMIZE>();
 long long ans;
 cht.insert(0, 0);
 for (int i= 0;;) {
  long long Q;
  cin >> Q;
  ans= cht.query(Q).first;
  if (++i == N) break;
  cht.insert(Q, ans);
 }
 cout << ans << '\n';
 return 0;
}