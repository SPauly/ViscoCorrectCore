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
#ifndef SPAULY_VCCORE_IMPL_CONVERSION_FUNCTIONS_H_
#define SPAULY_VCCORE_IMPL_CONVERSION_FUNCTIONS_H_

#include <map>

#include "spauly/vccore/input_parameters.h"

namespace spauly {
namespace vccore {
namespace impl {

/// Holds the factors by which to multiply the input parameters to convert them
/// to m³/h.
const std::map<FlowrateUnit, const float> kFlowrateToCubicMPH{
    {FlowrateUnit::kLitersPerMinute, 0.06f},
    {FlowrateUnit::kGallonsPerMinute, 0.2271f},
    {FlowrateUnit::kCubicMetersPerHour, 1.0f}};

/// Holds the factors by which to multiply the input parameters to convert them
/// to m.
const std::map<HeadUnit, const float> kHeadToMeters{{HeadUnit::kFeet, 0.3048f},
                                                    {HeadUnit::kMeters, 1.0f}};

/// Holds the factors by which to multiply the input parameters to convert them
/// to g/l.
const std::map<DensityUnit, const float> kDensityToGPL{
    {DensityUnit::kGramPerLiter, 1.0f},
    {DensityUnit::kKilogramsPerCubicMeter, 0.001f}};

template <typename _Unit>
P_type ConvertToBaseUnits(const P_type value, const _Unit from) {
  static_assert(std::is_same<FlowrateUnit, _Unit>::value ||
                    std::is_same<HeadUnit, _Unit>::value ||
                    std::is_same<ViscosityUnit, _Unit>::value ||
                    std::is_same<DensityUnit, _Unit>::value,
                "Invalid unit type. Must be either FlowrateUnit, HeadUnit, "
                "ViscosityUnit or DensityUnit.");

  if constexpr (std::is_same<FlowrateUnit, _Unit>::value)
    return value * kFlowrateToCubicMPH.at(from);
  else if constexpr (std::is_same<HeadUnit, _Unit>::value)
    return value * kHeadToMeters.at(from);
  else if constexpr (std::is_same<DensityUnit, _Unit>::value)
    return value * kDensityToKgM3.at(from);
  else
    return value;
}

}  // namespace vccore
}  // namespace spauly

}  // namespace spauly

}  // namespace impl

}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_CONVERSION_FUNCTIONS_H_