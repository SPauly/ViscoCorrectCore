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

namespace spauly {
namespace vccore {
namespace impl {
class AccuracyType {
 public:
  AccuracyType() = delete;
  AccuracyType(const double& value);
  AccuracyType(const long long& value, const size_t& exp = 1);
  ~AccuracyType() = default;

  AccuracyType(const AccuracyType& other);
  AccuracyType(AccuracyType&& other) noexcept;
  AccuracyType& operator=(const AccuracyType& other);
  AccuracyType& operator=(AccuracyType&& other) noexcept;

  const double& get_double() const;
  const long long& get_long_long() const;
  const size_t& get_exp() const;

  AccuracyType operator=(const double& value);
  AccuracyType operator+(const AccuracyType& other) const;
  AccuracyType operator-(const AccuracyType& other) const;
  AccuracyType operator*(const AccuracyType& other) const;
  AccuracyType operator/(const AccuracyType& other) const;

 private:
  double value_;
  long long int_value_;
  size_t exp_;
};

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_ACCURACY_TYPE_H_