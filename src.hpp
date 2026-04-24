#ifndef SJTU_COMPLEX_HPP
#define SJTU_COMPLEX_HPP

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdexcept>

namespace sjtu {

class divided_by_zero final : public std::exception {
 public:
  divided_by_zero() = default;
  ~divided_by_zero() = default;
  const char *what() const noexcept override { return "complex divided by zero!"; }
};

inline int sign(double x) {
    const double eps = 1e-6;
    return (x > eps) - (x < -eps);
}

class complex {
 private:
  double a, b; // a + bi

  static double norm2(double x, double y) { return x * x + y * y; }

 public:
  complex() : a(0), b(0) {}
  complex(double a_) : a(a_), b(0) {}
  complex(double a_, double b_) : a(a_), b(b_) {}

  double &real() { return a; }
  double &imag() { return b; }

  complex operator-() const { return complex(-a, -b); }
  complex operator~() const { return complex(a, -b); }

  complex operator+(const complex &rhs) const { return complex(a + rhs.a, b + rhs.b); }
  complex operator-(const complex &rhs) const { return complex(a - rhs.a, b - rhs.b); }
  complex operator*(const complex &rhs) const {
    return complex(a * rhs.a - b * rhs.b, a * rhs.b + b * rhs.a);
  }
  complex operator/(const complex &rhs) const {
    if (sign(rhs.a) == 0 && sign(rhs.b) == 0) throw divided_by_zero();
    double den = norm2(rhs.a, rhs.b);
    return complex((a * rhs.a + b * rhs.b) / den, (b * rhs.a - a * rhs.b) / den);
  }

  complex &operator+=(const complex &rhs) {
    a += rhs.a; b += rhs.b; return *this;
  }
  complex &operator-=(const complex &rhs) {
    a -= rhs.a; b -= rhs.b; return *this;
  }
  complex &operator*=(const complex &rhs) {
    double na = a * rhs.a - b * rhs.b;
    double nb = a * rhs.b + b * rhs.a;
    a = na; b = nb; return *this;
  }
  complex &operator/=(const complex &rhs) {
    if (sign(rhs.a) == 0 && sign(rhs.b) == 0) throw divided_by_zero();
    double den = norm2(rhs.a, rhs.b);
    double na = (a * rhs.a + b * rhs.b) / den;
    double nb = (b * rhs.a - a * rhs.b) / den;
    a = na; b = nb; return *this;
  }

  bool operator==(const complex &rhs) const {
    return std::fabs(a - rhs.a) < 1e-6 && std::fabs(b - rhs.b) < 1e-6;
  }

  explicit operator bool() const { return !(sign(a) == 0 && sign(b) == 0); }

  friend std::ostream &operator<<(std::ostream &os, const complex &x) {
    const double eps = 1e-6;
    double ra = (std::fabs(x.a) < eps) ? 0.0 : x.a;
    double rb = (std::fabs(x.b) < eps) ? 0.0 : x.b;

    // real part: no sign if non-negative, '-' if negative
    if (ra < -eps) {
      os << std::fixed << std::setprecision(6) << ra;
    } else {
      os << std::fixed << std::setprecision(6) << ra;
    }

    // imag sign: '-' if negative, '+' otherwise (including zero)
    os << ((rb < -eps) ? '-' : '+');
    os << std::fixed << std::setprecision(6) << std::fabs(rb) << 'i';
    return os;
  }
};

}

#endif
