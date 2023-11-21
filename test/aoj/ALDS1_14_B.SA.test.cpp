#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/14/ALDS1_14_B"
#include <iostream>
#include <string>
#include "src/String/SuffixArray.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string T, P;
 cin >> T >> P;
 SuffixArray sa(T);
 auto [l, r]= sa.pattern_matching(P);
 vector<int> ans(sa.begin() + l, sa.begin() + r);
 sort(ans.begin(), ans.end());
 for (auto a: ans) cout << a << '\n';
 return 0;
}