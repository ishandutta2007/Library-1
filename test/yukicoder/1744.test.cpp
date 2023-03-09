#define PROBLEM "https://yukicoder.me/problems/no/1744"
#include <iostream>
#include <vector>
#include "src/Graph/DulmageMendelsohn.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, L;
 cin >> N >> M >> L;
 DulmageMendelsohn dm(N, M);
 int S[L], T[L];
 for (int i= 0; i < L; ++i) {
  cin >> S[i] >> T[i];
  dm.add_edge(--S[i], --T[i]);
 }
 dm.build();
 int k= dm.component_num();
 for (int i= 0; i < L; ++i) {
  int l= dm.left_belong(S[i]), r= dm.right_belong(T[i]);
  cout << (l != r || l == 0 || r == k - 1 || dm.left_block(l).size() >= 2 ? "Yes" : "No") << '\n';
 }
 return 0;
}