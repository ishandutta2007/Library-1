// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/2276
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 string S;
 cin >> S;
 vector<int> p;
 for (int i= 0; i < N; ++i)
  if (S[i] == '?') p.push_back(i);
 auto f= [&](int n) {
  for (int i= 0; i < n; ++i) S[p[i]]= 'A';
  for (int i= n; i < (int)p.size(); ++i) S[p[i]]= 'C';
  long long a= 0, ret= 0;
  for (int i= 0; i < N; ++i) {
   if (S[i] == 'A') ++a;
   else ret+= a;
  }
  return ret;
 };
 auto [x, fx]= fibonacci_search<MAXIMIZE>(f, 0, p.size());
 cout << fx << '\n';
 return 0;
}