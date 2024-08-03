// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/737
#include <iostream>
#include <vector>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/Misc/Automaton.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 vector alp= {0, 1};
 int64_t N;
 cin >> N;
 auto tr= [&](int s, int c) {
  if (s <= -1) return s;
  int d= (N >> s) & 1;
  if (c < d) return -1;
  if (c > d) return -2;
  return s - 1;
 };
 Automaton dfa(
     alp, 59, tr, [&](int s) { return s == -1; }, -2);
 using T= array<Mint, 4>;
 auto op= [&](const T &l, const T &r) {
  T ret;
  for (int i= 4; i--;) ret[i]= l[i] + r[i];
  return ret;
 };
 auto f= [&](T x, int c, int) {
  x[3]+= x[3], x[1]+= x[1];
  if (c) x[3]+= x[1] + x[2] + x[0], x[1]+= x[0], x[2]+= x[0];
  return x;
 };
 cout << dfa.dp_run(60, op, T{0, 0, 0, 0}, f, T{1, 0, 0, 0})[3] << '\n';
 return 0;
}