#define PROBLEM                                                                \
    "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_3_D"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/DisjointSparseTable.hpp"
#undef call_from_test

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    int N, L;
    cin >> N >> L;
    vector<int> a(N);
    for(int i = 0; i < N; i++)
        cin >> a[i];
    auto f = [](int a, int b) { return min(a, b); };
    DisjointSparseTable<int> dst(a, f);
    for(int i = 0; i + L <= N; i++) {
        if(i)
            cout << " ";
        cout << dst.query(i, i + L);
    }
    cout << endl;
    return 0;
}