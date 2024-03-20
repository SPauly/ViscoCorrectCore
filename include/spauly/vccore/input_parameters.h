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
#ifndef SPAULY_VCCORE_INPUT_PARAMETERS_H_
#define SPAULY_VCCORE_INPUT_PARAMETERS_H_

#include <string>
#include "spauly/vccore/impl/accuracy_type.h"

namespace spauly {
namespace vccore {

// PType is the type used for the input parameters. Since double may not be
// accurate enough for the calculations, std::string is used instead. Internally
// this will be converted to a more accurate representation.
using PType = std::string;
// DensityInputType is only used when the viscosity is given in centipoise.
using DensityInputType = std::string;

/// FlowrateUnit determines the unit of the flowrate. Available units are: m³/h,
/// l/min, gpm.
enum class FlowrateUnit : int {
  kCubicMetersPerHour,
  kLitersPerMinute,
  kGallonsPerMinute
};

/// HeadUnit determines the unit of the total head. Available units are: m, ft.
enum class HeadUnit : int { kMeters, kFeet };

/// ViscosityUnit determines the unit of the viscosity. Available units are:
/// mm²/s, cSt, cP, mPa·s.
enum class ViscosityUnit : int {
  kSquareMilPerSecond,  // mm²/s
  kcSt,                 // Centistokes
  kcP,                  // Centipoise
  kmPas                 // Millipascal seconds
};

/// DensityUnit determines the unit of the density. Available units are: g/l,
/// kg/m³.
enum class DensityUnit : int { kGramPerLiter, kKilogramsPerCubicMeter };

/// InputParameters contains all input parameters for the calculation. It is a
/// DTO used for the communication with the Calculator.
struct InputParameters {
  PType flowrate_q = "";
  PType total_head = "";
  PType viscosity_v = "";
  /// Must be provided when the viscosity is given in centipoise.
  DensityInputType density_cp = "";

  FlowrateUnit flowrate_unit = FlowrateUnit::kCubicMetersPerHour;
  HeadUnit head_unit = HeadUnit::kMeters;
  ViscosityUnit viscosity_unit = ViscosityUnit::kSquareMilPerSecond;
  DensityUnit density_unit = DensityUnit::kGramPerLiter;

  InputParameters() = default;
  InputParameters(PType _flowrate, PType _head, PType _viscosity,
                  DensityInputType _density = 0,
                  FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
                  HeadUnit _h_unit = HeadUnit::kMeters,
                  ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
                  DensityUnit _d_unit = DensityUnit::kGramPerLiter)
      : flowrate_q(_flowrate),
        total_head(_head),
        viscosity_v(_viscosity),
        density_cp(_density),
        flowrate_unit(_f_unit),
        head_unit(_h_unit),
        viscosity_unit(_v_unit),
        density_unit(_d_unit) {}
  ~InputParameters() = default;

  // Sets the input parameters. Behaves like the constructor.
  void set(PType _flowrate, PType _head, PType _viscosity,
           DensityInputType _density = 0,
           FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
           HeadUnit _h_unit = HeadUnit::kMeters,
           ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
           DensityUnit _d_unit = DensityUnit::kGramPerLiter) {
    flowrate_q = _flowrate;
    total_head = _head;
    viscosity_v = _viscosity;
    density_cp = _density;
    flowrate_unit = _f_unit;
    head_unit = _h_unit;
    viscosity_unit = _v_unit;
    density_unit = _d_unit;
  }
};

namespace impl {

/// InternalPType is the internal representation of the input parameters. It
/// represents the data in the specified IType.
struct InternalPType {
  IType flowrate_q;
  IType total_head;
  IType viscosity_v;
  IType density_cp;

  FlowrateUnit flowrate_unit = FlowrateUnit::kCubicMetersPerHour;
  HeadUnit head_unit = HeadUnit::kMeters;
  ViscosityUnit viscosity_unit = ViscosityUnit::kSquareMilPerSecond;
  DensityUnit density_unit = DensityUnit::kGramPerLiter;

  InternalPType() = default;
  InternalPType(const InputParameters& input)
      : flowrate_q(IType(input.flowrate_q)),
        total_head(IType(input.total_head)),
        viscosity_v(IType(input.viscosity_v)),
        density_cp(IType(input.density_cp)),
        flowrate_unit(input.flowrate_unit),
        head_unit(input.head_unit),
        viscosity_unit(input.viscosity_unit),
        density_unit(input.density_unit) {}
};

}  // namespace impl

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_INPUT_PARAMETERS_H_