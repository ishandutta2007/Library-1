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
 SuffixArray sa(S);
 LCPArray lcp(sa);
 int N= S.length();
 long long ans= (long long)N * (N + 1) / 2;
 for (int i= N; --i;) ans-= lcp[i - 1];
 cout << ans << '\n';
 return 0;
}