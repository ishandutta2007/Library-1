// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/past202203-open/tasks/past202203_o
// ポテンシャルUF (2色塗り分け+連結成分)

#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
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
 UnionFind_Potentialized<bool> uf(N);
 bool odd[N];
 fill_n(odd, N, 0);
 for (int i= 0; i < M; ++i) {
  int A, B;
  scin >> A >> B, --A, --B;
  int x= uf.leader(A), y= uf.leader(B);
  odd[x]= odd[y]= odd[x] | odd[y];
  if (!uf.unite(A, B, 1)) odd[uf.leader(A)]= 1;
 }
 int cnt[N];
 fill_n(cnt, N, 0);
 for (int i= 0; i < N; ++i)
  if (uf.potential(i)) ++cnt[uf.leader(i)];
 int Z= N / 3, X= (N + 2) / 3, Y= (N + 1) / 3;
 bool dp[X + 1][Y + 1];
 for (int i= X + 1; i--;)
  for (int j= Y + 1; j--;) dp[i][j]= 0;
 dp[0][0]= 1;
 for (int i= 0; i < N; ++i) {
  if (uf.leader(i) != i) continue;
  if (odd[i]) {
   Z-= uf.size(i);
   if (Z < 0) return scout << "No" << '\n', 0;
   continue;
  }
  int a= cnt[i], b= uf.size(i) - cnt[i];
  for (int j= X + 1; j--;)
   for (int k= Y + 1; k--;) {
    if (j >= a && k >= b) dp[j][k]|= dp[j - a][k - b];
    if (j >= b && k >= a) dp[j][k]|= dp[j - b][k - a];
   }
 }
 scout << (dp[X][Y] ? "Yes" : "No") << '\n';
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "5 2\n1 2\n2 3\n", "Yes\n"));
 assert(test(TEST::main, "3 2\n1 2\n2 3\n", "No\n"));
 return 0;
}