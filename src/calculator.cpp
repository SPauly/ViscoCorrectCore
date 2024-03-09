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
#include "spauly/vccore/calculator.h"

namespace spauly {
namespace vccore {
const CorrectionFactors Calculator::Calculate(
    const InputParameters input) const noexcept {
  return std::move(CalcImpl(input));
}

const CorrectionFactors Calculator::Calculate(
    P_type _flowrate, P_type _head, P_type _viscosity,
    FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
    HeadUnit _h_unit = HeadUnit::kMeters,
    ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond) const noexcept {
  // Work with an InputParameters object internaly.
  InputParameters input{
      std::move(_flowrate), std::move(_head),   std::move(_viscosity), 0,
      std::move(_f_unit),   std::move(_h_unit), std::move(_v_unit)};

  return std::move(CalcImpl(input));
}

const CorrectionFactors Calculator::Calculate(
    P_type _flowrate, P_type _head, P_type _viscosity,
    DensityInputType _density = 0,
    FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
    HeadUnit _h_unit = HeadUnit::kMeters,
    ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
    DensityUnit _d_unit = DensityUnit::kGramPerLiter) const noexcept {
  // Work with an InputParameters object internaly.
  InputParameters input{std::move(_flowrate),  std::move(_head),
                        std::move(_viscosity), std::move(_density),
                        std::move(_f_unit),    std::move(_h_unit),
                        std::move(_v_unit),    std::move(_d_unit)};
  return std::move(CalcImpl(input));
}

// Implementations
const InputParameters Calculator::ConvertInput(
    const InputParameters& input) const noexcept {
  InputParameters out;

  // Set all converted values, the units can be left standard initialized.
  out.flowrate_q = impl::ConvertToBaseUnit<FlowrateUnit>(input.flowrate_q,
                                                         input.flowrate_unit);
  out.total_head =
      impl::ConvertToBaseUnit<HeadUnit>(input.total_head, input.head_unit);
  out.viscosity_v =
      impl::ConvertViscosityTomm2s(input.viscosity_v, input.viscosity_unit,
                                   input.density_cp, input.density_unit);
  out.density_cp = impl::ConvertToBaseUnit<DensityUnit>(input.density_cp,
                                                        input.density_unit);

  return std::move(out);
}

}  // namespace vccore

}  // namespace spauly
