// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/649
// competitive-verifier: TLE 0.5
#include <iostream>
#include <vector>
#include "src/Misc/compress.hpp"
#include "src/DataStructure/BinaryIndexedTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int Q, K;
 cin >> Q >> K;
 K--;
 vector<long long> query, x;
 while (Q--) {
  long long v;
  cin >> v;
  if (v == 1) cin >> v, x.push_back(v);
  else v= -1;
  query.push_back(v);
 }
 auto id= compress(x);
 BinaryIndexedTree<long long> bit(x.size());
 for (auto q: query)
  if (q < 0) {
   if (int i= bit.find(K); i >= 0) cout << x[i] << '\n', bit.add(i, -1);
   else cout << -1 << endl;
  } else bit.add(id(q), 1);
 return 0;
}
