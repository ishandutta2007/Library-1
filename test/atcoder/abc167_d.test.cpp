#define PROBLEM "https://atcoder.jp/contests/abc167/tasks/abc167_d"
#include <iostream>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 long long K;
 cin >> N >> K;
 vector<int> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i], --A[i];
 cout << Period(A).jump(0, K) + 1 << '\n';
 return 0;
}