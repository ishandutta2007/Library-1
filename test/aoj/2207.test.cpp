// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2207
// competitive-verifier: TLE 0.5
#include <iostream>
#include <map>
#include <string>
#include "src/DataStructure/UnionFind_Potential.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 while (cin >> N && N != 0) {
  UnionFind_Potential<long long> uf(2 * N);
  int cnt= 0;
  map<string, int> mp;
  bool isok= true;
  for (int i= 0; i < N; i++) {
   string one, A, eq, pow, B;
   cin >> one >> A >> eq >> pow >> B;
   if (!isok) continue;
   int a, b;
   if (mp.count(A) == 0) a= mp[A]= cnt++;
   else a= mp[A];
   if (mp.count(B) == 0) b= mp[B]= cnt++;
   else b= mp[B];
   pow= pow.substr(3);
   int x= stoi(pow);
   if (!uf.same(a, b) || uf.diff(a, b) == x) uf.unite(a, b, x);
   else isok= false;
  }
  cout << (isok ? "Yes" : "No") << endl;
 }
 return 0;
}