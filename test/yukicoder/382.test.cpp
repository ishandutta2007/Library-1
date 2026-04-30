// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/382
// competitive-verifier: TLE 5
// competitive-verifier: MLE 256
#include <iostream>
#include "mylib/graph/CliqueProblem.hpp"
using namespace std;
long long S;
int gen() { return (int)(S= S * 12345 % 1000003); }
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 cin >> S;
 int N= gen() % 120 + 2;
 int P= gen();
 CliqueProblem cp(N);
 for(int i= 0; i < N; ++i)
  for(int j= i + 1; j < N; ++j) {
   int k= gen();
   if(k >= P) cp.add_edge(i, j);
  }
 auto mis= cp.get_max_independent_set();
 cout << (int)mis.size() + 1 << '\n';
 for(size_t i= 0; i < mis.size(); ++i) cout << (mis[i] + 1) << " \n"[i + 1 == mis.size()];
 return 0;
}
