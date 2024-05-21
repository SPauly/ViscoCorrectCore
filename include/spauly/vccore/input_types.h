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
#ifndef SPAULY_VCCORE_INPUT_TYPES_H_
#define SPAULY_VCCORE_INPUT_TYPES_H_

namespace spauly {
namespace vccore {

#ifndef VCCORE_USE_ACCURACY_TYPE
using InputT = double;
#else
// InputT is the type used for the input parameters. Since double may not be
// accurate enough for the calculations, std::string is used instead. Internally
// this will be converted to a more accurate representation.
using InputT = std::string;
#endif  // VCCORE_USE_ACCURACY_TYPE

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

static const FlowrateUnit kStandardFlowrateUnit =
    FlowrateUnit::kCubicMetersPerHour;
static const HeadUnit kStandardHeadUnit = HeadUnit::kMeters;
static const ViscosityUnit kStandardViscosityUnit =
    ViscosityUnit::kSquareMilPerSecond;
static const DensityUnit kStandardDensityUnit = DensityUnit::kGramPerLiter;
}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_INPUT_TYPES_H_