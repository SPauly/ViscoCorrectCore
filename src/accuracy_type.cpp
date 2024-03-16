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
#include "spauly/vccore/impl/accuracy_type.h"

#include <cmath>  //used for std::modf to retrieve the fractional part of a number

namespace spauly {
namespace vccore {
namespace impl {

AccuracyType::AccuracyType(const double& value) { FromDouble(value); }

AccuracyType::AccuracyType(const unsigned long long& value,
                           const uint32_t& exp) {
  // If the resulting number is too large to be represented as double we neglect
  // this input.
  if (value / std::pow(10, exp + 1) > std::numeric_limits<double>::max()) {
    Invalidate(INFINITY);
    exp_ = 0;
    neg_ = false;
  } else {
    int_value_ = value;
    exp_ = exp;
  }
}

AccuracyType::AccuracyType(const std::string& value) { FromString(value); }

double AccuracyType::get_double() const {
  if (!is_valid_) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  double result = int_value_ / std::pow(10, exp_);

  if (neg_) result *= -1;

  return result;
}

AccuracyType& AccuracyType::operator=(const double& value) {
  FromDouble(value);
  return *this;
}

AccuracyType& AccuracyType::operator=(const std::string& str) {
  FromString(str);
  return *this;
}

AccuracyType& AccuracyType::operator*=(const AccuracyType& other) {
  if (!is_valid_ || !other.is_valid_) return *this;

  int_value_ *= other.int_value_;
  exp_ = (other.exp_ > exp_) ? other.exp_ : exp_;

  return *this;
}

AccuracyType& AccuracyType::operator/=(const AccuracyType& other) {
  if (!is_valid_ || !other.is_valid_) return *this;

  // If the exponents do not match they need to be adjusted before the division.
  if (exp_ < other.exp_) {
    int_value_ *= std::pow(10, other.exp_ - exp_);
    exp_ = other.exp_;

    operator=(static_cast<double>(int_value_ / other.int_value_));
  } else if (exp_ > other.exp_) {
    operator=(static_cast<double>(
        int_value_ / (other.int_value_ * std::pow(10, exp_ - other.exp_))));
  }

  return *this;
}

void AccuracyType::Invalidate(NormType reason) {
  int_value_ = reason;
  exp_ = 0;
  neg_ = false;

  is_valid_ = false;
}

bool AccuracyType::FromString(const std::string& value) {
  static const std::string kDigits = "0123456789";

  is_valid_ = true;
  std::string str = value;

  // Retrieve the sign of the number
  neg_ = false;

  if (str.size()) {
    if (str[0] == '-') {
      neg_ = true;
      str.erase(0, 1);
    } else if (str[0] == '+') {
      str.erase(0, 1);
    }
  }

  // Remove leading zeros
  while (str.size() && str[0] == '0') {
    str.erase(0, 1);
  }

  // Remove trailing zeros
  while (str.size() && str[str.size() - 1] == '0') {
    str.erase(str.size() - 1, 1);
  }

  // Find the position of the decimal point
  size_t pos = str.find('.');

  if (pos == std::string::npos) {
    if (str.find_first_not_of(kDigits) != std::string::npos) {
      // This means we have a non-digit character in the string
      Invalidate(NAN);
      return false;
    }

    exp_ = 0;

  } else {
    // Check that the values on both sides of the decimal point are digits
    if (str.find_first_not_of(kDigits) != pos ||
        str.find_first_not_of(kDigits, pos + 1) != std::string::npos) {
      // This means we have a non-digit character in the string
      Invalidate(NAN);
      return false;
    }

    // Check if the input is identically zero.
    if (str == std::string(".")) {
      int_value_ = 0;
      exp_ = 0;
      return true;
    }

    // Calculate the exponent based on the position of the decimal point
    exp_ = static_cast<size_t>(str.size() - pos - 1);

    // Remove the decimal point from the string
    str.erase(pos, 1);
  }

  // Now we should have a string of digits
  // Check for underflow and overflow
  if (str.size() > kMaxDigits) {
    Invalidate(INFINITY);
    return false;
  } else if (str.empty()) {
    int_value_ = 0;
    return true;
  }

  // Convert the string to a number
  int_value_ = std::stoull(str);

  return true;
}

}  // namespace impl
}  // namespace vccore
}  // namespace spauly