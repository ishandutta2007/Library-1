#define PROBLEM "https://atcoder.jp/contests/abc179/tasks/abc179_c"
// O(N^(2/3)log^(1/3)N))
#include <iostream>
#include "src/NumberTheory/DirichletSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N;
 cin >> N;
 cout << get_d<long long>(N - 1).sum() << '\n';
 return 0;
}