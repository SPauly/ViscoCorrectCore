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

namespace spauly {
namespace vccore {
namespace impl {

AccuracyType::AccuracyType(const double& value, const size_t& precision)
    : input_precision_(precision) {
  FromDouble(value);
}

AccuracyType::AccuracyType(const std::string& value) { FromString(value); }

double AccuracyType::get_double() const {
  if (!is_valid_) {
    if (error_state_ == ErrorState::kINFINITY)
      return std::numeric_limits<double>::infinity();
    return std::numeric_limits<double>::quiet_NaN();
  }

  double result = int_value_ / std::pow(10, exp_);

  if (neg_) result *= -1;

  return result;
}

AccuracyType& AccuracyType::operator=(const AccuracyType& other) {
  if (this == &other) return *this;

  is_valid_ = other.is_valid_;
  error_state_ = other.error_state_;
  int_value_ = other.int_value_;
  exp_ = other.exp_;
  neg_ = other.neg_;
  input_precision_ = other.input_precision_;

  return *this;
}

AccuracyType& AccuracyType::operator=(const double& value) {
  is_valid_ = true;
  error_state_ = ErrorState::kNone;
  FromDouble(value);
  return *this;
}

AccuracyType& AccuracyType::operator=(const std::string& str) {
  is_valid_ = true;
  error_state_ = ErrorState::kNone;
  FromString(str);
  return *this;
}

AccuracyType& AccuracyType::operator*=(const AccuracyType& other) {
  if (!is_valid_) return *this;
  if (!other.is_valid_) {
    Invalidate(other.error_state_);
    return *this;
  }

  // Check for overflow
  if (int_value_ != 0 &&
      other.int_value_ > std::numeric_limits<uint64_t>::max() / int_value_) {
    // Attempt to truncate one of the multiplicands to avoid overflow
    AccuracyType tmp = (exp_ > other.exp_) ? *this : other;
    const AccuracyType& tmp_other = (exp_ > other.exp_) ? other : *this;

    // Truncate the number with the larger exponent until fitting
    while (tmp.exp_ > 0 &&
           tmp.int_value_ >
               std::numeric_limits<uint64_t>::max() / tmp_other.int_value_) {
      tmp.int_value_ /= 10;
      tmp.exp_--;
    }

    // See if it is still too large
    if (tmp.int_value_ >
        std::numeric_limits<uint64_t>::max() / tmp_other.int_value_) {
      Invalidate(ErrorState::kINFINITY);
      return *this;
    } else {
      tmp.int_value_ *= tmp_other.int_value_;
      tmp.exp_ = (tmp_other.exp_ > tmp.exp_) ? tmp_other.exp_ : tmp.exp_;
      *this = tmp;
      return *this;
    }
  }

  int_value_ *= other.int_value_;
  exp_ = (other.exp_ > exp_) ? other.exp_ : exp_;

  // Set the sign of the result (using xor)
  neg_ ^= other.neg_;

  return *this;
}

AccuracyType& AccuracyType::operator/=(const AccuracyType& other) {
  if (!is_valid_ || !other.is_valid_) return *this;
  if (other.int_value_ == 0) {
    Invalidate(ErrorState::kINFINITY);
    return *this;
  }

  // If the exponents do not match they need to be adjusted before the division.
  if (exp_ < other.exp_) {
    int_value_ *= static_cast<IntType>(std::pow(10, other.exp_ - exp_));
    exp_ = other.exp_;

    operator=(static_cast<double>(int_value_ / other.int_value_));
  } else if (exp_ > other.exp_) {
    operator=(static_cast<double>(
        int_value_ / (other.int_value_ * std::pow(10, exp_ - other.exp_))));
  }

  return *this;
}

void AccuracyType::Invalidate(ErrorState error_state) {
  int_value_ = 0;
  exp_ = 0;
  neg_ = false;

  is_valid_ = false;
  error_state_ = error_state;
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
      Invalidate(ErrorState::kNAN);
      return false;
    }

    exp_ = 0;

  } else {
    // Check that the values on both sides of the decimal point are digits
    if (str.find_first_not_of(kDigits) != pos ||
        str.find_first_not_of(kDigits, pos + 1) != std::string::npos) {
      // This means we have a non-digit character in the string
      Invalidate(ErrorState::kNAN);
      return false;
    }

    // Check if the input is identically zero.
    if (str == std::string(".")) {
      int_value_ = 0;
      exp_ = 0;
      return true;
    }

    // Calculate the exponent based on the position of the decimal point
    exp_ = static_cast<uint32_t>(str.size() - pos - 1u);

    // Remove the decimal point from the string
    str.erase(pos, 1);
  }

  // Now we should have a string of digits
  // Check for underflow
  if (str.empty()) {
    int_value_ = 0;
    return true;
  }

  // Convert the string to a number
  try {
    int_value_ = std::stoull(str);
  } catch (const std::exception&) {
    // stoull throws an exception if the input is too large or cannot be
    // converted.
    Invalidate(ErrorState::kINFINITY);
    return false;
  }

  return true;
}

bool AccuracyType::FromDouble(const double& value) {
  if (std::isnan(value)) {
    Invalidate(ErrorState::kNAN);
    return false;
  } else if (std::isinf(value)) {
    Invalidate(ErrorState::kINFINITY);
    return false;
  }

  try {
    std::stringstream sstr;
    sstr << std::setprecision(input_precision_) << value;

    return FromString(sstr.str());
  } catch (const std::exception&) {
    Invalidate(ErrorState::kNAN);
    return false;
  }
}

}  // namespace impl
}  // namespace vccore
}  // namespace spauly