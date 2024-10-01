// competitive-verifier: PROBLEM https://atcoder.jp/contests/wtf19/tasks/wtf19_c2
// competitive-verifier: TLE 0.5
// competitive-verifier: MLE 64
// https://atcoder.jp/contests/wtf19-open/tasks/wtf19_c2
// nimber log
#include <iostream>
#include <vector>
#include "src/Math/Nimber.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
int main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Mint= ModInt<0xffffffffffffffff>;
 Nimber::init();
 static constexpr long long OFS= 1e17;

 Nimber a= -1, b= a.pow(11);
 Mint s= a.log(a + 1), t= b.log(b + 1);

 int N;
 cin >> N;
 vector<long long> x(N), y(N);
 for (int i= 0; i < N; i++) cin >> x[i] >> y[i], x[i]+= OFS, y[i]+= OFS;
 auto f= [&](Nimber z) -> Nimber {
  Nimber sum= 0;
  for (int i= N; i--;) sum+= z.pow(x[i]) * (z + 1).pow(y[i]);
  return sum;
 };
 Mint u= a.log(f(a)), v= b.log(f(b));
 Mint Y= (u - v) / (s - t);
 Mint X= u - s * Y;
 cout << (long long)X.val() - OFS << " " << (long long)Y.val() - OFS << '\n';
 return 0;
}