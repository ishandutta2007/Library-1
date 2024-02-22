#define PROBLEM "https://yukicoder.me/problems/no/1595"
#include <iostream>
#include <array>
#include "src/Misc/Period.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Dat= array<int, 3>;
 auto f= [](const Dat &x) -> Dat { return {x[1], x[2], (x[0] + x[1] + x[2]) % 10}; };
 int p, q, r;
 cin >> p >> q >> r;
 Dat init= {p, q, r};
 Period<Dat> period(f, {init});
 long long K;
 cin >> K;
 cout << period.jump(init, K - 1)[0] << '\n';
 return 0;
}