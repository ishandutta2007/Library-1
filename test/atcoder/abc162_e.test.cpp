#define PROBLEM "https://atcoder.jp/contests/abc162/tasks/abc162_e"
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/tables.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9) + 7>;
 int N, K;
 cin >> N >> K;
 vector<Mint> a(K + 1);
 for (int i= 1; i <= K; ++i) a[i]= Mint(K / i).pow(N);
 multiple_mobius(a);
 Mint ans= 0;
 for (int i= 1; i <= K; ++i) ans+= a[i] * i;
 cout << ans << '\n';
 return 0;
}