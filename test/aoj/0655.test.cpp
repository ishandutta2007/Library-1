// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/challenges/sources/JOI/Prelim/0655
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
#include <iostream>
#include <vector>
#include <algorithm>
#include "src/Misc/compress.hpp"
#include "src/DataStructure/RangeSet.hpp"

using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 int A[N];
 for (int i= 0; i < N; ++i) cin >> A[i];
 vector<int> vec(A, A + N);
 auto id= compress(vec);
 int n= vec.size();
 vector<int> group[n];
 for (int i= 0; i < N; ++i) group[id(A[i])].push_back(i);
 RangeSet<int> rs;
 int ans= 0;
 for (int i= n; i--;) {
  if (vec[i] == 0) break;
  for (int j: group[i]) rs.insert(j);
  ans= max(ans, (int)rs.size());
 }
 cout << ans << '\n';
 return 0;
}