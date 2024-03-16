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

#include <gtest/gtest.h>

namespace spauly {
namespace vccore {
namespace impl {
namespace vccore_testing {
namespace {

TEST(AccuracyTypeTest, string_initizalization) {
  AccuracyType accuracy_type("123.456");
  EXPECT_EQ(accuracy_type.get_int_value(), 123456);
  EXPECT_EQ(accuracy_type.get_exp(), 3);
  EXPECT_FALSE(accuracy_type.get_neg())
}

}  // namespace
}  // namespace vccore_testing
}  // namespace impl
}  // namespace vccore
}  // namespace spauly
