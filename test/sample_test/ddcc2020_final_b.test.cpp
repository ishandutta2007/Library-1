// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/ddcc2020-final/tasks/ddcc2020_final_b
// max plus semiring affine
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <array>
#include <cstdint>
#include "src/Math/Algebra.hpp"
#include "src/LinearAlgebra/Matrix.hpp"
using namespace std;
bool test(int (*solve)(stringstream &, stringstream &), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct Aff {
 using T= array<int64_t, 2>;
 static constexpr int64_t INF= 1ll << 62;
 static constexpr T o= {-INF, -INF}, i= {0ll, -INF};
 static T add(const T &vl, const T &vr) { return {max(vl[0], vr[0]), max(vl[1], vr[1])}; }
 static T mul(const T &vl, const T &vr) {
  if (vl == o) return o;
  if (vr == o) return o;
  return {vl[0] + vr[0], max(vl[1], vl[0] + vr[1])};
 }
};
signed main(stringstream &scin, stringstream &scout) {
 using Rig= Algebra<Aff>;
 using Mat= Matrix<Rig>;
 int64_t N, M, W, S, K;
 scin >> N >> M >> W >> S >> K, --S;
 Mat A(N, N);
 for (int64_t u, v, w; M--;) scin >> u >> v >> w, A[--v][--u]= array<int64_t, 2>{w, 0ll};
 A= A.pow(K);
 int64_t ans= -1;
 for (int i= 0; i < N; i++) ans= max(ans, max(A[i][S].x[0] + W, A[i][S].x[1]));
 scout << ans << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5 6 0 1 5\n1 2 1\n2 3 2\n3 4 1\n4 1 -1\n3 1 -4\n2 5 7\n", "8\n"));
 assert(test(TEST::main, "5 6 10 1 4\n1 2 1\n1 3 3\n2 4 6\n5 4 0\n3 5 2\n2 5 1\n", "-1\n"));
 assert(test(TEST::main, "2 2 0 2 1000000000\n1 2 100000\n2 1 100000\n", "100000000000000\n"));
 return 0;
}