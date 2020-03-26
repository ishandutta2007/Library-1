#define PROBLEM                                                                \
    "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_14_B"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "String/RollingHash.hpp"
#undef call_from_test

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    string T, P;
    cin >> T >> P;
    using RH1 = RollingHash<2020031611>;
    using RH2 = RollingHash<19990929>;
    RH1 rt1(T), rp1(P);
    RH2 rt2(T), rp2(P);
    int N = P.length();
    for(int i = 0; i + N <= T.length(); i++) {
        if(rt1.find(i, i + N) == rp1.find(0, N)
           && rt2.find(i, i + N) == rp2.find(0, N))
            cout << i << "\n";
    }
    cout << flush;
    return 0;
}