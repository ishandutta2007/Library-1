// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/137
#include <iostream>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/FFT/convolve.hpp"
#include "src/FFT/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<1234567891>;
 long long N, M;
 cin >> N >> M;
 std::vector<Mint> f= {1};
 for (int i= 0; i < N; i++) {
  int A;
  cin >> A;
  std::vector<Mint> g(A + 1);
  g[0]= 1, g[A]= -1, f= convolve(f, g);
 }
 cout << div_at<Mint>({1}, f, M) << '\n';
 return 0;
}