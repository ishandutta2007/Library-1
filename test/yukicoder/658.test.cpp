#define PROBLEM "https://yukicoder.me/problems/no/658"
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/FFT/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<17>;
 int Q;
 cin >> Q;
 while (Q--) {
  long long n;
  cin >> n;
  cout << linear_recurrence<Mint>({1, 1, 1, 1}, {0, 0, 0, 1}, n - 1) << "\n";
 }
 return 0;
}