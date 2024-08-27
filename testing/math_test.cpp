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

#include "spauly/vccore/impl/math.h"

namespace spauly {
namespace vccore {
namespace impl {
namespace vccore_testing {
namespace {

class FunctionTests : public testing::Test {
 protected:
  virtual void SetUp() override {
    square_ = {1, 0, 1};  // = x^2 + 1
    linear_ = {-1, 2};    // = -x +2
    komplex_ = {4, 7,  8,
                9, 10, 11};  // = 4*x^5 + 7*x^4 + 8*x^3 + 9*x^2 + 10*x + 11

    d_komplex_ = {
        DoubleT(4.3286373442021278e-09),
        DoubleT(-6.5935466655309209e-06),
        DoubleT(0.0039704102541411324),
        DoubleT(-1.1870337647376101),
        DoubleT(176.52190832690891),
        DoubleT(
            -10276.558815133236)};  // = 4.3286373442021278e-09*x^5 -
                                    // 6.5935466655309209e-06*x^4 +
                                    // 0.0039704102541411324*x^3 -
                                    // 1.1870337647376101*x^2 +
                                    // 176.52190832690891*x - 10276.558815133236

    d_logistic_ = {285.39113639063004, -0.019515612319848788,
                   451.79876054847699};  // 0.6
  }

 protected:
  std::array<int, 3> square_;
  std::array<int, 2> linear_;
  std::array<int, 6> komplex_;

  std::array<DoubleT, 6> d_komplex_;

  std::array<double, 3> d_logistic_;
};

TEST_F(FunctionTests, PolynomialTest) {
  // Test the square function
  PolynomialFunc<int, 3> square_func(square_);
  EXPECT_EQ(square_func(0), 1);
  EXPECT_EQ(square_func(2), 5);

  // Test the linear function
  PolynomialFunc<int, 2> linear_func(linear_);
  EXPECT_EQ(linear_func(0), 2);
  EXPECT_EQ(linear_func(2), 0);

  // Test the komplex function
  PolynomialFunc<int, 6> komplex_func(komplex_);
  EXPECT_EQ(komplex_func(0), 11);
  EXPECT_EQ(komplex_func(3), 1877);

  // Test the double komplex function
  PolynomialFunc<DoubleT, 6> d_komplex_func(d_komplex_);
  EXPECT_NEAR(d_komplex_func(0), -10276.558815133236, 1e-5);
  EXPECT_NEAR(d_komplex_func(242), 173.48327475572842, 1e-5);
}

TEST_F(FunctionTests, LogisticTest) {
  LogisticalFunc lfunc(d_logistic_);
  EXPECT_NEAR(lfunc(146), 284.6624748102717, 1e-10);
  EXPECT_NEAR(lfunc(382), 227.20314602110616, 1e-10);
}
}  // namespace

}  // namespace vccore_testing

}  // namespace impl

}  // namespace vccore

}  // namespace spauly
