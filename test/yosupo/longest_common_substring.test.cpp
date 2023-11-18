#define PROBLEM "https://judge.yosupo.jp/problem/longest_common_substring"
#include <iostream>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S, T;
 cin >> S >> T;
 int N= S.length();
 S+= "$" + T;
 SuffixArray sa(S);
 int a= 0, c= 0, len= 0;
 for (int i= 0; i + 1 < N; ++i) {
  int x= sa.SA[i], y= sa.SA[i + 1];
  if (x > y) swap(x, y);
  if (x < N && N < y && len < sa.LCP[i]) {
   len= sa.LCP[i];
   a= x, c= y - N - 1;
  }
 }
 cout << a << " " << a + len << " " << c << " " << c + len << "\n";
 return 0;
}