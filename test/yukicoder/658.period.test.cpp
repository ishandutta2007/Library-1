#define PROBLEM "https://yukicoder.me/problems/no/658"
#include <iostream>
#include <array>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Dat= array<int, 4>;
 auto f= [](const Dat &x) -> Dat { return {x[1], x[2], x[3], (x[0] + x[1] + x[2] + x[3]) % 17}; };
 Dat init{0, 0, 0, 1};
 Period<Dat> p(f, {init});
 int Q;
 cin >> Q;
 while (Q--) {
  long long n;
  cin >> n;
  cout << p.jump(init, n - 1)[0] << '\n';
 }
 return 0;
}