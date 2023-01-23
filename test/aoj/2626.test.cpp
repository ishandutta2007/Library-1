#define PROBLEM "http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2626"
#define ERROR "0.00000001"
#include <iostream>
#include <iomanip>
#include <string>
#include "src/Geometry/!geometry_temp.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 using namespace geometry;
 int H, W;
 cin >> H >> W;
 Real mA[2], mB[2], mX;
 cin >> mA[0] >> mA[1] >> mB[0] >> mB[1] >> mX;
 string M[H];
 int cA= 0, cB= 0, cX= 0;
 Real xA= 0, xB= 0, xX= 0;
 Real yA= 0, yB= 0, yX= 0;
 for (int i= 0; i < H; i++) {
  cin >> M[i];
  for (int j= 0; j < W; j++) {
   if (M[i][j] == 'A') cA++, xA+= j + 0.5, yA+= i + 0.5;
   if (M[i][j] == 'B') cB++, xB+= j + 0.5, yB+= i + 0.5;
   if (M[i][j] == 'X') cX++, xX+= j + 0.5, yX+= i + 0.5;
  }
 }
 xA/= cA, xB/= cB, xX/= cX;
 yA/= cA, yB/= cB, yX/= cX;
 mA[0]*= cA, mA[1]*= cA, mB[0]*= cB, mB[1]*= cB, mX*= cX;
 Real ans= 0;
 for (int i= 0; i < H; i++)
  for (int j= 0; j < W; j++)
   if (M[i][j] != '.') {
    Convex ch= {{mA[0], mB[0]}, {mA[1], mB[0]}, {mA[1], mB[1]}, {mA[0], mB[1]}};
    Real a, b, c;
    Line l;
    a= xA - j, b= xB - j, c= (xX - j) * mX;
    if (sgn(a) || sgn(b)) {  // ax+by+c>0
     l= Line(a, b, c);
     ch= ch.cut(l, LEFT);
    } else if (sgn(c) <= 0) ch.clear();
    a= xA - j - 1, b= xB - j - 1, c= (xX - j - 1) * mX;
    if (sgn(a) || sgn(b)) {  // ax+by+c<0
     l= Line(a, b, c);
     ch= ch.cut(l, RIGHT);
    } else if (sgn(c) >= 0) ch.clear();
    a= yA - i, b= yB - i, c= (yX - i) * mX;
    if (sgn(a) || sgn(b)) {  // ax+by+c>0
     l= Line(a, b, c);
     ch= ch.cut(l, LEFT);
    } else if (sgn(c) <= 0) ch.clear();
    a= yA - i - 1, b= yB - i - 1, c= (yX - i - 1) * mX;
    if (sgn(a) || sgn(b)) {  // ax+by+c<0
     l= Line(a, b, c);
     ch= ch.cut(l, RIGHT);
    } else if (sgn(c) >= 0) ch.clear();
    ans+= ch.area();
   }
 ans/= (mA[1] - mA[0]) * (mB[1] - mB[0]);
 cout << fixed << setprecision(12) << ans << endl;
 return 0;
}