#define PROBLEM "https://judge.yosupo.jp/problem/zalgorithm"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "String/z_algorithm.hpp"
#undef call_from_test

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    string s;
    cin >> s;
    vector<int> ans = z_algorithm(s);
    for(int i = 0; i < s.size(); i++) {
        cout << ans[i] << (i == s.size() - 1 ? "\n" : " ");
    }
    return 0;
}