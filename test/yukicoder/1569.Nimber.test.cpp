// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1569
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/Math/Nimber.hpp"
#include "src/Math/bostan_mori.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 Nimber::init();
 long long N, K;
 cin >> N >> K;
 vector<Nimber> A(N);
 for (int i= 0; i < N; ++i) cin >> A[i];
 cout << linear_recurrence(vector<Nimber>(N, 1), A, K - 1) << '\n';
 return 0;
}
