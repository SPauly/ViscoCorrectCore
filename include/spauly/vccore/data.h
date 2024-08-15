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
#ifndef SPAULY_VCCORE_DATA_H_
#define SPAULY_VCCORE_DATA_H_

#include <array>
#include <string>

namespace spauly {
namespace vccore {

// forward declarations
struct Parameters;
struct Units;
struct CorrectionFactors;

// Define the floatingpoint type used for all calculations.
using DoubleT = double;

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

/// @brief Parameters is a DTO used for the communicatio between the user and
/// the Calculator.
struct Parameters {
  DoubleT flowrate;
  DoubleT total_head;
  DoubleT viscosity;
  DoubleT density;

  Parameters() = default;
  Parameters(DoubleT flowrate, DoubleT total_head, DoubleT viscosity,
             DoubleT density)
      : flowrate(flowrate),
        total_head(total_head),
        viscosity(viscosity),
        density(density) {}
};

/// @brief Units is a DTO used for the communicatio between the user and the
/// Calculator. It should only be used incase units other than the standard
/// units where used in the Parameters DTO.
struct Units {
  FlowrateUnit flowrate = kStandardFlowrateUnit;
  HeadUnit total_head = kStandardHeadUnit;
  ViscosityUnit viscosity = kStandardViscosityUnit;
  DensityUnit density = kStandardDensityUnit;

  Units() = default;
  Units(FlowrateUnit flowrate, HeadUnit total_head = kStandardHeadUnit,
        ViscosityUnit viscosity = kStandardViscosityUnit,
        DensityUnit density = kStandardDensityUnit)
      : flowrate(flowrate),
        total_head(total_head),
        viscosity(viscosity),
        density(density) {}
};

static const Units kStandardUnits = Units();

/// @brief CorrectionFactors is a DTO used for the communication between the
/// Calculator and the user. It contains the correction factors based on a
/// Parameters and Unit set.
struct CorrectionFactors {
  double q = 0;
  double eta = 0;
  std::array<double, 4> h;

  int error_flag = 0;
  std::string error_msg = "";
};

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_DATA_H_