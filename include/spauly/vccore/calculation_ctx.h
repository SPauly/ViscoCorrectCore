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
#ifndef SPAULY_VCCORE_CALCULATION_CTX_H_
#define SPAULY_VCCORE_CALCULATION_CTX_H_

#include <array>
#include <string>
#include <memory>

#include "spauly/vccore/input_parameters.h"
#include "spauly/vccore/project.h"
#include "spauly/vccore/impl/conversion_functions.h"

namespace spauly {
namespace vccore {

class CalculationCTX {
 public:
  CalculationCTX() = default;
  ~CalculationCTX() = default;

  /// Calculates the correction factors for the given input parameters. The
  /// parameters are copied to ensure threadsafety. The error flag in
  /// CorrectionFactors is set when the input parameters are invalid.
  const CorrectionFactors Calculate(
      const ParametersInternal input) const noexcept;

  /// Calculates the correction factors for the given input parameters. The
  /// parameters are copied to ensure threadsafety. The error flag in
  /// CorrectionFactors is set when the input parameters are invalid.
  const CorrectionFactors Calculate(
      const std::string& _flowrate, const std::string& _head,
      const std::string& _viscosity,
      FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
      HeadUnit _h_unit = HeadUnit::kMeters,
      ViscosityUnit _v_unit =
          ViscosityUnit::kSquareMilPerSecond) const noexcept;

  /// Calculates the correction factors for the given input parameters. The
  /// parameters are copied to ensure threadsafety. The error flag in
  /// CorrectionFactors is set when the input parameters are invalid.
  const CorrectionFactors Calculate(
      const std::string& _flowrate, const std::string& _head,
      const std::string& _viscosity, const std::string& _density = "",
      FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
      HeadUnit _h_unit = HeadUnit::kMeters,
      ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
      DensityUnit _d_unit = DensityUnit::kGramPerLiter) const noexcept;

  /// Converts the given value from _Unit to the given internal representation.
  /// _Unit must be set to either FlowrateUnit, HeadUnit, ViscosityUnit or
  /// DensityUnit. Returns 0 on error.
  template <typename _Unit>
  const double ConvertValueToBase(
      const std::string value, const _Unit from,
      const std::string& density = "",
      const DensityUnit d_unit = DensityUnit::kGramPerLiter) const noexcept;

  /// Converts the given input parameters to the internal representation. This
  /// can be used to validate on which bases the correction factors were
  /// calculated.
  const ParametersInternal ConvertInput(
      const ParametersInternal& input) const noexcept;

 private:
  const CorrectionFactors CalcImpl(
      const ParametersInternal& input) const noexcept;

 private:
  // The provided values here were calculated in the original code and must
  // be updated. Please refer to the original code for how the were
  // obtained: <https://github.com/SPauly/ViscoCorrect>
  const std::array<const impl::IType, 6> kQ = {
      impl::IType("4.3286373442021278e-09"),
      impl::IType("-6.5935466655309209e-06"),
      impl::IType("0.0039704102541411324"),
      impl::IType("-1.1870337647376101"),
      impl::IType("176.52190832690891"),
      impl::IType("-10276.558815133236")};
  const std::array<const impl::IType, 6> kEta = {
      impl::IType("2.5116987378131985e-10"),
      impl::IType("-3.2416532447274418e-07"),
      impl::IType("0.00015531747394399714"),
      impl::IType("-0.037300324399145976"),
      impl::IType("4.2391803778160968"),
      impl::IType("-6.2364025573465849")};
  const std::array<const std::array<const impl::IType, 3>, 4> kH = {{
      {impl::IType("285.39113639063004"), impl::IType("-0.019515612319848788"),
       impl::IType("451.79876054847699")},  // 0.6
      {impl::IType("286.44331640461877"), impl::IType("-0.016739174282778945"),
       impl::IType("453.11949555301783")},  // 0.8
      {impl::IType("285.70823636118865"), impl::IType("-0.016126836943018912"),
       impl::IType("443.60573501332937")},  // 1.0
      {impl::IType("285.91175890816675"), impl::IType("-0.015057232233799856"),
       impl::IType("436.03377039579027")}  // 1.2
  }};
};

// Template definitions
template <typename _Unit>
const double CalculationCTX::ConvertValueToBase(
    const std::string value, const _Unit from, const DensityInputType& density,
    const DensityUnit d_unit) const noexcept {
  static_assert(std::is_same<ViscosityUnit, _Unit>::value,
                "Invalid unit type. Must be either FlowrateUnit, HeadUnit, or "
                "DensityUnit, or ViscosityUnit");

  if (!std::is_same<ViscosityUnit, _Unit>::value)
    return impl::ConvertToBaseUnit<_Unit>(value, from).get_double();
  else
    return impl::ConvertViscosityTomm2s(value, from, density, d_unit)
        .get_double();
}

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_CALCULATION_CTX_H_