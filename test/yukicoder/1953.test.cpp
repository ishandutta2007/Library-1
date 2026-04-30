// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1953
// competitive-verifier: TLE 5
// competitive-verifier: MLE 256
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "mylib/counting/Automaton.hpp"
using namespace std;
constexpr int CIRC[10]= {1, 0, 0, 0, 1, 0, 1, 0, 2, 1};

// numbers in [0, N] における「丸の総数」を Automaton で集計
// T = (count_unstarted, count_started, sum_circles_started)
// leading zero は started になるまでカウントしない
static long long getK(long long N) {
 if(N <= 0) return 0;
 string s= to_string(N);
 int n= (int)s.size();
 vector<int> alp= {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
 auto tr= [&](int st, int c) {
  if(st >= n) return st;
  int d= s[st] - '0';
  if(c < d) return n;
  if(c > d) return n + 1;
  return st + 1;
 };
 auto ac= [&](int st) { return st != n + 1; };
 Automaton dfa(alp, 0, tr, ac, n + 1);
 using T= array<__int128, 3>;
 auto op= [](const T& l, const T& r) -> T { return T{l[0] + r[0], l[1] + r[1], l[2] + r[2]}; };
 auto f= [&](const T& v, int d, int) -> T {
  __int128 ncu= (d == 0) ? v[0] : 0;
  __int128 ncs= v[1] + (d != 0 ? v[0] : 0);
  __int128 ns= v[2] + (__int128)CIRC[d] * ncs;
  return T{ncu, ncs, ns};
 };
 T r= dfa.dp_run(n, op, T{0, 0, 0}, f, T{1, 0, 0});
 // 結果は long long に収まる範囲 (N <= 10^18 で getK <= ~10^19 を超えない)
 // 比較目的なので飽和処理
 if(r[2] > (__int128)9'000'000'000'000'000'000LL) return 9'000'000'000'000'000'000LL;
 return (long long)r[2];
}

signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long K;
 cin >> K;
 long long ng= 0, ok= 1'000'000'000'000'000'000LL + 1;
 while(ok - ng > 1) {
  long long mid= ng + (ok - ng) / 2;
  if(getK(mid) >= K) ok= mid;
  else ng= mid;
 }
 if(getK(ok) == K) cout << ok << '\n';
 else cout << -1 << '\n';
 return 0;
}
