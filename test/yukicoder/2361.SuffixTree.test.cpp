#define PROBLEM "https://yukicoder.me/problems/no/2361"
#include <iostream>
#include <vector>
#include "src/String/SuffixTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, Q;
 cin >> N >> Q;
 string S;
 cin >> S;
 SuffixArray sa(S);
 LCPArray lcp(sa);
 SuffixTree st(sa, lcp);
 int n= st.size();
 vector<long long> sum(n);
 for (int i= 0; i + 1 < n; ++i) {
  auto [l, r, h, hh]= st[i];
  sum[i + 1]= sum[i] + (long long)(r - l) * (hh - h);
 }
 while (Q--) {
  int L, R;
  cin >> L >> R, --L;
  int len= R - L;
  int v= st.substr(L, len);
  auto [l, r, h, hh]= st[v];
  cout << sum[v] + (long long)(r - l) * (len - h - 1) << '\n';
 }
 return 0;
}