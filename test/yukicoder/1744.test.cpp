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
 BipartiteGraph bg(N, M, L);
 for (int i= 0; i < L; ++i) cin >> bg[i], --bg[i], bg[i].second+= N;
 DulmageMendelsohn dm(bg);
 int k= dm.size();
 for (auto [l, r]: bg) {
  l= dm(l), r= dm(r);
  cout << (l != r || l == 0 || r == k - 1 || dm.block(l).size() > 2 ? "Yes" : "No") << '\n';
 }
 return 0;
}