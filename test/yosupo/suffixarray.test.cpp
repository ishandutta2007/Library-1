#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"
#include <iostream>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 int n= S.length();
 SuffixArray sa(S);
 for (int i= 0; i < n; ++i) cout << sa[i] << " \n"[i + 1 == n];
 return 0;
}