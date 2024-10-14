// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/arc033/tasks/arc033_4
#include <sstream>
#include <string>
#include <cassert>
#include <vector>
#include "src/Math/ModInt.hpp"
#include "src/Math/sample_points_shift.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 using Mint= ModInt<int(1e9 + 7)>;
 int N;
 scin >> N;
 vector<Mint> A(N + 1);
 for (int i= 0; i <= N; ++i) scin >> A[i];
 int T;
 scin >> T;
 scout << sample_points_shift<Mint>(A, T) << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "2\n1 3 7\n3\n", "13\n"));
 assert(test(TEST::main, "5\n4 16 106 484 1624 4384\n1000000000\n", "999984471\n"));
 return 0;
}