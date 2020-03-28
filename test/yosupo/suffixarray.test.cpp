#define PROBLEM "https://judge.yosupo.jp/problem/suffixarray"

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
    for(int i = 0; i < S.length(); i++) {
        if(i)
            cout << " ";
        cout << SA[i];
    }
    cout << endl;
    return 0;
}