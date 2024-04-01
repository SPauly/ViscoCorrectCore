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
#include "spauly/vccore/calculation_ctx.h"

namespace spauly {
namespace vccore {
const CorrectionFactors CalculationCTX::Calculate(
    const InputParameters input) const noexcept {
  return std::move(CalcImpl(input));
}

const CorrectionFactors CalculationCTX::Calculate(
    const std::string& _flowrate, const std::string& _head,
    const std::string& _viscosity, FlowrateUnit _f_unit, HeadUnit _h_unit,
    ViscosityUnit _v_unit) const noexcept {
  // Work with an InputParameters object internaly.
  InputParameters input{
      std::move(_flowrate), std::move(_head),   std::move(_viscosity), 0,
      std::move(_f_unit),   std::move(_h_unit), std::move(_v_unit)};

  return std::move(CalcImpl(input));
}

const CorrectionFactors CalculationCTX::Calculate(
    const std::string& _flowrate, const std::string& _head,
    const std::string& _viscosity, const std::string& _density,
    FlowrateUnit _f_unit, HeadUnit _h_unit, ViscosityUnit _v_unit,
    DensityUnit _d_unit) const noexcept {
  // Work with an InputParameters object internaly.
  InputParameters input{std::move(_flowrate),  std::move(_head),
                        std::move(_viscosity), std::move(_density),
                        std::move(_f_unit),    std::move(_h_unit),
                        std::move(_v_unit),    std::move(_d_unit)};
  return std::move(CalcImpl(input));
}

// Implementations
const InputParameters CalculationCTX::ConvertInput(
    const InputParameters& input) const noexcept {
  // This constructor converts the given input to the internally used
  // representation.
  impl::InternalPType i_input(input);

  // Set all converted values, the units can be left standard initialized.
  i_input.flowrate_q = impl::ConvertToBaseUnit<FlowrateUnit>(
      i_input.flowrate_q, i_input.flowrate_unit);
  i_input.total_head =
      impl::ConvertToBaseUnit<HeadUnit>(i_input.total_head, i_input.head_unit);
  i_input.viscosity_v =
      impl::ConvertViscosityTomm2s(i_input.viscosity_v, i_input.viscosity_unit,
                                   i_input.density_cp, i_input.density_unit);
  i_input.density_cp = impl::ConvertToBaseUnit<DensityUnit>(
      i_input.density_cp, i_input.density_unit);

  InputParameters out;
  out.flowrate_q = static_cast<std::string>(i_input.flowrate_q);
  out.total_head = static_cast<std::string>(i_input.total_head);
  out.viscosity_v = static_cast<std::string>(i_input.viscosity_v);
  out.density_cp = static_cast<std::string>(i_input.density_cp);
  return std::move(out);
}

const CorrectionFactors CalculationCTX::CalcImpl(
    const InputParameters& input) const noexcept {
  // Convert the given Inputs to the internally used units
  // Verify that the input values are in the provided range by the standard
  // Create Linear Functions for Viscosity and TotalHead
  // Fit Flowrate Input to Flowrate scale
  // Determine resulting X coordinate
  // Calculate CorrectionFactors based on that
  return CorrectionFactors(1);
}

}  // namespace vccore

}  // namespace spauly
