// competitive-verifier: STANDALONE

// https://codeforces.com/contest/622/problem/F
#include <sstream>
#include <string>
#include <cassert>
#include "src/Math/ModInt.hpp"
#include "src/Math/sample_points_shift.hpp"
#include "src/NumberTheory/enumerate_primes.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 using Mint= ModInt<int(1e9 + 7)>;
 int n, k;
 scin >> n >> k;
 auto y= pow_table<Mint>(k + 1, k);
 for (int i= 0; i < k + 1; ++i) y[i + 1]+= y[i];
 scout << sample_points_shift<Mint>(y, n) - y[0] << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "4 1\n", "10\n"));
 assert(test(TEST::main, "4 2\n", "30\n"));
 assert(test(TEST::main, "4 3\n", "100\n"));
 assert(test(TEST::main, "4 0\n", "4\n"));
 return 0;
}