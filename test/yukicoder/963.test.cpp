#define PROBLEM "https://yukicoder.me/problems/no/963"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/FFT/fps_sequences.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(0);
  using Mint = StaticModInt<1012924417>;
  int N;
  cin >> N;
  cout << alternating_permutation<Mint>(N)[N] * 2 << '\n';
  return 0;
}
