#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_quotients"
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/enumerate_quotients.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N;
 cin >> N;
 auto ans= enumerate_quotients(N);
 for (int i= 0, e= ans.size(); i < e; ++i) cout << get<0>(ans[i]) << " \n"[i == e - 1];
 return 0;
}