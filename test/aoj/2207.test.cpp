// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/problems/2207
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64

// ポテンシャルUF
#include <iostream>
#include <map>
#include <string>
#include "src/DataStructure/UnionFind_Potentialized.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 while (cin >> N && N != 0) {
  UnionFind_Potentialized<long long> uf(2 * N);
  int cnt= 0;
  map<string, int> mp;
  bool isok= true;
  for (int i= 0; i < N; i++) {
   string one, A, eq, pow, B;
   cin >> one >> A >> eq >> pow >> B;
   if (!isok) continue;
   int a, b;
   if (auto it= mp.find(A); it != mp.end()) a= it->second;
   else a= mp[A]= cnt++;
   if (auto it= mp.find(B); it != mp.end()) b= it->second;
   else b= mp[B]= cnt++;
   pow= pow.substr(3);
   int x= stoi(pow);
   isok&= uf.unite(a, b, x);
  }
  cout << (isok ? "Yes" : "No") << '\n';
 }
 return 0;
}