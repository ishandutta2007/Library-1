#define PROBLEM "https://onlinejudge.u-aizu.ac.jp/challenges/sources/PCK/Final/0350"
#include <iostream>
#include <string>
#include "src/Math/Rational.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using Q= Rational<long long>;
 string str;
 cin >> str;
 if (str.back() != ')') str+= "(0)";
 int pw[12]= {1};
 for (int i= 0; i < 11; ++i) pw[i + 1]= pw[i] * 10;
 int p= str.find('.'), q= str.find('(');
 string a= str.substr(0, p);
 string b= str.substr(p + 1, q - p - 1);
 string c= str.substr(q + 1, str.size() - q - 2);
 int n= b.length();
 Q ans= stoi(a);
 if (n) ans+= Q(stoi(b), pw[n]);
 ans+= Q(stoi(c), pw[n] * (pw[c.length()] - 1));
 cout << ans.to_string(true) << '\n';
 return 0;
}