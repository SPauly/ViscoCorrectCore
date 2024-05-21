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

#ifndef VCCORE_USE_ACCURACY_TYPE
using AccType = double;
#else
using AccType = AccuracyType;

/// @brief AccuracyType stores floating point numbers as their integer
/// representation together with an exponent to the base 10. This allows for
/// more accurate arithmetic. When get_double() is called this representation is
/// converted back to a double which may introduce rounding errors.
class AccuracyType {
 public:
  // used types
  using IntType = uint64_t;

  /// @brief Default constructor initializes the number to 0.
  AccuracyType() = default;

  /// @brief This constructor has the overhead of converting the input into a
  /// string using std::to_string. It also can introduce percision errors since
  /// not all number can be represented exact. E.g. 0.06 could be represented as
  /// 0.059999999999999997. Use std::string input for better accuracy.
  /// @param value The number to store in double format.
  /// @param precision The precision with which the number is stored.
  AccuracyType(const double& value, const size_t& precision = 17);

  /// @brief This constructor exposes the internal representation of the number.
  /// This is useful for debugging and testing purposes.
  /// @param value The number to store as integer.
  /// @param exp The exponent to the base 10 by which the value needs to be
  /// devided.
  /// @param neg The sign of the number.
  constexpr AccuracyType(const uint64_t& value, const uint32_t& exp,
                         bool neg = false)
      : int_value_(value), exp_(exp), neg_(neg) {}

  /// @brief Parses the input string and stores the number as int_value and exp.
  /// If the input is not in the format +/-[0-9]*(.[0-9]*)* it is considered
  /// invalid and int_value is set to NAN. If the number is too large to be
  /// converted to a double int_value is set to INFINITY.
  /// @param value The number to store as string.
  AccuracyType(const std::string& value);

  ~AccuracyType() = default;

  // Getters

  /// @brief The valid flag is set to false if the input was not representable
  /// in the given format.
  /// @return true if the number is valid else false.
  inline const bool& is_valid() const { return is_valid_; }

  /// @brief Returns the value stored as integer. Which needs to be devided by
  /// 10^exp to retrieve the original value. Note however that this may
  /// introduce inaccuracies.
  /// @return The integer representation of the number.
  inline const uint64_t& get_int_value() const { return int_value_; }

  /// @brief Returns the exponent to the base 10 by which the int_value value
  /// needs to be devided.
  /// @return The exponent to the base 10.
  inline const uint32_t& get_exp() const { return exp_; }

  /// @brief Returns the sign of the stored number.
  /// @return true if the number is negative else false.
  inline const bool& get_neg() const { return neg_; }

  /// @brief Returns the precision with with double input is converted to
  /// string.
  /// @return The precision with which the number is stored.
  inline const size_t& get_input_precision() const { return input_precision_; }

  /// @brief Calculates the double representation: int_value / 10^exp. This may
  /// introduce inaccuracies caused by the double representation.
  /// @return The double representation of the number.
  double get_double() const;

  // Setters
  /// @brief Sets the precision with which double input is converted to string.
  /// Note that a higher precision affects runtime and memory usage and my
  /// introduce inaccuracies. Additionally this may cause INFINITY errors when
  /// the numbers get too big to fit into uint64_t. The default precision is 17.
  /// @param precision The precision with which the number is stored.
  inline void set_input_precision(const size_t& precision) {
    input_precision_ = precision;
  }

  // Type conversion
  explicit operator double() const { return std::move(get_double()); }

  operator std::string() const {
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

  /// @brief Returns true if the stored number is equal to the other number.
  /// Comparisson is performed by checking the equality of both int value and
  /// exponent.
  /// @param other The number to compare to.
  /// @return true if this == other else false.
  bool operator==(const AccuracyType& other) const;
  /// @brief Returns true if the stored number is equal to the other number. The
  /// comparison is performed by calling this->get_double() == other.
  /// @param other double to compare to.
  /// @return true if this == other else false.
  bool operator==(const double& other) const;
  /// @brief Returns true if the stored number is not equal to the other number.
  /// Comparrisson is performed by checking the equality of both int value and
  /// exponent. Stops as soon as one of the values is not equal.
  /// @param other The number to compare to.
  /// @return true if this != other else false.
  bool operator!=(const AccuracyType& other) const;
  /// @brief Returns true if the stored number is not equal to the other number.
  /// The comparison is performed by calling this->get_double() != other.
  /// @param other double to compare to.
  /// @return true if this != other else false.
  bool operator!=(const double& other) const;

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

#endif

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_ACCURACY_TYPE_H_