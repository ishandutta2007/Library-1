#define PROBLEM "https://judge.yosupo.jp/problem/find_linear_recurrence"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/berlekamp_massey.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 vector<Mint> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 vector<Mint> c= berlekamp_massey(a);
 int d= c.size();
 cout << d << '\n';
 for (int i= 0; i < d; i++) cout << c[i] << " \n"[i == d - 1];
 return 0;
}