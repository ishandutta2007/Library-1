#define PROBLEM                                                                \
    "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_3_D"

#include <bits/stdc++.h>
using namespace std;

#define call_from_test
#include "DataStructure/SparseTable.hpp"
#undef call_from_test

signed main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    int N, L;
    cin >> N >> L;
    vector<int> a(N);
    for(int i = 0; i < N; i++)
        cin >> a[i];
    SparseTable<int> st(a);
    for(int i = 0; i + L <= N; i++) {
        if(i)
            cout << " ";
        cout << st.range_min(i, i + L);
    }
    cout << endl;
    return 0;
}