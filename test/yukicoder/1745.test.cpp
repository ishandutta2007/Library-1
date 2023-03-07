#define PROBLEM "https://yukicoder.me/problems/no/1745"
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
 for (int i= 0; i < L; ++i) {
  cout << (dm.left_belong(S[i]) == dm.right_belong(T[i]) ? "Yes" : "No") << '\n';
 }
 return 0;
}