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
#ifndef SPAULY_VCCORE_CALCULATOR_H_
#define SPAULY_VCCORE_CALCULATOR_H_

#include <array>
#include <map>
#include <memory>

#include "spauly/vccore/data.h"
#include "spauly/vccore/impl/conversion_functions.h"
#include "spauly/vccore/impl/math.h"

namespace spauly {
namespace vccore {

class Calculator {
 public:
  Calculator() = default;
  ~Calculator() = default;

  /// @brief Calculates the correction factors for the given Parameters and
  /// Units.
  /// @param p Flowrate, Head, Viscosity, and Density of the fluid as
  /// Parameters.
  /// @param u Set to standard units m³/h, m, mm²/s, and g/l if not provided.
  /// @return CorrectionFactors for the given Parameters.
  CorrectionFactors Calculate(const Parameters& p,
                              const Units& u = kStandardUnits) const noexcept;

  /// @brief Converts the given value to the base unit.
  /// @tparam _Unit Must be either FlowrateUnit, HeadUnit, DensityUnit, or
  /// ViscosityUnit.
  /// @param value value to be converted to the base unit.
  /// @param from Unit of the given value. Must be of type _Unit.
  /// @param density Density of the fluid if needed for Unit.
  /// @param d_unit Unit of the density.
  /// @return Converted value in the base unit according to the given unit.
  template <typename _Unit>
  const double ConvertValueToBase(
      const DoubleT value, const _Unit from, const DoubleT density = 0,
      const DensityUnit d_unit = DensityUnit::kGramPerLiter) const noexcept;

  /// @brief Converts the given Parameters to the base units. Usefull to get the
  /// internal values.
  /// @param p Parameters to be converted.
  /// @param u Units of the Parameters.
  /// @return Converted Parameters in the base units.
  Parameters GetConverted(const Parameters& p, const Units& u) const noexcept;

 protected:
  /// @brief Validates the given Parameters.
  /// @param p Parameters to be validated must be in the base units.
  /// @return ErrorFlags if an error was found.
  const size_t ValidateInput(const Parameters& p) const noexcept;

  /// @brief Maps the input value to the given scale, relative to the given
  /// values for the scale.
  /// @param raw_scale_units map where the key is the value of the position and
  /// the value is the distance in pixels to the next position.
  /// @param input value input by the user.
  /// @param startpos position to start the mapping.
  /// @return Returns the input value mapped to the scale in pixels.
  const double FitToScale(const std::map<const int, const int>& raw_scale_units,
                          const double& input,
                          const int startpos = 0) const noexcept;

 private:
  //------------------------------------------------
  // Constants for the correction factors calculation

  // The provided values here were calculated in the original code and must
  // be updated. Please refer to the original code for how the were
  // obtained: <https://github.com/SPauly/ViscoCorrect>
  const std::array<const DoubleT, 6> kQ{
      4.3286373442021278e-09, -6.5935466655309209e-06, 0.0039704102541411324,
      -1.1870337647376101,    176.52190832690891,      -10276.558815133236};
  const std::array<const DoubleT, 6> kEta{
      2.5116987378131985e-10, -3.2416532447274418e-07, 0.00015531747394399714,
      -0.037300324399145976,  4.2391803778160968,      -6.2364025573465849};
  const std::array<const std::array<const DoubleT, 3>, 4> kH{{
      {285.39113639063004, -0.019515612319848788, 451.79876054847699},  // 0.6
      {286.44331640461877, -0.016739174282778945, 453.11949555301783},  // 0.8
      {285.70823636118865, -0.016126836943018912, 443.60573501332937},  // 1.0
      {285.91175890816675, -0.015057232233799856, 436.03377039579027}   // 1.2
  }};

  const std::array<const int, 2> kStartFlowrate{0, 0};
  const std::array<const int, 2> kStartTotalH{4, 1};
  const std::array<const int, 2> kStartVisco{105, 304};
  const double kPitchTotalH = 0.5255813953488372;
  const double kPitchVisco = -1.9090909090909092;

  const std::map<const int, const int> kFlowrateScale{
      {6, 0},    {7, 14},    {8, 9},    {9, 9},    {10, 9},   {15, 30},
      {20, 21},  {30, 30},   {40, 21},  {50, 17},  {60, 13},  {70, 12},
      {80, 9},   {90, 9},    {100, 9},  {150, 30}, {200, 21}, {300, 30},
      {400, 21}, {500, 17},  {600, 14}, {700, 11}, {800, 10}, {900, 8},
      {1000, 8}, {1500, 30}, {2000, 22}};

  const std::map<const int, const int> kTotalHeadScale{
      {5, 0}, {10, 15}, {20, 12}, {40, 14}, {50, 8}, {100, 9}, {200, 13}};

  std::map<const int, const int> kViscoScale{
      {10, 0},   {20, 27},  {30, 16},   {40, 10},   {60, 15},  {80, 11},
      {100, 8},  {200, 26}, {300, 16},  {400, 11},  {500, 8},  {600, 6},
      {800, 12}, {1000, 9}, {2000, 26}, {3000, 14}, {4000, 10}};
};

// Template definitions
template <typename _Unit>
const double Calculator::ConvertValueToBase(
    const DoubleT value, const _Unit from, const DoubleT density,
    const DensityUnit d_unit) const noexcept {
  // Correct static_assert to allow only the valid unit types
  static_assert((std::is_same<FlowrateUnit, _Unit>::value ||
                 std::is_same<_Unit, HeadUnit>::value ||
                 std::is_same<_Unit, DensityUnit>::value ||
                 std::is_same<_Unit, ViscosityUnit>::value),
                "Invalid unit type. Must be either FlowrateUnit, HeadUnit, "
                "DensityUnit, or ViscosityUnit");

  // Handle the specific case for ViscosityUnit separately
  if (std::is_same<ViscosityUnit, _Unit>::value) {
    return static_cast<double>(impl::ConvertViscosityTomm2s(
        value, static_cast<ViscosityUnit>(from), density, d_unit));
  } else {
    return static_cast<double>(impl::ConvertToBaseUnit<_Unit>(value, from));
  }
}

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_CALCULATOR_H_