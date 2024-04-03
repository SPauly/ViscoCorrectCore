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

const Project CalculationCTX::ConvertInput(
    const Project& input) const noexcept {
  Project out = input;

  // Set all converted values, the units can be left standard initialized.
  out.set_flowrate(impl::ConvertToBaseUnit<FlowrateUnit>(
      impl::AccType(out.flowrate()), out.flowrate_unit()));
  out.set_total_head(impl::ConvertToBaseUnit<HeadUnit>(
      impl::AccType(out.total_head()), out.total_head_unit()));
  out.set_viscosity_v(impl::ConvertViscosityTomm2s(
      impl::AccType(out.viscosity_v()), out.viscosity_v_unit(),
      out.density_cp(), out.density_cp_unit()));
  out.set_density_cp(impl::ConvertToBaseUnit<DensityUnit>(
      impl::AccType(out.density_cp()), out.density_cp_unit()));

  out.set_flowrate_unit(StandardFlowrateUnit);
  out.set_head_unit(StandardHeadUnit);
  out.set_viscosity_unit(StandardViscosityUnit);
  out.set_density_unit(StandardDensityUnit);

  return out;
}
}  // namespace vccore

}  // namespace spauly
