#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"
#include <iostream>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 SuffixArray SA(S);
 int N= S.length();
 auto LCP= SA.get_lcp();
 long long ans= 0;
 for (int i= 0; i < N; i++) {
  ans+= (N - SA[i]) - LCP[i];
 }
 cout << ans << endl;
 return 0;
}