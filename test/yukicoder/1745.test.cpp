// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1745
// competitive-verifier: TLE 0.5
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
 for (auto [l, r]: bg) cout << (dm(l) == dm(r) ? "Yes" : "No") << '\n';
 return 0;
}