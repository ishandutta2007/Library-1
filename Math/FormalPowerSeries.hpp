/**
 * @title 形式的冪級数(任意素数MOD)
 * @category 数学
 * @brief 998244353とかでなくても使える
 * @brief まずinitを呼んでmodを設定
 * @brief 配列のサイズに注意(REの原因になりがち)
 */
// verify用 https://loj.ac/problem/150

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

using u32 = unsigned;
using u64 = unsigned long long;
namespace ntt {
template <u64 mod, u64 prim_root> class Mod64 {
  private:
    using u128 = __uint128_t;
    static constexpr u64 mul_inv(u64 n, int e = 6, u64 x = 1) {
        return e == 0 ? x : mul_inv(n, e - 1, x * (2 - x * n));
    }

  public:
    static constexpr u64 inv = mul_inv(mod, 6, 1);
    static constexpr u64 r2 = -u128(mod) % mod;
    static constexpr int level = __builtin_ctzll(mod - 1);
    static_assert(inv * mod == 1, "invalid 1/M modulo 2^64.");
    Mod64() {}
    Mod64(u64 n) : x(init(n)){};
    static u64 modulo() { return mod; }
    static u64 init(u64 w) { return reduce(u128(w) * r2); }
    static u64 reduce(const u128 w) {
        return u64(w >> 64) + mod - ((u128(u64(w) * inv) * mod) >> 64);
    }
    static Mod64 omega() { return Mod64(prim_root).pow((mod - 1) >> level); }
    Mod64 &operator+=(Mod64 rhs) {
        this->x += rhs.x;
        return *this;
    }
    Mod64 &operator-=(Mod64 rhs) {
        this->x += 2 * mod - rhs.x;
        return *this;
    }
    Mod64 &operator*=(Mod64 rhs) {
        this->x = reduce(u128(this->x) * rhs.x);
        return *this;
    }
    Mod64 operator+(Mod64 rhs) const { return Mod64(*this) += rhs; }
    Mod64 operator-(Mod64 rhs) const { return Mod64(*this) -= rhs; }
    Mod64 operator*(Mod64 rhs) const { return Mod64(*this) *= rhs; }
    u64 get() const { return reduce(this->x) % mod; }
    void set(u64 n) const { this->x = n; }
    Mod64 pow(u64 exp) const {
        Mod64 ret = Mod64(1);
        for(Mod64 base = *this; exp; exp >>= 1, base *= base)
            if(exp & 1)
                ret *= base;
        return ret;
    }
    Mod64 inverse() const { return pow(mod - 2); }
    friend ostream &operator<<(ostream &os, const Mod64 &m) {
        return os << m.get();
    }
    u64 x;
};

template <typename mod_t>
void convolute(mod_t *A, int s1, mod_t *B, int s2, bool cyclic = false) {
    int s = (cyclic ? max(s1, s2) : s1 + s2 - 1);
    int size = 1;
    while(size < s)
        size <<= 1;
    mod_t roots[mod_t::level] = {mod_t::omega()};
    for(int i = 1; i < mod_t::level; i++)
        roots[i] = roots[i - 1] * roots[i - 1];
    fill(A + s1, A + size, 0);
    ntt_dit4(A, size, 1, roots);
    if(A == B && s1 == s2) {
        for(int i = 0; i < size; i++)
            A[i] *= A[i];
    } else {
        fill(B + s2, B + size, 0);
        ntt_dit4(B, size, 1, roots);
        for(int i = 0; i < size; i++)
            A[i] *= B[i];
    }
    ntt_dit4(A, size, -1, roots);
    mod_t inv = mod_t(size).inverse();
    for(int i = 0; i < (cyclic ? size : s); i++)
        A[i] *= inv;
}

template <typename mod_t> void rev_permute(mod_t *A, int n) {
    int r = 0, nh = n >> 1;
    for(int i = 1; i < n; i++) {
        for(int h = nh; !((r ^= h) & h); h >>= 1)
            ;
        if(r > i)
            swap(A[i], A[r]);
    }
}

template <typename mod_t>
void ntt_dit4(mod_t *A, int n, int sign, mod_t *roots) {
    rev_permute(A, n);
    int logn = __builtin_ctz(n);
    if(logn & 1)
        for(int i = 0; i < n; i += 2) {
            mod_t a = A[i], b = A[i + 1];
            A[i] = a + b;
            A[i + 1] = a - b;
        }
    mod_t imag = roots[mod_t::level - 2];
    if(sign < 0)
        imag = imag.inverse();
    mod_t one = mod_t(1);
    for(int e = 2 + (logn & 1); e < logn + 1; e += 2) {
        const int m = 1 << e;
        const int m4 = m >> 2;
        mod_t dw = roots[mod_t::level - e];
        if(sign < 0)
            dw = dw.inverse();
        const int block_size = max(m, (1 << 15) / int(sizeof(A[0])));
        for(int k = 0; k < n; k += block_size) {
            mod_t w = one, w2 = one, w3 = one;
            for(int j = 0; j < m4; j++) {
                for(int i = k + j; i < k + block_size; i += m) {
                    mod_t a0 = A[i + m4 * 0] * one, a2 = A[i + m4 * 1] * w2;
                    mod_t a1 = A[i + m4 * 2] * w, a3 = A[i + m4 * 3] * w3;
                    mod_t t02 = a0 + a2, t13 = a1 + a3;
                    A[i + m4 * 0] = t02 + t13;
                    A[i + m4 * 2] = t02 - t13;
                    t02 = a0 - a2, t13 = (a1 - a3) * imag;
                    A[i + m4 * 1] = t02 + t13;
                    A[i + m4 * 3] = t02 - t13;
                }
                w *= dw;
                w2 = w * w;
                w3 = w2 * w;
            }
        }
    }
}

const int size = 1 << 22;
using m64_1 = ntt::Mod64<34703335751681, 3>;
using m64_2 = ntt::Mod64<35012573396993, 3>;
m64_1 f1[size], g1[size];
m64_2 f2[size], g2[size];

} // namespace ntt

using R = u64;
class FormalPowerSeries {
    using FPS = FormalPowerSeries;

  public:
    FormalPowerSeries() {}
    FormalPowerSeries(int n) : coefs(n) {}
    FormalPowerSeries(int n, int c) : coefs(n, c % mod) {}
    FormalPowerSeries(const vector<R> &v) : coefs(v) {}
    FormalPowerSeries(const FPS &f, int beg, int end = -1) {
        if(end < 0)
            end = beg, beg = 0;
        resize(end - beg);
        for(int i = beg; i < end; i++)
            if(i < f.size())
                coefs[i - beg] = f[i];
    }
    int size() const { return coefs.size(); }
    void resize(int s, R v = 0) { coefs.resize(s, v); }
    void push_back(R c) { coefs.push_back(c); }
    void shrink() {
        while(!coefs.empty() && !coefs.back())
            coefs.pop_back();
    }
    void pop_back() { coefs.pop_back(); }
    const R *data() const { return coefs.data(); }
    R *data() { return coefs.data(); }
    const R &operator[](int i) const { return coefs[i]; }
    R &operator[](int i) { return coefs[i]; }

  public:
    static void mod_add(R &a, R b) {
        if((a += b) >= mod)
            a -= mod;
    }
    static void mod_sub(R &a, R b) {
        if(int(a -= b) < 0)
            a += mod;
    }
    static R mod_mul(R a, R b) { return a * b % fast_mod; }
    static R mod_pow(R v, u64 exp) {
        R ret = 1;
        for(R base = v; exp; exp >>= 1, base = mod_mul(base, base))
            if(exp & 1)
                ret = mod_mul(ret, base);
        return ret;
    }
    static R mod_inverse(R v) { return mod_pow(v, mod - 2); }
    static R mod_sqrt(R x) {
        if(x == 0 || mod == 2)
            return x;
        if(mod_pow(x, (mod - 1) >> 1) != 1)
            return 0; // no solution
        R b = 2;
        R w = (b * b + mod - x) % fast_mod;
        while(mod_pow(w, (mod - 1) >> 1) == 1) {
            b++;
            w = (b * b + mod - x) % fast_mod;
        }
        auto mul = [&](pair<R, R> u, pair<R, R> v) {
            R a = (u.first * v.first + u.second * v.second % fast_mod * w)
                  % fast_mod;
            R b = (u.first * v.second + u.second * v.first) % fast_mod;
            return make_pair(a, b);
        };
        u32 exp = (mod + 1) >> 1;
        pair<R, R> ret = make_pair(1, 0);
        for(auto base = make_pair(b, 1); exp;
            exp >>= 1, base = mul(base, base)) {
            if(exp & 1)
                ret = mul(ret, base);
        }
        return ret.first;
    }

  public:
    struct fast_div {
        using u128 = __uint128_t;
        fast_div(){};
        fast_div(u64 n) : m(n) {
            s = (n == 1) ? 0 : 127 - __builtin_clzll(n - 1);
            x = ((u128(1) << s) + n - 1) / n;
        }
        friend u64 operator/(u64 n, fast_div d) { return u128(n) * d.x >> d.s; }
        friend u64 operator%(u64 n, fast_div d) { return n - n / d * d.m; }
        u64 m, s, x;
    };

    static FPS mul_crt(int beg, int end) {
        using namespace ntt;
        auto inv = m64_2(m64_1::modulo()).inverse();
        auto mod1 = m64_1::modulo() % fast_mod;
        FPS ret(end - beg);
        for(int i = 0; i < (int)ret.size(); i++) {
            u64 r1 = f1[i + beg].get(), r2 = f2[i + beg].get();
            ret[i] = (r1
                      + (m64_2(r2 + m64_2::modulo() - r1) * inv).get()
                            % fast_mod * mod1)
                     % fast_mod;
        }
        return ret;
    }

    static void mul2(const FPS &f, const FPS &g, bool cyclic = false) {
        using namespace ntt;
        if(&f == &g) {
            for(int i = 0; i < (int)f.size(); i++)
                f1[i] = f[i];
            convolute(f1, f.size(), f1, f.size(), cyclic);
            for(int i = 0; i < (int)f.size(); i++)
                f2[i] = f[i];
            convolute(f2, f.size(), f2, f.size(), cyclic);
        } else {
            for(int i = 0; i < (int)f.size(); i++)
                f1[i] = f[i];
            for(int i = 0; i < (int)g.size(); i++)
                g1[i] = g[i];
            convolute(f1, f.size(), g1, g.size(), cyclic);
            for(int i = 0; i < (int)f.size(); i++)
                f2[i] = f[i];
            for(int i = 0; i < (int)g.size(); i++)
                g2[i] = g[i];
            convolute(f2, f.size(), g2, g.size(), cyclic);
        }
    }

  private:
    FPS mul_n(const FPS &g) const {
        if(size() == 0 || g.size() == 0)
            return FPS();
        FPS ret(size() + g.size() - 1, 0);
        for(int i = 0; i < size(); i++)
            for(int j = 0; j < g.size(); j++)
                mod_add(ret[i + j], mod_mul((*this)[i], g[j]));
        return ret;
    }
    FPS mul(const FPS &g) const {
        if(size() + g.size() < 80)
            return mul_n(g);
        const auto &f = *this;
        mul2(f, g, false);
        return mul_crt(0, f.size() + g.size() - 1);
    }
    pair<FPS, FPS> div_n(const FPS &g) const {
        FPS f(*this, size());
        if(f.size() < g.size())
            return make_pair(FPS(), f);
        FPS u(f.size() - g.size() + 1, 0);
        R inv = mod_inverse(g[g.size() - 1]);
        for(int i = u.size() - 1; i >= 0; --i) {
            u[i] = mod_mul(f[f.size() - 1], inv);
            for(int j = 0; j < g.size(); ++j)
                mod_sub(f[j + f.size() - g.size()], mod_mul(g[j], u[i]));
            f.pop_back();
        }
        return {u, f};
    }
    FPS middle_product(const FPS &g) const {
        const FPS &f = *this;
        if(f.size() == 0 || g.size() == 0)
            return FPS();
        mul2(f, g, true);
        return mul_crt(f.size(), g.size());
    }

    FPS inverse(int deg = -1) const {
        if(deg < 0)
            deg = size();
        FPS ret(1, mod_inverse((*this)[0]));
        for(int e = 1, ne; e < deg; e = ne) {
            ne = min(2 * e, deg);
            FPS h = FPS(ret, ne - e) * -ret.middle_product(FPS(*this, ne));
            for(int i = e; i < ne; i++)
                ret.push_back(h[i - e]);
        }
        return ret;
    }
    FPS differentiation() const {
        FPS ret(max(0, size() - 1));
        for(int i = 1; i < size(); i++)
            ret[i - 1] = mod_mul(i, (*this)[i]);
        return ret;
    }

    FPS integral() const {
        FPS ret(size() + 1);
        ret[0] = 0;
        for(int i = 0; i < size(); i++)
            ret[i + 1] = mod_mul(inve[i + 1], (*this)[i]);
        return ret;
    }

    FPS logarithm(int deg = -1) const {
        assert((*this)[0] == 1);
        if(deg < 0)
            deg = size();
        return FPS(differentiation() * inverse(deg), deg - 1).integral();
    }

    FPS exponent(int deg = -1) const {
        assert((*this)[0] == 0);
        if(deg < 0)
            deg = size();
        FPS ret(vector<R>({1, 1 < size() ? (*this)[1] : 0})), retinv(1, 1);
        FPS f = differentiation();
        FPS retdif = ret.differentiation();
        for(int e = 1, ne = 2, nne; ne < deg; e = ne, ne = nne) {
            nne = min(2 * ne, deg);
            FPS h = FPS(retinv, ne - e) * -retinv.middle_product(ret);
            for(int i = e; i < ne; i++)
                retinv.push_back(h[i - e]);
            FPS a = ret * FPS(f, nne) - retdif;
            FPS c = (retinv * FPS(a, nne)).integral();
            h = ret.middle_product(FPS(c, nne));
            for(int i = ne; i < nne; i++) {
                ret.push_back(h[i - ne]);
                retdif.push_back(mod_mul(i, h[i - ne]));
            }
        }
        return ret;
    }

    FPS square_root(int deg = -1) const {
        if(deg < 0)
            deg = size();
        if((*this)[0] == 0) {
            for(int i = 1; i < size(); i++) {
                if((*this)[i] != 0) {
                    if(i & 1)
                        return FPS(); // no solution
                    if(deg - i / 2 <= 0)
                        break;
                    auto ret = (*this >> i).square_root(deg - i / 2);
                    if(!ret.size())
                        return FPS(); // no solution
                    ret = ret << (i / 2);
                    if(ret.size() < deg)
                        ret.resize(deg, 0);
                    return ret;
                }
            }
            return FPS(deg, 0);
        }
        R sqr = mod_sqrt((*this)[0]);
        if(sqr * 2 > mod)
            sqr = mod - sqr;
        if(mod_mul(sqr, sqr) != (*this)[0])
            return FPS(); // no solution
        FPS ret(1, sqr);
        for(int i = 1; i < deg; i <<= 1) {
            ret = FPS(ret + (FPS(*this, i << 1) * ret.inverse(i << 1)), i << 1);
            ret *= inve[2];
        }
        return ret;
    }

    FPS power(u64 k, int deg = -1) const {
        if(deg < 0)
            deg = size();
        for(int i = 0; i < size(); i++) {
            if((*this)[i] != 0) {
                if(i * k > deg)
                    return FPS(deg, 0);
                R rev = mod_inverse((*this)[i]);
                FPS ret = (((*this * rev) >> i).logarithm() * k).exponent()
                          * mod_pow((*this)[i], k);
                return FPS(ret << (i * k), deg);
            }
        }
        return *this;
    }

  public:
    FPS rev() const {
        FPS ret((*this), size());
        reverse(ret.coefs.begin(), ret.coefs.end());
        return ret;
    }
    FPS operator-() {
        FPS ret = *this;
        for(int i = 0; i < (int)ret.size(); i++)
            ret[i] = (ret[i] == 0 ? 0 : mod - ret[i]);
        return ret;
    }
    FPS &operator+=(const FPS &rhs) {
        if(size() < rhs.size())
            resize(rhs.size());
        for(int i = 0; i < (int)rhs.size(); i++)
            mod_add((*this)[i], rhs[i]);
        return *this;
    }
    FPS &operator-=(const FPS &rhs) {
        if(size() < rhs.size())
            resize(rhs.size());
        for(int i = 0; i < (int)rhs.size(); i++)
            mod_sub((*this)[i], rhs[i]);
        return *this;
    }
    FPS &operator*=(const FPS &rhs) { return *this = *this * rhs; }
    FPS &operator/=(const FPS &rhs) {
        if(size() < rhs.size())
            return *this = FPS();
        if(rhs.size() < 250)
            return *this = div_n(rhs).first;
        int sq = size() - rhs.size() + 1;
        return *this = FPS(FPS(rev(), sq) * FPS(rhs.rev().inverse(sq), sq), sq)
                           .rev();
    }
    FPS &operator%=(const FPS &rhs) {
        if(rhs.size() < 250)
            return *this = div_n(rhs).second;
        *this -= (*this / rhs) * rhs;
        shrink();
        return *this;
    }
    FPS &operator+=(const R &v) {
        mod_add((*this)[0], v);
        return *this;
    }
    FPS &operator-=(const R &v) {
        mod_sub((*this)[0], v);
        return *this;
    }
    FPS &operator*=(const R &v) {
        for(int k = 0; k < size(); k++)
            (*this)[k] = mod_mul((*this)[k], v);
        return *this;
    }
    FPS operator>>(int sz) const {
        if(size() <= sz)
            return {};
        return FPS(*this, sz, size());
    }
    FPS operator<<(int sz) const {
        FPS ret(size() + sz, 0);
        for(int i = 0; i < size(); i++)
            ret[sz + i] = (*this)[i];
        return ret;
    }
    FPS operator+(const FPS &rhs) const { return FPS(*this) += rhs; } // O(N)
    FPS operator-(const FPS &rhs) const { return FPS(*this) -= rhs; } // O(N)
    FPS operator*(const FPS &rhs) const { return this->mul(rhs); } // O(NlogN)
    FPS operator/(const FPS &rhs) const {
        return FPS(*this) /= rhs;
    } // O(NlogN)
    FPS operator%(const FPS &rhs) const {
        return FPS(*this) %= rhs;
    }                                                            // O(NlogN)
    FPS operator+(const R &v) const { return FPS(*this) += v; }  // O(1)
    FPS operator-(const R &v) const { return FPS(*this) -= v; }  // O(1)
    FPS operator*(const R &v) const { return FPS(*this) *= v; }  // O(N)
    FPS diff() const { return differentiation(); }               // O(N)
    FPS inte() const { return integral(); }                      // O(N)
    FPS inv(int deg = -1) const { return inverse(deg); }         // O(NlogN)
    FPS log(int deg = -1) const { return logarithm(deg); }       // O(NlogN)
    FPS exp(int deg = -1) const { return exponent(deg); }        // O(NlogN)
    FPS sqrt(int deg = -1) const { return square_root(deg); }    // O(NlogN)
    FPS pow(u64 k, int deg = -1) const { return power(k, deg); } // O(NlogN)

  public:
    vector<R> coefs;
    static R mod;
    static fast_div fast_mod;
    static R inve[ntt::size];
    static void init(R m) {
        mod = m;
        fast_mod = fast_div(m);
        inve[1] = 1;
        for(int i = 2; i < ntt::size; ++i)
            inve[i] = inve[mod % i] * (mod - mod / i) % fast_mod;
    }
};
using FPS = FormalPowerSeries;
R FPS::mod;
FPS::fast_div FPS::fast_mod;
R FPS::inve[];