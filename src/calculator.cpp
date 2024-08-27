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

CorrectionFactors Calculator::Calculate(const Parameters& p,
                                        const Units& u) const noexcept {
  CorrectionFactors out;
  Parameters p_base = p;

  // Check if we need to convert the input values to the base units.
  if (u != kStandardUnits) p_base = GetConverted(p, u);

  // Validate the Input
  out.error_flag = ValidateInput(p_base);
  if (!(out.error_flag & kNoError)) return std::move(out);

  // Map the input values to the scales.
  double flow_pos = FitToScale(kFlowrateScale, p_base.flowrate, 0);
  double head_pos =
      FitToScale(kTotalHeadScale, p_base.total_head,
                 kStartTotalH.at(1));  // head_pos is on the y-axis so we take
                                       // this is start coordinate.
  double visc_pos =
      FitToScale(kViscoScale, p_base.viscosity,
                 kStartVisco.at(0));  // visc_pos is on the x-axis so we take
                                      // this is start coordinate.

  // Create linear functions for totalhead and viscosity.
  impl::LinearFunc<DoubleT> head_func(
      kPitchTotalH, static_cast<DoubleT>(kStartTotalH.at(0)), head_pos);
  impl::LinearFunc<DoubleT> visc_func(kPitchVisco, visc_pos,
                                      static_cast<DoubleT>(kStartVisco.at(1)));

  // Calculate the correction x position.
  DoubleT pos_main = visc_func.SolveForX(
      head_func(flow_pos));  // Get the position of the intersection point of
                             // the two lines.

  // Calculate the correction factors.
  if (ValidateXQ(pos_main)) {
    out.q = (kFuncQ(pos_main) / kPixelsCorrectionScale / 10.0) + 0.2;
  } else {
    out.q = (pos_main < 242) ? 1.0 : 0.0;  // 242 is the lower cutoff value.
  }

  if (ValidateXEta(pos_main)) {
    out.eta = (kFuncEta(pos_main) / kPixelsCorrectionScale / 10.0) + 0.2;
  } else {
    out.eta = (pos_main < 122) ? 1.0 : 0.0;  // 122 is the lower cutoff value.
  }

  if (ValidateXH(pos_main)) {
    for (int i = 0; i < kFuncH.size(); i++) {
      out.h.at(i) =
          (kFuncH.at(i)(pos_main) / kPixelsCorrectionScale / 10) - 0.3;
    }
  } else {
    for (int i = 0; i < kFuncH.size(); i++) {
      out.h.at(i) = (pos_main < 146.0) ? 1.0 : 0.0;
    }
  }

  return std::move(out);
}

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

const size_t Calculator::ValidateInput(const Parameters& p) const noexcept {
  size_t errors = 0;

  if (p.flowrate < 6 || p.flowrate > 2000) {
    errors |= ErrorFlag::kFlowrateError;
  }

  if (p.total_head < 5 || p.total_head > 200) {
    errors |= ErrorFlag::kTotalHeadError;
  }

  if (p.viscosity < 10 || p.viscosity > 4000) {
    errors |= ErrorFlag::kViscosityError;
  }

  return std::move(errors);
}

const double Calculator::FitToScale(
    const std::map<const int, const int>& raw_scale, const double& input,
    const int start_pos) const noexcept {
  double absolute_position = static_cast<double>(start_pos);
  double prev_value = 0;
  double curr_value = 0;
  bool bfound = false;

  for (const auto& [key, distpixels] : raw_scale) {
    curr_value = static_cast<double>(key);

    if (curr_value == input) {
      absolute_position += static_cast<double>(distpixels);
      bfound = true;
      break;
    } else if (curr_value > input) {
      double range = curr_value - prev_value;
      double relative_value = input - prev_value;

      absolute_position +=
          (relative_value / range) * static_cast<double>(distpixels);
      bfound = true;
      break;
    }

    absolute_position += static_cast<double>(distpixels);
    prev_value = curr_value;
  }

  if (bfound)
    return absolute_position;
  else
    return -1.0f;
}

}  // namespace vccore
}  // namespace spauly