// competitive-verifier: PROBLEM https://onlinejudge.u-aizu.ac.jp/courses/lesson/1/ALDS1/9/ALDS1_9_C
// competitive-verifier: TLE 1.5
#include <iostream>
#include <string>
#include "src/DataStructure/SkewHeap.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 SkewHeap<int> S;
 string op;
 while (cin >> op && op != "end") {
  if (op[0] == 'i') {
   int k;
   cin >> k;
   S.push(k);
  } else cout << S.pop() << '\n';
 }
 return 0;
}