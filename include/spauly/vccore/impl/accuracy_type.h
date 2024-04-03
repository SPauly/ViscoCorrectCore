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
#include <cstdint>
#include <limits>
#include <iomanip>
#include <string>
#include <sstream>

namespace spauly {
namespace vccore {
namespace impl {

// Forward declaration
class AccuracyType;

// AccType represents the internally used type for the calculations.
using AccType = AccuracyType;

/// AccuracyType stores floating point number as their integer representation
/// together with an exponent to the base 10. This allows for more accurate
/// arithmetic. When get_double() is called this representation is converted
/// back to a double which may introduce rounding errors.
class AccuracyType {
 public:
  // used types
  using IntType = uint64_t;

  AccuracyType() = default;

  /// This constructor has the overhead of converting the input into a string
  /// using std::to_string. It also can introduce percision errors since not all
  /// number can be represented exact. E.g. 0.06 could be represented as
  /// 0.059999999999999997. Use std::string input for better accuracy.
  explicit AccuracyType(const double& value, const size_t& precision = 17);

  /// This constructor exposes the internal representation of the number.
  explicit constexpr AccuracyType(const uint64_t& value, const uint32_t& exp,
                                  bool neg = false)
      : int_value_(value), exp_(exp), neg_(neg) {}

  /// Parses the input string and stores the number as int_value and exp. If
  /// the input is not in the format +/-[0-9]*(.[0-9]*)* it is considered
  /// invalid and int_value is set to NAN. If the number is too large to be
  /// converted to a double int_value is set to INFINITY.
  explicit AccuracyType(const std::string& value);

  ~AccuracyType() = default;

  // Getters

  /// The valid flag is set to false if the input was not representable in the
  /// given format.
  inline const bool& is_valid() const { return is_valid_; }

  /// Returns the value stored as integer. Which needs to be devided by 10^exp
  /// to retrieve the original value. Note however that this may introduce
  /// inaccuracies.
  inline const uint64_t& get_int_value() const { return int_value_; }

  /// Returns the exponent to the base 10 by which the int_value value needs to
  /// be devided.
  inline const uint32_t& get_exp() const { return exp_; }

  /// Returns the sign of the stored number.
  inline const bool& get_neg() const { return neg_; }

  /// Returns the precision with with double input is converted to string.
  inline const size_t& get_input_precision() const { return input_precision_; }

  /// Calculates the double representation: int_value / 10^exp. This may
  /// introduce inaccuracies caused by the double representation.
  double get_double() const;

  // Setters
  /// Sets the precision with which double input is converted to string. Note
  /// that a higher precision affects runtime and memory usage and my introduce
  /// inaccuracies. Additionally this may cause INFINITY errors when the numbers
  /// get too big to fit into uint64_t. The default precision is 17.
  inline void set_input_precision(const size_t& precision) {
    input_precision_ = precision;
  }

  // Type conversion
  explicit operator double() const { return std::move(get_double()); }

  explicit operator std::string() const {
    std::stringstream sstr;
    sstr << std::setprecision(input_precision_) << this->get_double();
    return sstr.str();
  }

  AccuracyType& operator=(const AccuracyType& other);
  AccuracyType& operator=(const double& value);
  AccuracyType& operator=(const std::string& str);

  // Multiplication is supported without loss of accuracy.
  AccuracyType& operator*=(const AccuracyType& other);

  // Support for multiplication with other types. Must be convertable to double.
  // This may introduce precision errors unless T is an AccuracyType.
  friend AccuracyType operator*(const AccuracyType& acc_t,
                                const AccuracyType& other) {
    AccuracyType result;
    result = acc_t;
    result *= other;
    return result;
  }

  template <typename T>
  friend AccuracyType operator*(const T& num, const AccuracyType& acc_t) {
    AccuracyType result;

    static_assert(std::is_convertible<T, double>::value,
                  "T must be convertable to double.");
    result = static_cast<AccuracyType>(static_cast<double>(num));
    result *= acc_t;
    return result;
  }

  template <typename T>
  friend AccuracyType operator*(const AccuracyType& acc_t, const T& num) {
    return num * acc_t;
  }

  // Devision may introduce some precision error since the result needs to be
  // converted back from its double representation.
  AccuracyType& operator/=(const AccuracyType& other);

  // Support for division with other types. Must be convertable to double.
  friend AccuracyType operator/(const AccuracyType& acc_t,
                                const AccuracyType& other) {
    AccuracyType result;
    result = acc_t;
    result /= other;
    return result;
  }

  template <typename T>
  friend AccuracyType operator/(const T& num, const AccuracyType& acc_t) {
    AccuracyType result;

    static_assert(std::is_convertible<T, double>::value,
                  "T must be convertable to double.");
    result = static_cast<AccuracyType>(static_cast<double>(num));
    result /= acc_t;
    return result;
  }
  template <typename T>
  friend AccuracyType operator/(const AccuracyType& acc_t, const T& num) {
    AccuracyType result;

    static_assert(std::is_convertible<T, double>::value,
                  "T must be convertable to double.");
    result = acc_t;
    result /= static_cast<AccuracyType>(static_cast<double>(num));
    return result;
  }

 protected:
  // Error_state is used to store the type of error
  enum class ErrorState { kNone, kNAN, kINFINITY };

  /// Invalidates the AccuracyType and sets int_value to INFINITX if the input
  /// was to large or to NAN if the input could not be parsed.
  void Invalidate(ErrorState error_state);

  bool FromString(const std::string& value);
  bool FromDouble(const double& value);

  // This helper function retrieves the exponent from a string in scientific
  // notation.Should start with e or E.
  int32_t RetrieveExponent(const std::string& value) const;

 private:
  bool is_valid_ = true;
  ErrorState error_state_ = ErrorState::kNone;
  size_t input_precision_ =
      17;  // 17 seems to be a good sweet spot for my purposes

  IntType int_value_ = 0;
  uint32_t exp_ = 0;
  bool neg_ = false;
};

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_ACCURACY_TYPE_H_