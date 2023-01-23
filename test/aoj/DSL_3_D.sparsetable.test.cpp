#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/courses/library/3/DSL/3/DSL_3_D"
#include <iostream>
#include <vector>
#include "src/DataStructure/SparseTable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, L;
 cin >> N >> L;
 vector<int> a(N);
 for (int i= 0; i < N; i++) cin >> a[i];
 SparseTable<int> st(a);
 for (int i= 0; i + L <= N; i++) {
  cout << (i ? " " : "") << st.range_min(i, i + L);
 }
 cout << endl;
 return 0;
}