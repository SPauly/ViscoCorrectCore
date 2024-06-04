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
#include "spauly/vccore/impl/accuracy_type.h"

namespace spauly {
namespace vccore {
namespace impl {
namespace vccore_testing {
namespace {

class FunctionTests : public testing::Test {
 protected:
  virtual void SetUp() override {
    square_ = {1, 0, 1};  // = x^2 + 1
    linear_ = {-1, 2};    // = 2x - 1
    komplex_ = {4, 7,  8,
                9, 10, 11};  // = 11*x^5 + 10*x^4 + 9*x^3 + 8*x^2 + 7x + 4

    d_komplex_ = {
        DOUBLE_T(4.3286373442021278e-09), DOUBLE_T(-6.5935466655309209e-06),
        DOUBLE_T(0.0039704102541411324),  DOUBLE_T(-1.1870337647376101),
        DOUBLE_T(176.52190832690891),     DOUBLE_T(-10276.558815133236)};
  }

 protected:
  std::array<int, 3> square_;
  std::array<int, 2> linear_;
  std::array<int, 6> komplex_;

  std::array<DoubleT, 6> d_komplex_;
};

TEST_F(FunctionTests, Polynomical) {}

}  // namespace

}  // namespace vccore_testing

}  // namespace impl

}  // namespace vccore

}  // namespace spauly
