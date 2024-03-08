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

#include "spauly/vccore/impl/correction_context.h"

namespace spauly {
namespace vccore {
namespace impl {
namespace vccore_testing {
namespace {

class CorrectionContextTest : public ::testing::Test {
 protected:
  CorrectionContextTest() : context_() {}

 protected:
  // Create standard context
  CorrectionContext context_;
};

TEST_F(CorrectionContextTest, Initialization) {}

}  // namespace

}  // namespace vccore_testing
}  // namespace impl
}  // namespace vccore
}  // namespace spauly