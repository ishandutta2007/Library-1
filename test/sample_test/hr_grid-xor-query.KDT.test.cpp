// competitive-verifier: STANDALONE

// https://www.hackerrank.com/contests/happy-query-contest/challenges/grid-xor-query
// range xor
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct RxorQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l ^ r; }
};
signed main(stringstream& scin, stringstream& scout) {
 int N;
 scin >> N;
 vector<array<int, 3>> xyv(N);
 for (int i= 0; i < N; ++i) scin >> xyv[i][0] >> xyv[i][1] >> xyv[i][2];
 KDTree<int, 2, RxorQ> kdt(xyv);
 int Q;
 scin >> Q;
 while (Q--) {
  int a, b, c, d;
  scin >> a >> b >> c >> d;
  scout << kdt.prod_cuboid(a, b, c, d) << '\n';
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main,
             "9\n"
             "2 1 4\n"
             "1 1 1\n"
             "3 2 8\n"
             "1 2 2\n"
             "2 2 5\n"
             "3 3 9\n"
             "2 3 6\n"
             "1 3 3\n"
             "3 1 7\n"
             "7\n"
             "1 1 1 2\n"
             "2 3 2 3\n"
             "1 3 1 3\n"
             "2 2 1 2\n"
             "2 8 2 6\n"
             "1 10000 1 1\n"
             "100 1000 100 1000\n",
             "3\n2\n1\n1\n2\n2\n0\n"));
 return 0;
}