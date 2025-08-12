// competitive-verifier: PROBLEM https://loj.ac/p/3350
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 using Mat= Matrix<Mint>;
 long long N, D;
 cin >> N >> D;
 Graph g(N, N - 1);
 for (auto &e: g) cin >> e, --e;
 auto put_edge= [](int, int, Mat x) {
  swap(x[0][0], x[1][0]);
  swap(x[0][1], x[1][1]);
  return x;
 };
 auto op= [](const Mat &l, const Mat &r) {
  Mat ret(2, 2);
  for (int i= 2; i--;)
   for (int j= 2; j--;) {
    ret[i & j][1]+= l[i][1] * r[j][0] + l[i][0] * r[j][1];
    ret[i & j][0]+= l[i][0] * r[j][0];
   }
  return ret;
 };
 Mat ui(2, 2);
 ui[1][0]= 1;
 auto put_vertex= [](int, Mat x) {
  x[0][1]+= 1;
  return x;
 };
 Rerooting<Mat> dp(g, put_edge, op, ui, put_vertex);
 Mat sum(2, 2);
 for (int i= N; i--;) sum+= dp[i];
 auto M= sum;
 M[0][0]*= N, M[1][0]*= N;
 auto A= dp[0];
 A[0][0]*= N, A[1][0]*= N;
 Mint ans= (A * M.pow(D - 1) * sum)[0][0];
 cout << ans << '\n';
 return 0;
}