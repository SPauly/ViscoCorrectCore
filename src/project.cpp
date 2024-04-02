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
#include "spauly/vccore/project.h"

namespace spauly {
namespace vccore {
Project::Project(PType _flowrate, PType _head, PType _viscosity,
                 DensityInputType _density, FlowrateUnit _f_unit,
                 HeadUnit _h_unit, ViscosityUnit _v_unit, DensityUnit _d_unit)
    : input_flowrate_(_flowrate),
      input_total_head_(_head),
      input_viscosity_(_viscosity),
      input_density_cp_(_density),
      flowrate_unit_(_f_unit),
      head_unit_(_h_unit),
      viscosity_unit_(_v_unit),
      density_unit_(_d_unit) {}

Project::Project(const Project& other) {
  std::unique_lock<std::shared_mutex> lock(other.mtx_);

  input_flowrate_ = other.input_flowrate_;
  input_total_head_ = other.input_total_head_;
  input_viscosity_ = other.input_viscosity_;
  input_density_cp_ = other.input_density_cp_;
  flowrate_unit_ = other.flowrate_unit_;
  head_unit_ = other.head_unit_;
  viscosity_unit_ = other.viscosity_unit_;
  density_unit_ = other.density_unit_;
}

Project& Project::operator=(const Project& other) {
  std::unique_lock<std::shared_mutex> lock(other.mtx_);
  std::unique_lock<std::shared_mutex> lock2(mtx_);

  input_flowrate_ = other.input_flowrate_;
  input_total_head_ = other.input_total_head_;
  input_viscosity_ = other.input_viscosity_;
  input_density_cp_ = other.input_density_cp_;
  flowrate_unit_ = other.flowrate_unit_;
  head_unit_ = other.head_unit_;
  viscosity_unit_ = other.viscosity_unit_;
  density_unit_ = other.density_unit_;

  return *this;
}

}  // namespace vccore
}  // namespace spauly