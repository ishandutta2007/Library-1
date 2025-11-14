// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/980
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sparse_fps.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 Mint p;
 cin >> p;
 auto b= sfps::pow_of_div<Mint>({0, 1}, {1, -p, -1}, 2, 2000000);
 int Q;
 cin >> Q;
 while (Q--) {
  int q;
  cin >> q;
  cout << b[q - 2] << '\n';
 }
 return 0;
}
