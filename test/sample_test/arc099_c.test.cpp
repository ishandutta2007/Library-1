// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/arc099/tasks/arc099_c
// ポテンシャルUF (2色塗り分け+連結成分)

#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <bitset>
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
 bool adj[N][N];
 for (int i= N; i--;)
  for (int j= i; j--;) adj[i][j]= adj[j][i]= 1;
 for (int i= M; i--;) {
  int A, B;
  scin >> A >> B, --A, --B;
  adj[A][B]= adj[B][A]= 0;
 }
 UnionFind_Potentialized<bool> uf(N);
 for (int i= N; i--;)
  for (int j= i; j--;)
   if (adj[i][j])
    if (!uf.unite(i, j, 1)) return scout << "-1\n", 0;
 int cnt[N];
 fill_n(cnt, N, 0);
 for (int i= N; i--;)
  if (uf.potential(i)) ++cnt[uf.leader(i)];
 bitset<701> dp;
 dp[0]= 1;
 for (int i= N; i--;) {
  if (uf.leader(i) != i) continue;
  dp= (dp << cnt[i]) | (dp << (uf.size(i) - cnt[i]));
 }
 int ans= N * N;
 for (int i= N + 1; i--;)
  if (dp[i]) ans= min(ans, i * (i - 1) / 2 + (N - i) * (N - i - 1) / 2);
 scout << ans << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5 5\n1 2\n1 3\n3 4\n3 5\n4 5\n", "4\n"));
 assert(test(TEST::main, "5 1\n1 2\n4 5\n", "-1\n"));
 assert(test(TEST::main, "4 3\n1 2\n1 3\n2 3\n", "3\n"));
 assert(test(TEST::main, "10 39\n7 2\n7 1\n5 6\n5 8\n9 10\n2 8\n8 7\n3 10\n10 1\n8 10\n2 3\n7 4\n3 9\n4 10\n3 4\n6 1\n6 7\n9 5\n9 7\n6 9\n9 4\n4 6\n7 5\n8 3\n2 5\n9 2\n10 7\n8 6\n8 9\n7 3\n5 3\n4 5\n6 3\n2 10\n5 10\n4 2\n6 2\n8 4\n10 6\n", "21\n"));
 return 0;
}