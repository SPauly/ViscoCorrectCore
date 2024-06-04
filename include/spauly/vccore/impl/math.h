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

namespace spauly {
namespace vccore {
namespace impl {

template <typename T, size_t S>
class ParameterisedBaseFunc {
 public:
  ParameterisedBaseFunc() : coeffs_(std::array<T, S>(0)) {}
  /// @brief Initializes the coefficients in the correct order.
  /// @param coefficients The coefficients should be ordered like so [0]*x^0 [1]
  /// * x^1 if not set reverse_order = false
  /// @param reverse_order Set to true by default. If coefficients are ordered
  /// like [0]*x^n [1]*x^n-1 set this to false.
  ParameterisedBaseFunc(const std::array<T, S> &coefficients,
                        bool reverse_order = true) {
    if (reverse_order) {
      // The coefficients need to be stored in reverse order
      for (int i = 0; i < S; i++) {
        coeffs_[i] = coefficients[S - i - 1];
      }
    } else {
      coeffs_ = coefficients;
    }
  }

  /// @brief Returns the y value of the given function at position x. Here the
  /// Function dependent code should be implemented.
  /// @param x -Value
  /// @return function value.
  virtual T operator()(T &x) = 0;

 protected:
  std::array<T, S> coeffs_;
};

template <typename T, size_t S>
class PolynomialFunc {
 public:
  PolynomialFunc() = default;
  /// @brief Constructs a polynomial function using the coefficients in the
  /// given order.
  /// @param coefficients The coefficients should be ordered like so [0]*x^0 [1]
  /// * x^1 if not set reverse_order = false
  /// @param reverse_order Set to true by default. If coefficients are ordered
  /// like [0]*x^n [1]*x^n-1 set this to false.
  PolynomialFunc(const std::array<T, S> &coefficients,
                 bool reverse_order = true) {
    if (reverse_order) {
      // The coefficients need to be stored in reverse order
      for (int i = 0; i < S; i++) {
        coeffs_[i] = coefficients[S - i - 1];
      }
    } else {
      coeffs_ = coefficients;
    }
  }

  T operator()(T &x) const {
    T result = 0;
    for (size_t i = 0; i < coeffs_.size(); ++i) {
      result += coeffs_[i] * std::pow(x, i);
    }
    return result;
  }

 private:
  // The coefficeints of the polynomial function are stored in reverse order
  // meaning: coeffs_[0]*x^0 + coeffs_[1]*x^1 + ... + coeffs_[S-1]*x^(S-1)
  std::array<T, S> coeffs_;
};

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_MATH_H_