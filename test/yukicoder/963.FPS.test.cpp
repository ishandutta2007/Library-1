// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/963
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/FFT/FormalPowerSeries.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<1012924417>;
 using FPS= FormalPowerSeries<Mint>;
 FPS f;
 f.reset().set(integ((f * f + 1) / 2) + 1);
 int N;
 cin >> N;
 Mint ans= f[N] * 2;
 for (int i= N; i; i--) ans*= i;
 cout << ans << '\n';
 return 0;
}
