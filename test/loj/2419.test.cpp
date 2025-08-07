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
 f.add_inf(), f.add_inf(true);
 for (int i= 0; i < N; ++i) {
  int A, B;
  cin >> A >> B;
  f.shift(A - B);
  f.add_linear(Y);
  f.chmin_cum(true);
  f.add_linear(-X - Y);
  f.chmin_cum();
  f.add_linear(X);
  f.add_abs(Z, 0);
 }
 cout << f(0).value() << '\n';
 return 0;
}