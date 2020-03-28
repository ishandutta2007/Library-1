#define PROBLEM "https://judge.yosupo.jp/problem/number_of_substrings"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "String/SuffixArray.hpp"
#undef call_from_test

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    string S;
    cin >> S;
    SuffixArray SA(S);
    int N = S.length();
    auto LCP = SA.get_lcp();
    long long ans = 0;
    for(int i = 0; i < N; i++) {
        ans += (N - SA[i]) - LCP[i];
    }
    cout << ans << endl;
    return 0;
}