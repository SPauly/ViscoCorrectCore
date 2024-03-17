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

#include <limits>

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
  EXPECT_EQ(accuracy_type.get_double(), 123.456);
  EXPECT_FALSE(accuracy_type.get_neg());

  accuracy_type = AccuracyType("+123.456");
  EXPECT_EQ(accuracy_type.get_int_value(), 123456);
  EXPECT_FALSE(accuracy_type.get_neg());

  accuracy_type = AccuracyType("-123.456");
  EXPECT_EQ(accuracy_type.get_int_value(), 123456);
  EXPECT_EQ(accuracy_type.get_exp(), 3);
  EXPECT_EQ(accuracy_type.get_double(), -123.456);
  EXPECT_TRUE(accuracy_type.get_neg());

  accuracy_type = AccuracyType("0.");
  EXPECT_EQ(accuracy_type.get_int_value(), 0);

  accuracy_type = AccuracyType("0.0");
  EXPECT_EQ(accuracy_type.get_int_value(), 0);

  accuracy_type = AccuracyType("0.06");
  EXPECT_EQ(accuracy_type.get_int_value(), 6);
  EXPECT_EQ(accuracy_type.get_exp(), 2);
  EXPECT_EQ(accuracy_type.get_double(), 0.059999999999999997);

  accuracy_type = AccuracyType(".0");
  EXPECT_EQ(accuracy_type.get_int_value(), 0);

  accuracy_type = AccuracyType(".");
  EXPECT_EQ(accuracy_type.get_int_value(), 0);

  accuracy_type = AccuracyType("0.1234");
  EXPECT_EQ(accuracy_type.get_int_value(), 1234);
  EXPECT_EQ(accuracy_type.get_exp(), 4);

  accuracy_type = AccuracyType("1234");
  EXPECT_EQ(accuracy_type.get_int_value(), 1234);
  EXPECT_EQ(accuracy_type.get_exp(), 0);

  accuracy_type = AccuracyType("1234.0");
  EXPECT_EQ(accuracy_type.get_int_value(), 1234);

  accuracy_type = AccuracyType(".123");
  EXPECT_EQ(accuracy_type.get_int_value(), 123);
  EXPECT_EQ(accuracy_type.get_exp(), 3);

  // Check non valid inputs
  accuracy_type = AccuracyType("123.456.789");
  EXPECT_TRUE(accuracy_type.get_int_value() == 0);
  EXPECT_TRUE(std::isnan(accuracy_type.get_double()));

  accuracy_type = AccuracyType("123.A");
  EXPECT_TRUE(std::isnan(accuracy_type.get_double()));

  // Check overflow
  accuracy_type = AccuracyType("18.446744073709551615");
  EXPECT_EQ(accuracy_type.get_double(), 18.446744073709553);

  accuracy_type = AccuracyType("18.446744073709551616");
  EXPECT_EQ(accuracy_type.get_double(),
            std::numeric_limits<double>::infinity());
}

TEST(AccuracyTypeTest, double_initialization) {
  AccuracyType accuracy_type(123.456);
  EXPECT_EQ(accuracy_type.get_int_value(), 123456);
  EXPECT_EQ(accuracy_type.get_exp(), 3);
  EXPECT_EQ(accuracy_type.get_double(), 123.456);
  EXPECT_FALSE(accuracy_type.get_neg());

  accuracy_type = AccuracyType(-123.456);
  EXPECT_EQ(accuracy_type.get_int_value(), 123456);
  EXPECT_EQ(accuracy_type.get_exp(), 3);
  EXPECT_EQ(accuracy_type.get_double(), -123.456);
  EXPECT_TRUE(accuracy_type.get_neg());

  accuracy_type = AccuracyType(0.0);
  EXPECT_EQ(accuracy_type.get_int_value(), 0);

  accuracy_type = AccuracyType(0.06);
  EXPECT_NE(accuracy_type.get_int_value(), 6);
  EXPECT_EQ(accuracy_type.get_double(), 0.06);
  accuracy_type.set_input_precision(2);
  accuracy_type = 0.06;
  EXPECT_EQ(accuracy_type.get_int_value(), 6);
  accuracy_type.set_input_precision(17);

  accuracy_type = AccuracyType(0.1234);
  EXPECT_EQ(accuracy_type.get_int_value(), 1234);
  EXPECT_EQ(accuracy_type.get_exp(), 4);

  accuracy_type = AccuracyType(1234.0);
  EXPECT_EQ(accuracy_type.get_int_value(), 1234);
  EXPECT_EQ(accuracy_type.get_exp(), 0);

  accuracy_type = AccuracyType(18.446744073709553);
  EXPECT_EQ(accuracy_type.get_int_value(), 18446744073709553);
  EXPECT_EQ(accuracy_type.get_exp(), 15);
  EXPECT_EQ(accuracy_type.get_double(), 18.446744073709553);
}

TEST(AccuracyTypeTest, conversion) {
  AccuracyType accuracy_type("123.456");
  EXPECT_EQ(static_cast<double>(accuracy_type), 123.456);
  EXPECT_STREQ(static_cast<std::string>(accuracy_type).c_str(), "123.456");
}

TEST(AccuracyTypeTest, assignement) {
  AccuracyType accuracy_type;
  accuracy_type = "123.456";
  EXPECT_EQ(accuracy_type.get_int_value(), 123456);
  EXPECT_EQ(accuracy_type.get_exp(), 3);

  accuracy_type = 123.456;
  EXPECT_EQ(accuracy_type.get_int_value(), 123456);

  AccuracyType accuracy_type2("7");
  accuracy_type = accuracy_type2;
  EXPECT_EQ(accuracy_type.get_int_value(), 7);
  EXPECT_EQ(accuracy_type.get_exp(), 0);
  EXPECT_EQ(accuracy_type.get_double(), 7.0);
}

TEST(AccuracyTypeTest, multiplication) {
  AccuracyType accuracy_type("123.456");
  accuracy_type *= accuracy_type;
  EXPECT_EQ(accuracy_type.get_int_value(), 15241384);
  EXPECT_EQ(accuracy_type.get_exp(), 6);
  EXPECT_EQ(accuracy_type.get_double(), 152.41384);

  accuracy_type = "123.456";
  accuracy_type *= 2;
  EXPECT_EQ(accuracy_type.get_int_value(), 246912);
  EXPECT_EQ(accuracy_type.get_exp(), 3);
  EXPECT_EQ(accuracy_type.get_double(), 246.912);

  accuracy_type = "123.456";
  accuracy_type *= 0.5;
  EXPECT_EQ(accuracy_type.get_int_value(), 61728);
  EXPECT_EQ(accuracy_type.get_exp(), 3);
  EXPECT_EQ(accuracy_type.get_double(), 61.728);

  accuracy_type = "123.456";
  accuracy_type *= 0.0001;
  EXPECT_EQ(accuracy_type.get_int_value(), 1234);
  EXPECT_EQ(accuracy_type.get_exp(), 0);
  EXPECT_EQ(accuracy_type.get_double(), 0.1234);
}

}  // namespace
}  // namespace vccore_testing
}  // namespace impl
}  // namespace vccore
}  // namespace spauly
