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
#include "spauly/vccore/impl/conversion_functions.h"

#include <string>
#include <unordered_map>

#include "spauly/vccore/input_parameters.h"

#include <gtest/gtest.h>

namespace spauly {
namespace vccore {
namespace impl {
namespace vccore_testing {
namespace {
// First param represents the test value the second one the expected conversion.
using TestCasesMap = std::unordered_map<P_type, P_type>;

// Test utilities for Flowrate:
TestCasesMap lpmToCubicMPH{};  // Liters per Minute to m^3/h
TestCasesMap gpmToCubicMPH{};  // Gallons per Minute to m^3/h

// Test utilities for Head
TestCasesMap feetToMeters{};

// Test utilities for Viscosity
TestCasesMap cpmpasTomm2s{};  // Centipoise and Millipascal Seconds to mm^2/s

// Test utilities for Density
TestCasesMap kpcmToGPL{};  // KilogramsPerCubicMeter To GramPerLiter

TEST(ConversionFunctions, LitersPerMinute) {
  // collect all failing cases as string so that they will be provided for the
  // testcase
  std::string errors = "";

  for (const auto& elem : lpmToCubicMPH) {
    P_type res = ConvertToBaseUnit<FlowrateUnit>(
        elem.first, FlowrateUnit::kLitersPerMinute);

    // If the result is not what expected we create a string with the expected
    // and resulting conversion.
    if (res != elem.second) {
      errors += std::to_string(elem.first) + " -> " + std::to_string(res) +
                " != " + std::to_string(elem.second) + '\n';
    }
  }

  EXPECT_STREQ(errors.c_str(), "");
}

}  // namespace

}  // namespace vccore_testing
}  // namespace impl
}  // namespace vccore
}  // namespace spauly