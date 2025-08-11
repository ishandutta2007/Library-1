// competitive-verifier: PROBLEM https://loj.ac/p/2419
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

// オリジナル: https://usaco.org/index.php?page=viewproblem2&cpid=650

#include <cassert>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
#include "src/Optimization/fibonacci_search.hpp"
using namespace std;
signed main() {
 int N, X, Y, Z;
 cin >> N >> X >> Y >> Z;
 PiecewiseLinearConvex<int> f;
 for (int i= 0; i < N; ++i) {
  int A, B;
  cin >> A >> B;
  f.add_linear(A - B);
  f.add_inf(false, -Y);
  f.add_inf(true, X);
  f.chmin_slide_win(-Z, Z);
 }
 cout << -f.min().value() << '\n';
 return 0;
}