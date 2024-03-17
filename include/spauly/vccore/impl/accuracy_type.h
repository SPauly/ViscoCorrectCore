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
#ifndef SPAULY_VCCORE_IMPL_ACCURACY_TYPE_H_
#define SPAULY_VCCORE_IMPL_ACCURACY_TYPE_H_

#include <cstddef>
#include <cmath>
#include <limits>
#include <string>

namespace spauly {
namespace vccore {
namespace impl {

/// AccuracyType stores floating point number as their integer representation
/// together with an exponent to the base 10. This allows for more accurate
/// arithmetic. When get_double() is called this representation is converted
/// back to a double which may introduce rounding errors.
class AccuracyType {
 public:
  // used types
  using NormType = unsigned long long;

  AccuracyType() = default;

  /// This constructor has the overhead of determining the decimals of the input
  /// number. It also can produce a larger overhead since not all real numbers
  /// can be represented as double. E.g. 0.06 could be represented as
  /// 0.059999999999999997. Use std::string input for better accuracy.
  AccuracyType(const double& value);

  /// This constructor exposes the internal representation of the number. The
  /// resulting value is computed as: value * 10^exp and must fit into a double.
  /// Otherwise int_value is set to INFINITY.
  explicit AccuracyType(const unsigned long long& value,
                        const uint32_t& exp = 0);

  /// Parses the input string and stores the number as int_value and exp. If
  /// the input is not in the format +/-[0-9]*(.[0-9]*)* it is considered
  /// invalid and int_value is set to NAN. If the number is too large to be
  /// converted to a double int_value is set to INFINITY.
  AccuracyType(const std::string& value);

  ~AccuracyType() = default;

  // Getters

  /// The valid flag is set to false if the input was not representable in the
  /// given format.
  inline const bool& is_valid() const { return is_valid_; }

  /// Returns the value stored as integer. Which needs to be devided by 10^exp
  /// to retrieve the original value. Note however that this may introduce
  /// inaccuracies.
  inline const unsigned long long& get_int_value() const { return int_value_; }

  /// Returns the exponent to the base 10 by which the int_value value needs to
  /// be devided.
  inline const uint32_t& get_exp() const { return exp_; }

  /// Returns the sign of the stored number.
  inline const bool& get_neg() const { return neg_; }

  /// Calculates the double representation: int_value / 10^exp. This may
  /// introduce inaccuracies caused by the double representation.
  double get_double() const;

  // Type conversion
  explicit operator double() const { return get_double(); }

  AccuracyType& operator=(const double& value);
  AccuracyType& operator=(const std::string& str);

  // Multiplication is supported without loss of accuracy.
  AccuracyType& operator*=(const AccuracyType& other);
  template <typename T>
  friend AccuracyType operator*(T& num, AccuracyType& acc_t) {}
  template <typename T>
  friend AccuracyType operator*(AccuracyType& acc_t, T& num) {}

  // Devision may introduce some error margin since the result needs to be
  // converted back from its double value.
  AccuracyType& operator/=(const AccuracyType& other);
  template <typename T>
  friend AccuracyType operator/(T& num, AccuracyType& acc_t) {}
  template <typename T>
  friend AccuracyType operator/(AccuracyType& acc_t, T& num) {}

 protected:
  // Error_state is used to store the type of error
  enum class ErrorState { kNone, kNAN, kINFINITY };

  /// Invalidates the AccuracyType and sets int_value to INFINITX if the input
  /// was to large or to NAN if the input could not be parsed.
  void Invalidate(ErrorState error_state);

  bool FromString(const std::string& value);
  bool FromDouble(const double& value) { return true; }

 private:
  bool is_valid_ = true;
  ErrorState error_state_ = ErrorState::kNone;

  NormType int_value_ = 0;
  uint32_t exp_ = 0;
  bool neg_ = false;
};

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_ACCURACY_TYPE_H_