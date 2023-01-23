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
 SuffixArray SA(S);
 for (int i= 0; i < S.length(); i++) {
  cout << (i ? " " : "") << SA[i];
 }
 cout << endl;
 return 0;
}