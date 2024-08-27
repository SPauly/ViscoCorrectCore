// ViscoCorrectCore - Correction factors for centrifugal pumps
// Copyright (C) 2024  Simon Pauly
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Contact via <https://github.com/SPauly/ViscoCorrectCore>
#ifndef SPAULY_VCCORE_IMPL_MATH_H_
#define SPAULY_VCCORE_IMPL_MATH_H_

#include <array>
#include <cmath>

namespace spauly {
namespace vccore {
namespace impl {

using DoubleT = double;

template <typename T, size_t S>
class ParameterisedBaseFunc {
 public:
  /// @brief Default constructor initializes all coefficients to 0.
  ParameterisedBaseFunc() = default;

  /// @brief Initializes the coefficients in the correct order.
  /// @param coefficients The coefficients should be ordered like so [0]*x^n-1
  /// [1]
  /// * x^n-2 ... [n] * x^0
  ParameterisedBaseFunc(const std::array<T, S> &coefficients)
      : coeffs_(coefficients) {}

  virtual ~ParameterisedBaseFunc() = default;

  /// @brief Returns the y value of the given function at position x. Here the
  /// Function dependent code should be implemented.
  /// @param x -Value
  /// @return function value.
  virtual T operator()(T x) const = 0;

 protected:
  std::array<T, S> coeffs_{};
};

/// @brief Linearfunction y = m*x + b
/// @tparam T Type of the function.
template <typename T>
class LinearFunc : public ParameterisedBaseFunc<T, 2> {
 public:
  /// @brief Default constructor initializes m and b to 0.
  LinearFunc() : ParameterisedBaseFunc() {}

  /// @brief Constructs a linear function using the coefficients in the given
  /// order.
  /// @param coefficients The coefficients should be ordered like so [0] = m
  /// [1] = b
  LinearFunc(const std::array<T, 2> &coefficients)
      : ParameterisedBaseFunc(coefficients) {}

  /// @brief Constructs a linear function based of the pitch and a point.
  /// @param m Pitch of the function.
  /// @param x x value of the point.
  /// @param y y value of the point.
  LinearFunc(T m, T x, T y)
      : ParameterisedBaseFunc({m, static_cast<T>(y - (m * x))}) {}

  virtual ~LinearFunc() = default;

  /// @brief Returns the y value of the given function at position x.
  /// @param x Position to evaluate the function at.
  /// @return Value of the function at x.
  virtual T operator()(T x) const override {
    return coeffs_[0] * x + coeffs_[1];
  }

  /// @brief Returns the x value of the given function at position y.
  /// @param y Position to evaluate the function at.
  virtual T SolveForX(const T y) const {
    if (coeffs_[0] == 0) {
      return 0;
    }
    return (y - coeffs_[1]) / coeffs_[0];
  }
};

template <typename T, size_t S>
class PolynomialFunc : public ParameterisedBaseFunc<T, S> {
 public:
  /// @brief Default constructor initializes all coefficients to 0.
  PolynomialFunc() : ParameterisedBaseFunc() {};

  /// @brief Constructs a polynomial function using the coefficients in the
  /// given order.
  /// @param coefficients The coefficients should be ordered like so [0]*x^0 [1]
  /// * x^1 if not set reverse_order = false
  PolynomialFunc(const std::array<T, S> &coefficients)
      : ParameterisedBaseFunc(coefficients) {}

  virtual ~PolynomialFunc() = default;

  /// @brief Returns the y value of the given function at position x.
  /// @param x Position to evaluate the function at.
  /// @return Value of the function at x.
  virtual T operator()(T x) const override {
    T y = 0;
    size_t inverse_iter = coeffs_.size() - 1;

    for (size_t i = 0; i < coeffs_.size(); i++) {
      y += static_cast<T>(
          coeffs_.at(i) *
          std::pow(static_cast<double>(x), static_cast<double>(inverse_iter)));
      --inverse_iter;
    }

    return y;
  }
};

class LogisticalFunc : public ParameterisedBaseFunc<double, 3> {
 public:
  /// @brief Default constructor initializes all coefficients to 0.
  LogisticalFunc() : ParameterisedBaseFunc() {}

  /// @brief Constructs a logistical function using the coefficients in the
  /// given order.
  /// @param coefficients The coefficients should be ordered like so [0] = l
  /// [1] = k [2] = x0
  LogisticalFunc(const std::array<double, 3> &coefficients)
      : ParameterisedBaseFunc(coefficients) {}

  virtual ~LogisticalFunc() = default;

  /// @brief Returns the y value of the given function at position x as a
  /// double.
  /// @param x Double value of the position to evaluate the function at.
  /// @return copy of the value of the function at x.
  virtual double operator()(double x) const override {
    // coeffs_[0] = l, coeffs_[1] = k, coeffs_[2] = x0
    // l / (1 + exp(-k * (x - x0)))
    return coeffs_[0] / (1 + std::exp(-coeffs_[1] * (x - coeffs_[2])));
  }
};

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_MATH_H_