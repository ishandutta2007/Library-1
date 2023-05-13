#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"
// O(N^(2/3)log^(1/3)N))
#include <iostream>
#include "src/Math/DirichletSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N;
 cin >> N;
 cout << get_Id<long long>(N).square().sum() << '\n';
 return 0;
}