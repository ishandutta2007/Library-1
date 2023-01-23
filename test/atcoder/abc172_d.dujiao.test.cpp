#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"
// O(√N)
#include <iostream>
#include "src/Math/DirichletConvSumTable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 long long N;
 cin >> N;
 auto f= get_Id<long long>(N, (int)sqrt(N));
 cout << dirichlet_mul_sum<long long>(f, f) << '\n';
 return 0;
}