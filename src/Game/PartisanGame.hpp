#pragma once
#include <bits/stdc++.h>
class DyadicRational {
 static constexpr char FracLen= std::numeric_limits<std::uint64_t>::digits - 1;
 static constexpr std::uint64_t Denom= 1ULL << FracLen;
 int integ;
 std::uint64_t frac;
 template <class l_t, class r_t>  // Conway's realization
 static DyadicRational reduce(const std::vector<l_t> &L, const std::vector<r_t> &R) {
  if (L.empty() && R.empty()) return DyadicRational();
  DyadicRational sl, sr;
  if (!L.empty()) sl= *std::max_element(L.begin(), L.end());
  if (!R.empty()) sr= *std::min_element(R.begin(), R.end());
  if (L.empty()) return DyadicRational(sr.integ - !sr.frac);
  if (R.empty()) return DyadicRational(sl.integ + 1);
  return reduce(sl, sr);
 }
 static DyadicRational reduce(const DyadicRational &l, const DyadicRational &r) {
  assert(l < r);
  if (r <= 0) return -reduce(-r, -l);
  if (l.integ < 0) return DyadicRational();
  if (DyadicRational(l.integ + 1) < r) return DyadicRational(l.integ + 1);
  DyadicRational ret;
  std::uint64_t rfrac= r.frac == 0 ? Denom : r.frac;
  std::uint64_t D= 1ULL << (FracLen - __builtin_clzll(l.frac ^ rfrac));
  std::uint64_t f= (l.frac & (Denom - D)) | D;
  if (f < rfrac) return ret.integ= l.integ, ret.frac= f, ret;
  D= 1ULL << (FracLen - __builtin_clzll((l.frac & (D - 1)) ^ (D - 1)));
  f= (l.frac & (Denom - D)) | D;
  return ret.integ= l.integ, ret.frac= f, ret;
 }
public:
 DyadicRational(): integ(0), frac(0) {}
 DyadicRational(double x) {
  double i, f= std::modf(x, &i);
  if (f < 0) f+= 1, i-= 1;
  integ= i, frac= f * Denom;
 }
 template <class l_t= double, class r_t= double> DyadicRational(const std::vector<l_t> &L, const std::vector<r_t> &R): DyadicRational(reduce<l_t, r_t>(L, R)) {}
 template <class l_t= double, class r_t= double> DyadicRational(const std::initializer_list<l_t> &L, const std::initializer_list<r_t> &R): DyadicRational(reduce<l_t, r_t>(L, R)) {}
 template <class l_t= double, class r_t= double> DyadicRational(const std::vector<l_t> &L, const std::initializer_list<r_t> &R): DyadicRational(reduce<l_t, r_t>(L, R)) {}
 template <class l_t= double, class r_t= double> DyadicRational(const std::initializer_list<l_t> &L, const std::vector<r_t> &R): DyadicRational(reduce<l_t, r_t>(L, R)) {}
 double to_double() const { return integ + double(frac) / Denom; }
 bool operator==(const DyadicRational &r) const { return integ == r.integ && frac == r.frac; }
 bool operator!=(const DyadicRational &r) const { return !(*this == r); }
 bool operator<(const DyadicRational &r) const { return integ == r.integ ? frac < r.frac : integ < r.integ; }
 bool operator>(const DyadicRational &r) const { return r < *this; }
 bool operator<=(const DyadicRational &r) const { return !(r < *this); }
 bool operator>=(const DyadicRational &r) const { return !(*this < r); }
 DyadicRational &operator+=(const DyadicRational &r) {
  if ((frac+= r.frac) >= Denom) integ++, frac-= Denom;
  return integ+= r.integ, *this;
 }
 DyadicRational &operator-=(const DyadicRational &r) {
  frac= frac >= r.frac ? frac - r.frac : (integ--, frac + (Denom - r.frac));
  return integ-= r.integ, *this;
 }
 DyadicRational operator-() const { return DyadicRational()-= *this; }
 DyadicRational operator+(const DyadicRational &r) const { return DyadicRational(*this)+= r; }
 DyadicRational operator-(const DyadicRational &r) const { return DyadicRational(*this)-= r; }
 friend std::istream &operator>>(std::istream &is, DyadicRational &r) {
  double x;
  return is >> x, r= DyadicRational(x), is;
 }
 friend std::ostream &operator<<(std::ostream &os, const DyadicRational &r) { return os << r.to_double(); }
};
template <typename Game, typename F> class PartisanGame {
 std::map<Game, DyadicRational> mp;
 F f;  // : Game -> (std::vector<Game>, std::vector<Game>)
public:
 PartisanGame(const F &_f): f(_f) {}
 DyadicRational eval(Game g) {
  if (mp.count(g)) return mp[g];
  auto [gls, grs]= f(g);
  std::vector<DyadicRational> L, R;
  for (auto &gl: gls) L.emplace_back(eval(gl));
  for (auto &gr: grs) R.emplace_back(eval(gr));
  return mp[g]= DyadicRational(L, R);
 }
};