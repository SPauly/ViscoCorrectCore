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

#include "spauly/vccore/input_types.h"
#include "spauly/vccore/impl/bundle_types.h"
#include "spauly/vccore/impl/conversion_functions.h"

namespace spauly {
namespace vccore {
// Forward declarations
class CalculationCTX;
namespace impl {
class Calculator;
}

class CalculationCTX {
 private:
  // The Calculator needs direct access to the constant values for faster
  // calculations.
  friend class impl::Calculator;

 public:
  CalculationCTX() = default;
  ~CalculationCTX() = default;

  /// Converts the given value from _Unit to the given internal representation.
  /// _Unit must be set to either FlowrateUnit, HeadUnit, ViscosityUnit or
  /// DensityUnit. Returns 0 on error.
  template <typename _Unit>
  const double ConvertValueToBase(
      const InputT& value, const _Unit from, const InputT& density = "",
      const DensityUnit d_unit = DensityUnit::kGramPerLiter) const noexcept;

 private:
  // The provided values here were calculated in the original code and must
  // be updated. Please refer to the original code for how the were
  // obtained: <https://github.com/SPauly/ViscoCorrect>

#ifndef VCCORE_USE_ACCURACY_TYPE
  const std::array<const impl::DoubleT, 6> kQ = {
      4.3286373442021278e-09, -6.5935466655309209e-06, 0.0039704102541411324,
      -1.1870337647376101,    176.52190832690891,      -10276.558815133236};
  const std::array<const impl::DoubleT, 6> kEta = {
      2.5116987378131985e-10, -3.2416532447274418e-07, 0.00015531747394399714,
      -0.037300324399145976,  4.2391803778160968,      -6.2364025573465849};
  const std::array<const std::array<const impl::DoubleT, 3>, 4> kH = {{
      {285.39113639063004, -0.019515612319848788, 451.79876054847699},  // 0.6
      {286.44331640461877, -0.016739174282778945, 453.11949555301783},  // 0.8
      {285.70823636118865, -0.016126836943018912, 443.60573501332937},  // 1.0
      {285.91175890816675, -0.015057232233799856, 436.03377039579027}   // 1.2
  }};
#else
  const std::array<const impl::DoubleT, 6> kQ = {
      impl::DoubleT("4.3286373442021278e-09"),
      impl::DoubleT("-6.5935466655309209e-06"),
      impl::DoubleT("0.0039704102541411324"),
      impl::DoubleT("-1.1870337647376101"),
      impl::DoubleT("176.52190832690891"),
      impl::DoubleT("-10276.558815133236")};
  const std::array<const impl::DoubleT, 6> kEta = {
      impl::DoubleT("2.5116987378131985e-10"),
      impl::DoubleT("-3.2416532447274418e-07"),
      impl::DoubleT("0.00015531747394399714"),
      impl::DoubleT("-0.037300324399145976"),
      impl::DoubleT("4.2391803778160968"),
      impl::DoubleT("-6.2364025573465849")};
  const std::array<const std::array<const impl::DoubleT, 3>, 4> kH = {{
      {impl::DoubleT("285.39113639063004"),
       impl::DoubleT("-0.019515612319848788"),
       impl::DoubleT("451.79876054847699")},  // 0.6
      {impl::DoubleT("286.44331640461877"),
       impl::DoubleT("-0.016739174282778945"),
       impl::DoubleT("453.11949555301783")},  // 0.8
      {impl::DoubleT("285.70823636118865"),
       impl::DoubleT("-0.016126836943018912"),
       impl::DoubleT("443.60573501332937")},  // 1.0
      {impl::DoubleT("285.91175890816675"),
       impl::DoubleT("-0.015057232233799856"),
       impl::DoubleT("436.03377039579027")}  // 1.2
  }};
#endif  // VCCORE_USE_ACCURACY_TYPE
};

// Template definitions
template <typename _Unit>
const double CalculationCTX::ConvertValueToBase(
    const InputT& value, const _Unit from, const InputT& density,
    const DensityUnit d_unit) const noexcept {
  static_assert(std::is_same<ViscosityUnit, _Unit>::value,
                "Invalid unit type. Must be either FlowrateUnit, HeadUnit, or "
                "DensityUnit, or ViscosityUnit");

  if (!std::is_same<ViscosityUnit, _Unit>::value)
    return (double)impl::ConvertToBaseUnit<_Unit>(value, from);
  else
    return (double)impl::ConvertViscosityTomm2s(value, from, density, d_unit);
}

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_CALCULATION_CTX_H_