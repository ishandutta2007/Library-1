// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/joisc2019/tasks/joisc2019_a
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include <array>
#include <set>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct RSQ {
 using T= int;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main(stringstream& scin, stringstream& scout) {
 int N, Q;
 scin >> N >> Q;
 vector<array<int, 4>> query;
 set<array<int, 2>> st;
 for (int i= 0; i < N; ++i) {
  int S, T;
  scin >> S >> T;
  st.insert({S, T});
  query.push_back({-(S + T), -1, S, T});
 }
 for (int i= 0; i < Q; ++i) {
  int X, Y, Z;
  scin >> X >> Y >> Z;
  query.push_back({-Z, i, X, Y});
 }
 sort(query.begin(), query.end());
 vector<int> ans(Q);
 KDTree<int, 2, RSQ> kdt(st);
 static constexpr int INF= 0x7fffffff;
 for (auto [z, i, x, y]: query) {
  if (i < 0) kdt.mul(x, y, 1);
  else ans[i]= kdt.prod_cuboid(x, INF, y, INF);
 }
 for (int i= 0; i < Q; ++i) scout << ans[i] << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5 4\n35 100\n70 70\n45 15\n80 40\n20 95\n20 50 120\n10 10 100\n60 60 80\n0 100 100\n", "2\n4\n1\n1\n"));
 assert(test(TEST::main,
             "10 10\n"
             "41304 98327\n"
             "91921 28251\n"
             "85635 59191\n"
             "30361 72671\n"
             "28949 96958\n"
             "99041 37826\n"
             "10245 2726\n"
             "19387 20282\n"
             "60366 87723\n"
             "95388 49726\n"
             "52302 69501 66009\n"
             "43754 45346 3158\n"
             "25224 58881 18727\n"
             "7298 24412 63782\n"
             "24107 10583 61508\n"
             "65025 29140 7278\n"
             "36104 56758 2775\n"
             "23126 67608 122051\n"
             "56910 17272 62933\n"
             "39675 15874 117117\n",
             "1\n3\n5\n8\n8\n3\n3\n3\n5\n6\n"));
 return 0;
}