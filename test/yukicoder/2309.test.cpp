#define PROBLEM "https://yukicoder.me/problems/no/2309"
#include <iostream>
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int T;
 cin >> T;
 for (; T--;) {
  int A, B, C, X, Y, Z, W;
  cin >> A >> B >> C >> X >> Y >> Z >> W;
  long long ans= 0;
  for (int a= min(A, B) + 1; a--;) {
   auto f= [&](int b) {
    long long D= min(A - a, C - b), E= B - a - b, F= min(D, E);
    return D * Z + F * max(0, W - Z) + (long long)b * Y + (long long)a * X;
   };
   auto [x, fx]= fibonacci_search<MAXIMIZE>(f, 0, min(B - a, C));
   ans= max(ans, fx);
  }
  cout << ans << '\n';
 }
 return 0;
}
