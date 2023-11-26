#define PROBLEM "https://yukicoder.me/problems/no/644"
#include <iostream>
#include "src/Math/ModInt.hpp"
#include "src/NumberTheory/Sieve.hpp"
#include "src/NumberTheory/famous_arithmetic_functions.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<int(1e9 + 7)>;
 using namespace famous_arithmetic_functions;
 int N, M;
 cin >> N >> M;
 auto phi= Sieve<>::multiplicative_table<Mint>(N, mul::Totient<Mint>::f);
 Mint ans= 0;
 for (int i= 2; i <= N / M; i++) ans+= phi[i];
 for (int i= 1; i <= N - 2; i++) ans*= i;
 ans*= 2;
 cout << ans << endl;
 return 0;
}