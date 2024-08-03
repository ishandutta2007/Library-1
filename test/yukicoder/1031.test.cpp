// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1031
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Misc/CartesianTree.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 vector<int> p(N);
 for (int i= 0; i < N; ++i) cin >> p[i];
 long long ans= 0;
 for (int _= 2; _--;) {
  reverse(p.begin(), p.end());
  CartesianTree ct(p, false);
  vector<array<int, 2>> st;
  for (int i= 0; i < N; ++i) {
   while (!st.empty() && st.back()[1] > p[i]) st.pop_back();
   int l= ct.range(i)[0];
   int n= lower_bound(st.begin(), st.end(), array{l, -1}) - st.begin();
   ans+= st.size() - n;
   st.push_back(array{i, p[i]});
  }
 }
 cout << ans << '\n';
 return 0;
}