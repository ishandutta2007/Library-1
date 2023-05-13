#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_c"
// O(√N)
#include <iostream>
#include "src/Math/enumerate_quotients.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 long long ans= 0;
 for (auto [q, l, r]: enumerate_quotients(N - 1)) ans+= (r - l) * q;
 cout << ans << '\n';
 return 0;
}