// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/abc087/tasks/arc090_b
// ポテンシャルUF
#include <sstream>
#include <string>
#include <cassert>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int N, M;
 scin >> N >> M;
 UnionFind_Potentialized<long long> uf(N);
 bool isok= true;
 for (int i= 0; i < M; ++i) {
  int L, R, D;
  scin >> L >> R >> D, --L, --R;
  isok&= uf.unite(L, R, D);
 }
 scout << (isok ? "Yes" : "No") << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3 3\n1 2 1\n2 3 1\n1 3 2\n", "Yes\n"));
 assert(test(TEST::main, "3 3\n1 2 1\n2 3 1\n1 3 5\n", "No\n"));
 assert(test(TEST::main, "4 3\n2 1 1\n2 3 5\n3 4 2\n", "Yes\n"));
 assert(test(TEST::main, "10 3\n8 7 100\n7 9 100\n9 8 100\n", "No\n"));
 assert(test(TEST::main, "100 0\n", "Yes\n"));
 return 0;
}