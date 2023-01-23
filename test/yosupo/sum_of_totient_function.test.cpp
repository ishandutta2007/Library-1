#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_totient_function"
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/DirichletConvSumTable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 long long N;
 cin >> N;
 cout << get_phi<Mint>(N, powl(N, 2. / 3)).sum() << '\n';
 return 0;
}