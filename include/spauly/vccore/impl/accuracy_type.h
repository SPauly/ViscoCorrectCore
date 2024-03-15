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
#include <numeric_limits>
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

  // static const locals
  static constexpr size_t kMaxDigits = std::numeric_limits<NormType>::digits;
  static constexpr int32_t kMaxExp =
      std::numeric_limits<double>::max_exponent10;
  static constexpr int32_t kMinExp =
      std::numeric_limits<double>::min_exponent10;

  AccuracyType() = delete;
  AccuracyType(const double& value);
  AccuracyType(const unsigned long long& value, const size_t& exp = 1);
  AccuracyType(std::string str);
  ~AccuracyType() = default;

  AccuracyType(const AccuracyType& other);
  AccuracyType(AccuracyType&& other) noexcept;
  AccuracyType& operator=(const AccuracyType& other);
  AccuracyType& operator=(AccuracyType&& other) noexcept;

  const double& get_double();
  const unsigned long long& get_normalized() const;
  const size_t& get_exp() const;

  AccuracyType operator=(const double& value);
  AccuracyType operator+(const AccuracyType& other) const;
  AccuracyType operator-(const AccuracyType& other) const;
  AccuracyType operator*(const AccuracyType& other) const;
  AccuracyType operator/(const AccuracyType& other) const;

 protected:
  void CreateFromString(std::string& str);

 private:
  NormType normalized_;
  int32_t exp_;
  bool neg_;
};

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_ACCURACY_TYPE_H_