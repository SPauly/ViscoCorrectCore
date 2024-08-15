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
#include <gtest/gtest.h>

#include "spauly/vccore/calculator.h"

namespace spauly {
namespace vccore {
namespace vccore_testing {
namespace {

class CalculatorTests : public testing::Test {
 protected:
  virtual void SetUp() override {}

 protected:
  Calculator c_;
};

TEST_F(CalculatorTests, ConvertTBaseTest) {
  EXPECT_EQ(c_.ConvertValueToBase(1.0, FlowrateUnit::kLitersPerMinute), 0.06);
  EXPECT_EQ(c_.ConvertValueToBase(10.0, FlowrateUnit::kLitersPerMinute), 0.6);
  EXPECT_EQ(c_.ConvertValueToBase(100.0, FlowrateUnit::kLitersPerMinute), 6.0);

  EXPECT_EQ(c_.ConvertValueToBase(1.0, FlowrateUnit::kGallonsPerMinute),
            0.227125);

  EXPECT_EQ(c_.ConvertValueToBase(1.0, HeadUnit::kFeet), 0.3048);
  EXPECT_EQ(c_.ConvertValueToBase(10.0, HeadUnit::kFeet), 3.048);

  EXPECT_EQ(c_.ConvertValueToBase(1.0, DensityUnit::kGramPerLiter), 1.0);
};

}  // namespace

}  // namespace vccore_testing

}  // namespace vccore

}  // namespace spauly
