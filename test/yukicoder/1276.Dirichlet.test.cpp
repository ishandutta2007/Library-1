#define PROBLEM "https://yukicoder.me/problems/no/1276"
#include <iostream>
#include "src/NumberTheory/DirichletSeries.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 int N;
 cin >> N;
 auto zeta= get_1<Mint>(N), d= zeta.square();
 Mint x= d.sum();                         // zeta(s)^2
 Mint y= (zeta * d).sum();                // zeta(s)^3
 Mint z= (zeta * get_Id<Mint>(N)).sum();  // zeta(s) * zeta(s-1)
 Mint A= y - x - x + N;                   // #{(a,b,c): distinct ∧ a|b|c}
 Mint B= z + z - x * 4 + N + N;           // #{(a,b,c): distinct ∧ b|a ∧ b|c}
 Mint C= (x - N) * (N - 2);               // #{(a,b,c): distinct ∧ b|c}
 cout << C - A - B << '\n';
 return 0;
}
