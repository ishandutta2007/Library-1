#define PROBLEM "https://yukicoder.me/problems/no/1269"
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/String/AhoCorasick.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 long long N, L, R;
 cin >> N >> L >> R;
 vector<vector<int>> Fs;
 long long fp= 1, fpp= 1;
 if (L <= 1 && 1 <= R) Fs.push_back({1}), Fs.push_back({1});
 while (fp <= R) {
  long long tmp= fpp;
  fpp= fp;
  fp+= tmp;
  if (L <= fp && fp <= R) {
   vector<int> F;
   for (tmp= fp; tmp; tmp/= 10) F.push_back(tmp % 10);
   reverse(F.begin(), F.end());
   Fs.push_back(F);
  }
 }
 AhoCorasick ac(Fs);
 int M= ac.state_size();
 vector<Mint> dp(M);
 dp[0]= 1;
 for (int i= 0; i < N; i++) {
  vector<Mint> next(M);
  for (int s= 0; s < M; s++) {
   for (int k= 0; k < 10; k++) {
    int t= ac.transition(s, k);
    if (ac.is_accept(t)) continue;
    next[t]+= dp[s];
   }
  }
  dp.swap(next);
 }
 cout << accumulate(dp.begin(), dp.end(), Mint(0)) - 1 << '\n';
 return 0;
}