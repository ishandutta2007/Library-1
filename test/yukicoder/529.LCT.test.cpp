#define PROBLEM "https://yukicoder.me/problems/no/529"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "src/Misc/compress.hpp"
#include "src/Graph/IncrementalBridgeConnectivity.hpp"
#include "src/DataStructure/LinkCutTree.hpp"
using namespace std;
struct RmaxQ {
 using T= pair<long long, int>;
 static T op(const T &vl, const T &vr) { return vl.first > vr.first ? vl : vr; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N, M, Q;
 cin >> N >> M >> Q;
 IncrementalBridgeConnectivity ibc(N);
 int A[M], B[M];
 for (int i= 0; i < M; i++) {
  cin >> A[i] >> B[i];
  ibc.add_edge(--A[i], --B[i]);
 }
 vector<int> vec;
 for (int i= 0; i < N; ++i) vec.push_back(ibc.represent(i));
 auto id= compress(vec);
 int n= vec.size();
 auto idx= [&](int i) { return id(ibc.represent(i)); };

 LinkCutTree<RmaxQ> lct(n);
 for (int i= 0; i < n; i++) lct.set(i, {-1, i});
 for (int i= 0; i < M; i++) {
  int u= idx(A[i]), v= idx(B[i]);
  if (u == v) continue;
  lct.link(u, v);
 }

 priority_queue<long long> pq[n];
 for (int i= 0; i < n; i++) pq[i].push(-1);
 while (Q--) {
  int op, x, y;
  cin >> op >> x >> y;
  if (op == 1) {
   int u= idx(--x);
   pq[u].push(y);
   lct.set(u, make_pair(pq[u].top(), u));
  } else {
   int u= idx(--x), v= idx(--y);
   auto [ans, i]= lct.fold(u, v);
   cout << ans << endl;
   if (ans != -1) {
    pq[i].pop();
    lct.set(i, make_pair(pq[i].top(), i));
   }
  }
 }
 return 0;
}