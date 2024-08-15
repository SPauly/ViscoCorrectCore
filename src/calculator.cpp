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

Parameters Calculator::GetConverted(const Parameters& p,
                                    const Units& u) const noexcept {
  Parameters out;

  // Set all converted values, the units can be left standard initialized.
  out.flowrate = impl::ConvertToBaseUnit<FlowrateUnit>(
      impl::DoubleT(p.flowrate), u.flowrate);
  out.total_head = impl::ConvertToBaseUnit<HeadUnit>(
      impl::DoubleT(p.total_head), u.total_head);
  out.viscosity =
      impl::ConvertViscosityTomm2s(impl::DoubleT(p.viscosity), u.viscosity,
                                   impl::DoubleT(p.density), u.density);
  out.density =
      impl::ConvertToBaseUnit<DensityUnit>(impl::DoubleT(p.density), u.density);

  return std::move(out);
}

}  // namespace vccore
}  // namespace spauly