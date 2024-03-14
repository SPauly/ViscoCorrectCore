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

#include "spauly/vccore/input_parameters.h"

#include <gtest/gtest.h>

namespace spauly {
namespace vccore {
namespace impl {
namespace vccore_testing {
namespace {
// First param represents the test value the second one the expected conversion.
using TestCasesMap = std::map<P_type, P_type>;

// Test utilities for Flowrate:
TestCasesMap lpmToCubicMPH{
    {1, 0.06f},       {10, 0.6f},         {100, 6},        {1000, 60},
    {10000, 600},     {0.5f, 0.03f},      {5.5f, 0.33f},   {11.1f, 0.666f},
    {111.1f, 6.666f}, {1111.1f, 66.666f}, {0.11f, 0.0066f}};  // Liters per
                                                              // Minute to m^3/h
TestCasesMap gpmToCubicMPH{
    {1, 0.2271f},        {10, 2.271f},      {100, 22.71f},
    {1000, 227.1f},      {10000, 2271},     {0.5f, 0.11355f},
    {5.5f, 1.24905f},    {11.1f, 2.52281f}, {111.1f, 25.2711f},
    {1111.1f, 252.281f}, {0.11f, 0.02498f}};  // Gallons per
                                              // Minute to m^3/h

// Test utilities for Head
TestCasesMap feetToMeters{
    {1, 0.3048f},  {10, 3.048f},    {100, 30.48f},     {1000, 304.8f},
    {10000, 3048}, {5.5f, 1.6764f}, {11.1f, 3.38328f},
};  // Feet to Meters

// Test utilities for Density
TestCasesMap
    kpcmToGPL{{1, 0.001f},       {10, 0.01f},      {100, 0.1f},
              {1000, 1},         {10000, 10},      {0.5f, 0.0005f},
              {5.5f, 0.0055f},   {11.1f, 0.0111f}, {111.1f, 0.111f},
              {1111.1f, 1.111f}, {0.11f, 0.00011f}};  // KilogramsPerCubicMeter
                                                      // To GramPerLiter

// Test utilities for Viscosity
// The pair holds the following args: 1. Viscosity in cp/mpas, 2. Density in gPl
std::map<std::pair<P_type, P_type>, P_type> cpmpasTomm2s{
    {{1, 1}, 1},    {{1, 2}, 0.5f},      {{1, 3}, 0.333333f},
    {{1, 4}, 0.25}, {{1, 5}, 0.2f},      {{2, 1}, 2},
    {{2, 2}, 1},    {{2, 3}, 0.666667f}, {{2, 4}, 0.5f},
    {{2, 5}, 0.4f}, {{3, 1}, 3},         {{3, 2}, 1.5f},
    {{3, 3}, 1},    {{3, 4}, 0.75f},     {{3, 5}, 0.6f},
    {{4, 1}, 4},    {{4, 2}, 2},         {{4, 3}, 1.333333f},
    {{4, 4}, 1},    {{4, 5}, 0.8f},      {{5, 1}, 5},
    {{5, 2}, 2.5f}, {{5, 3}, 1.666667f}, {{5, 4}, 1.25f},
    {{5, 5}, 1}};  // Centipoise and Millipascal Seconds to mm^2/s

// Helper function to create a string with the differences in the calculated and
// the expected conversions. _Unit must fullfill the same requirements as
// ConvertToBaseUnit.
template <typename _Unit>
std::string ConversionHelper(TestCasesMap& cases, _Unit from) {
  static_assert(!std::is_same<ViscosityUnit, _Unit>::value,
                "Use ConvertViscoHelper instead");

  std::string errors = "";

  for (const auto& elem : cases) {
    P_type res = ConvertToBaseUnit<_Unit>(elem.first, from);

    // If the result is not what expected we create a string with the
    // expected and resulting conversion.
    if (res != elem.second) {
      errors += std::to_string(elem.first) + " -> " + std::to_string(res) +
                " != " + std::to_string(elem.second) + '\n';
    }
  }

  return std::move(errors);
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
      lpmToCubicMPH, FlowrateUnit::kGallonsPerMinute);

  EXPECT_STREQ(errors.c_str(), "");
}

TEST(ConversionFunctions, feet_to_meters) {
  // collect all failing cases as string so that they will be provided for the
  // testcase
  std::string errors =
      ConversionHelper<HeadUnit>(lpmToCubicMPH, HeadUnit::kFeet);

  EXPECT_STREQ(errors.c_str(), "");
}

TEST(ConversionFunctions, kilogram_per_cubic_meter_to_gpl) {
  // collect all failing cases as string so that they will be provided for the
  // testcase
  std::string errors = ConversionHelper<DensityUnit>(
      lpmToCubicMPH, DensityUnit::kKilogramsPerCubicMeter);

  EXPECT_STREQ(errors.c_str(), "");
}

TEST(ConversionFunctions, viscosity_conversion) {
  // For this test to pass Density must be converted properly
  ASSERT_STREQ(ConversionHelper<DensityUnit>(
                   lpmToCubicMPH, DensityUnit::kKilogramsPerCubicMeter)
                   .c_str(),
               "");

  std::string errors = "";

  for (const auto& elem : cpmpasTomm2s) {
    P_type res =
        ConvertViscosityTomm2s(elem.first.first, ViscosityUnit::kcP,
                               elem.first.second, DensityUnit::kGramPerLiter);

    // If the result is not what expected we create a string with the
    // expected and resulting conversion.
    if (res != elem.second) {
      errors += std::to_string(elem.first.first) + " -> " +
                std::to_string(res) + " != " + std::to_string(elem.second) +
                '\n';
    }
  }

  EXPECT_STREQ(errors.c_str(), "");
}

}  // namespace

}  // namespace vccore_testing
}  // namespace impl
}  // namespace vccore
}  // namespace spauly