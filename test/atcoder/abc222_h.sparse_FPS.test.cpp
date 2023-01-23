#define PROBLEM "https://atcoder.jp/contests/abc222/tasks/abc222_h"
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 int N;
 cin >> N;
 auto F= sparse_pow<Mint>({1, 3, 1}, 2 * N, N);
 cout << F[N - 1] / N << '\n';
 return 0;
}