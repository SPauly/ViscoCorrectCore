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

template <typename T, size_t S>
class ParameterisedBaseFunc {
 public:
  /// @brief Default constructor initializes all coefficients to 0.
  ParameterisedBaseFunc() : coeffs_(std::array<T, S>(0)) {}
  /// @brief Initializes the coefficients in the correct order.
  /// @param coefficients The coefficients should be ordered like so [0]*x^0 [1]
  /// * x^1 ...
  ParameterisedBaseFunc(const std::array<T, S> &coefficients)
      : coeffs_(coefficients) {}

  virtual ~ParameterisedBaseFunc() = default;

  /// @brief Returns the y value of the given function at position x. Here the
  /// Function dependent code should be implemented.
  /// @param x -Value
  /// @return function value.
  virtual T operator()(T x) const = 0;

 protected:
  std::array<T, S> coeffs_;
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
    T result = 0;
    for (size_t i = 0; i < coeffs_.size(); ++i) {
      result += coeffs_[i] * std::pow(x, i);
    }
    return result;
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

  /// @brief Returns the y value of the given function at position x as a
  /// double.
  /// @param x Integer value of the position to evaluate the function at. Will
  /// be casted via static_cast.
  /// @return copy of the value of the function at x.
  template <typename T>
  double operator()(T x) const {
    // coeffs_[0] = l, coeffs_[1] = k, coeffs_[2] = x0
    // l / (1 + exp(-k * (x - x0)))
    return coeffs_[0] /
           (1 + std::exp(-coeffs_[1] * (static_cast<double>(x) - coeffs_[2])));
  };
};

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_MATH_H_