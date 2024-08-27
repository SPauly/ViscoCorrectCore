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
#include <map>

#include <gtest/gtest.h>

namespace spauly {
namespace vccore {
namespace impl {
namespace vccore_testing {
namespace {
// First param represents the test value the second one the expected conversion.
using TestCasesMap = std::map<DoubleT, DoubleT>;

// Test utilities for Flowrate:
TestCasesMap lpmToCubicMPH{
    {1.0, 0.06},      {10.0, 0.6},      {100.0, 6.0},  {1000.0, 60.0},
    {10000.0, 600.0}, {0.5, 0.03},      {5.5, 0.33},   {11.1, 0.666},
    {111.1, 6.666},   {1111.1, 66.666}, {0.11, 0.0066}};  // Liters per
                                                          // Minute to m^3/h
TestCasesMap gpmToCubicMPH{
    {1.0, 0.227125},     {10.0, 2.27125},    {100.0, 22.7125},
    {1000.0, 227.125},   {10000.0, 2271.25}, {0.5, 0.1135625},
    {5.5, 1.2491875},    {11.1, 2.5210875},  {111.1, 25.2335875},
    {1111.1, 252.35858}, {0.11, 0.02498375}};  // Gallons per
                                               // Minute to m^3/h

// Test utilities for Head
TestCasesMap feetToMeters{
    {1.0, 0.3048},     {10.0, 3.048}, {100.0, 30.48},  {1000.0, 304.8},
    {10000.0, 3048.0}, {5.5, 1.6764}, {11.1, 3.38328},
};  // Feet to Meters

// Test utilities for Density
TestCasesMap kpcmToGPL{
    {1.0, 0.001},    {10.0, 0.01},    {100.0, 0.1},   {1000.0, 1.0},
    {10000.0, 10.0}, {0.5, 0.0005},   {5.5, 0.0055},  {11.1, 0.0111},
    {111.1, 0.111},  {1111.1, 1.111}, {0.11, 0.00011}};  // KilogramsPerCubicMeter
                                                         // To GramPerLiter

// Test utilities for Viscosity
// The pair holds the following args: 1. Viscosity in cp/mpas, 2. Density in gPl
std::map<std::pair<DoubleT, DoubleT>, DoubleT> cpmpasTomm2s{
    {{1.0, 1.0}, 1.0},  {{1.0, 2.0}, 0.5},      {{1.0, 3.0}, 0.333333},
    {{1.0, 4.0}, 0.25}, {{1.0, 5.0}, 0.2},      {{2.0, 1.0}, 2.0},
    {{2.0, 2.0}, 1.0},  {{2.0, 3.0}, 0.666667}, {{2.0, 4.0}, 0.5},
    {{2.0, 5.0}, 0.4},  {{3.0, 1.0}, 3.0},      {{3.0, 2.0}, 1.5},
    {{3.0, 3.0}, 1.0},  {{3.0, 4.0}, 0.75},     {{3.0, 5.0}, 0.6},
    {{4.0, 1.0}, 4.0},  {{4.0, 2.0}, 2.0},      {{4.0, 3.0}, 1.333333},
    {{4.0, 4.0}, 1.0},  {{4.0, 5.0}, 0.8},      {{5.0, 1.0}, 5.0},
    {{5.0, 2.0}, 2.5},  {{5.0, 3.0}, 1.666667}, {{5.0, 4.0}, 1.25},
    {{5.0, 5.0}, 1.0}};  // Centipoise and Millipascal Seconds to mm^2/s

// Helper function to create a string with the differences in the calculated and
// the expected conversions. _Unit must fullfill the same requirements as
// ConvertToBaseUnit.
template <typename _Unit>
std::string ConversionHelper(TestCasesMap& cases, _Unit from) {
  static_assert(!std::is_same<ViscosityUnit, _Unit>::value,
                "Use ConvertViscoHelper instead");

  std::string errors = "";

  for (const auto& elem : cases) {
    DoubleT res = ConvertToBaseUnit<_Unit>(DoubleT(elem.first), from);

    // If the result is not what expected we create a string with the
    // expected and resulting conversion.
    if (std::abs((double)(res - DoubleT(elem.second))) > 0.01) {
      errors += std::to_string(elem.first) + " -> " +
                std::to_string((double)(res)) +
                " != " + std::to_string((double)(DoubleT(elem.second))) + '\n';
    }
  }

  return errors;
}

TEST(ConversionFunctions, liters_per_minute) {
  // collect all failing cases as string so that they will be provided for the
  // testcase
  std::string errors = ConversionHelper<FlowrateUnit>(
      lpmToCubicMPH, FlowrateUnit::kLitersPerMinute);

  EXPECT_STREQ(errors.c_str(), "");
}

TEST(ConversionFunctions, gallons_per_minute) {
  // collect all failing cases as string so that they will be provided for the
  // testcase
  std::string errors = ConversionHelper<FlowrateUnit>(
      gpmToCubicMPH, FlowrateUnit::kGallonsPerMinute);

  EXPECT_STREQ(errors.c_str(), "");
}

TEST(ConversionFunctions, feet_to_meters) {
  // collect all failing cases as string so that they will be provided for the
  // testcase
  std::string errors =
      ConversionHelper<HeadUnit>(feetToMeters, HeadUnit::kFeet);

  EXPECT_STREQ(errors.c_str(), "");
}

TEST(ConversionFunctions, kilogram_per_cubic_meter_to_gpl) {
  // collect all failing cases as string so that they will be provided for the
  // testcase
  std::string errors = ConversionHelper<DensityUnit>(
      kpcmToGPL, DensityUnit::kKilogramsPerCubicMeter);

  EXPECT_STREQ(errors.c_str(), "");
}

TEST(ConversionFunctions, viscosity_conversion) {
  // For this test to pass Density must be converted properly
  ASSERT_STREQ(ConversionHelper<DensityUnit>(
                   kpcmToGPL, DensityUnit::kKilogramsPerCubicMeter)
                   .c_str(),
               "");

  std::string errors = "";

  for (const auto& [test_c, expect] : cpmpasTomm2s) {
    DoubleT res = ConvertViscosityTomm2s(
        DoubleT(test_c.first), ViscosityUnit::kcP, DoubleT(test_c.second),
        DensityUnit::kGramPerLiter);

    // If the result is not what expected we create a string with the
    // expected and resulting conversion.
    if (std::abs((double)(res - DoubleT(expect))) > 0.001) {
#ifdef VCCORE_USE_ACCURACY_TYPE
      errors += static_cast<std::string>(test_c.first) + " -> " +
                std::to_string((double)(res)) +
                " != " + std::to_string((double)(DOUBLE_T(expect))) + '\n';
#else
      errors += std::to_string(test_c.first) + " -> " +
                std::to_string((double)(res)) +
                " != " + std::to_string((double)(DoubleT(expect))) + '\n';
#endif
    }
  }

  EXPECT_STREQ(errors.c_str(), "");
}

}  // namespace

}  // namespace vccore_testing
}  // namespace impl
}  // namespace vccore
}  // namespace spauly