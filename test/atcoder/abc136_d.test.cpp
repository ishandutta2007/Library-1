// competitive-verifier: IGNORE
// competitive-verifier: PROBLEM https://atcoder.jp/contests/abc136/tasks/abc136_d
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include "src/FFT/BigInt.hpp"
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 string S;
 cin >> S;
 int N= S.length();
 vector<int> to(N);
 for (int i= 0; i < N; ++i) to[i]= S[i] == 'L' ? i - 1 : i + 1;
 Period g(to);
 BigInt K("1" + string(100, '0'));
 vector cnt(N, 0);
 for (int i= 0; i < N; ++i) ++cnt[g.jump(i, K)];
 for (int i= 0; i < N; ++i) cout << cnt[i] << " \n"[i == N - 1];
 return 0;
}