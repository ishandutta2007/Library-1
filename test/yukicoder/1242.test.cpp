// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1242
#include <iostream>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 vector<int> to(64);
 for (int s= 0; s < 64; ++s) {
  int t= (s << 1) & 63;
  bool isok= true;
  if (!(s & 1) && !(s & 32)) isok= false;
  if (!(s & 2) && !(s & 16)) isok= false;
  if (!(s & 4) && !(s & 8)) isok= false;
  if (isok) t|= 1;
  to[s]= t;
 }
 Period g(to);
 long long N;
 cin >> N;
 int K;
 cin >> K;
 long long A[K];
 for (int i= 0; i < K; ++i) cin >> A[i];
 int s= 63;
 long long cur= N;
 for (int i= K; i--;) {
  s= g.jump(s, cur - A[i]) & 62;
  cur= A[i];
 }
 s= g.jump(s, cur - 1);
 cout << ((s & 1) ? "Yes" : "No") << '\n';
 return 0;
}