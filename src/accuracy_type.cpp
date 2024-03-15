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

BoolStruct AccuracyType::CreateFromString(std::string& str) {
  static const std::string kDigits = "0123456789";

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
      normalized_ = NAN, exp_ = NAN, neg_ = false;
      return false;
    }

    exp_ = 0;

  } else {
    // Check that the values on both sides of the decimal point are digits
    if (str.find_first_not_of(kDigits) != pos ||
        str.find_first_not_of(kDigits, pos + 1) != std::string::npos) {
      // This means we have a non-digit character in the string
      normalized_ = NAN, exp_ = NAN, neg_ = false;
      return false;
    }

    // Check if the input is identically zero.
    if (str == std::string(".")) {
      normalized_ = 0;
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
  if (exp_ > kMaxExp || str.size() > kMaxDigits) {
    normalized_ = INFINITY;
    return false;
  } else if (exp_ < kMinExp) {
    normalized_ = 0;
    return false;
  }

  if (str.empty()) {
    // This means the input was identically zero
    normalized_ = 0;
    return true;
  }

  // Convert the string to a number
  normalized_ = std::stoull(str);

  return true;
}
}  // namespace impl
}  // namespace vccore
}  // namespace spauly