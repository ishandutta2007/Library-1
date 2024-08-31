// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/980
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/convolve.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 Mint p;
 cin >> p;
 const int q_max= 2000000;
 vector<Mint> a(q_max);
 a[0]= 0, a[1]= 1;
 for (int i= 2; i < q_max; i++) a[i]= p * a[i - 1] + a[i - 2];
 auto b= convolve(a, a);
 b.resize(q_max + 1);
 int Q;
 cin >> Q;
 while (Q--) {
  int q;
  cin >> q;
  cout << b[q - 2] << endl;
 }
 return 0;
}
