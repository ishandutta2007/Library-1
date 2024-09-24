// competitive-verifier: STANDALONE

// https://atcoder.jp/contests/abc155/tasks/abc155_f
// sp judge
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <vector>
#include "src/Graph/incidence_matrix_equation.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 if (expected == "-1\n") return scout.str() == expected;
 stringstream scin2(in);
 int N, M;
 scin2 >> N >> M;
 int A[N], B[N];
 for (int i= 0; i < N; ++i) scin2 >> A[i] >> B[i];
 int L[M], R[M];
 for (int i= 0; i < M; ++i) scin2 >> L[i] >> R[i];
 int k;
 scout >> k;
 for (int i= 0; i < k; ++i) {
  int j;
  scout >> j, --j;
  for (int x= 0; x < N; ++x)
   if (L[j] <= A[x] && A[x] <= R[j]) B[x]^= 1;
 }
 for (int x= 0; x < N; ++x)
  if (B[x]) return false;
 return true;
}
namespace TEST {
signed main(stringstream& scin, stringstream& scout) {
 int N, M;
 scin >> N >> M;
 pair<int, bool> AB[N + 2];
 AB[0]= {0, 0}, AB[N + 1]= {1e9 + 10, 0};
 for (int i= 1; i <= N; i++) scin >> AB[i].first >> AB[i].second;
 sort(AB, AB + N + 2);
 vector<bool> b(N + 1);
 for (int i= 0; i <= N; i++) b[i]= AB[i + 1].second ^ AB[i].second;
 Graph g(N + 1);
 for (int i= 0; i < M; i++) {
  int L, R;
  scin >> L >> R;
  L= lower_bound(AB, AB + N + 2, pair(L, false)) - AB;
  R= upper_bound(AB, AB + N + 2, pair(R, true)) - AB;
  g.add_edge(L - 1, R - 1);
 }
 auto sol= incidence_matrix_equation(g, b);
 if (sol.empty()) return scout << -1 << '\n', 0;
 vector<int> ans;
 for (int i= 0; i < M; i++)
  if (sol[i]) ans.emplace_back(i + 1);
 int k= ans.size();
 scout << k << '\n';
 for (int i= 0; i < k; i++) scout << ans[i] << " \n"[i == k - 1];
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "3 4\n5 1\n10 1\n8 0\n1 10\n4 5\n6 7\n8 9\n", "2\n1 4\n"));
 assert(test(TEST::main, "4 2\n2 0\n3 1\n5 1\n7 0\n1 4\n4 7\n", "-1\n"));
 assert(test(TEST::main, "3 2\n5 0\n10 0\n8 0\n6 9\n66 99\n", "0\n"));
 assert(test(TEST::main,
             "12 20\n"
             "536130100 1\n"
             "150049660 1\n"
             "79245447 1\n"
             "132551741 0\n"
             "89484841 1\n"
             "328129089 0\n"
             "623467741 0\n"
             "248785745 0\n"
             "421631475 0\n"
             "498966877 0\n"
             "43768791 1\n"
             "112237273 0\n"
             "21499042 142460201\n"
             "58176487 384985131\n"
             "88563042 144788076\n"
             "120198276 497115965\n"
             "134867387 563350571\n"
             "211946499 458996604\n"
             "233934566 297258009\n"
             "335674184 555985828\n"
             "414601661 520203502\n"
             "101135608 501051309\n"
             "90972258 300372385\n"
             "255474956 630621190\n"
             "436210625 517850028\n"
             "145652401 192476406\n"
             "377607297 520655694\n"
             "244404406 304034433\n"
             "112237273 359737255\n"
             "392593015 463983307\n"
             "150586788 504362212\n"
             "54772353 83124235\n",
             "5\n1 7 8 9 11\n"));
 return 0;
}