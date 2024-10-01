// competitive-verifier: STANDALONE

// https://codeforces.com/contest/1314/problem/F
// nimber log
#include <sstream>
#include <string>
#include <cassert>
#include <cstdint>
#include "src/Math/Nimber.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 Nimber::init();
 int t;
 scin >> t;
 while (t--) {
  Nimber a, b;
  scin >> a >> b;
  auto ans= a.log(b);
  if (ans == (uint64_t)(-1)) scout << -1 << '\n';
  else scout << ans << '\n';
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "7\n2 2\n1 1\n2 3\n8 10\n8 2\n321321321321 2\n123214213213 4356903202345442785\n", "1\n1\n2\n4\n-1\n6148914691236517205\n68943624821423112\n"));
 return 0;
}