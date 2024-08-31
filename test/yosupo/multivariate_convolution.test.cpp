// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/multivariate_convolution
// competitive-verifier: TLE 1.0
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/MultiVariateConvolution.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<998244353>;
 int k;
 cin >> k;
 vector<int> dim(k);
 for (int i= 0; i < k; i++) cin >> dim[i];
 MultiVariateConvolution mvc(dim);
 int n= mvc.size();
 vector<Mint> f(n), g(n);
 for (int i= 0; i < n; i++) cin >> f[i];
 for (int i= 0; i < n; i++) cin >> g[i];
 auto h= mvc.convolve(f, g);
 for (int i= 0; i < n; i++) cout << h[i] << " \n"[i == n - 1];
 return 0;
}