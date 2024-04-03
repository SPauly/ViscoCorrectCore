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
#ifndef SPAULY_VCCORE_IMPL_BUNDLE_TYPES_H_
#define SPAULY_VCCORE_IMPL_BUNDLE_TYPES_H_

#include <array>
#include <string>
#include "spauly/vccore/impl/accuracy_type.h"

namespace spauly {
namespace vccore {
namespace impl {

/// ParametersInternal is a DTO used for the communication between Project and
/// Calculator. This should only contain the input parameters already converted
/// to the needed units.
struct ParametersInternal {
  AccType flowrate_q;
  AccType total_head;
  AccType viscosity_v;
  AccType density_cp;

  ParametersInternal() = default;
  ParametersInternal(AccType _flowrate, AccType _head, AccType _viscosity,
                     AccType _density = AccType("0"))
      : flowrate_q(std::move(_flowrate)),
        total_head(std::move(_head)),
        viscosity_v(std::move(_viscosity)),
        density_cp(std::move(_density)) {}

  ParametersInternal(const ParametersInternal& other)
      : flowrate_q(std::move(other.flowrate_q)),
        total_head(std::move(other.total_head)),
        viscosity_v(std::move(other.viscosity_v)),
        density_cp(std::move(other.density_cp)) {}
};

/// HFaktor depicts the coefficient for the head correction. 0.6 = kH06, 0.8 =
/// kH08, 1.0 = kH10, 1.2 = kH12.
enum class HFaktor : int { kH06 = 0, kH08 = 1, kH10 = 2, kH12 = 3 };

/// CorrectionFactors is a DTO used for the communication between Project and
/// Calculator. It is mainly used for the output of the calculation.
struct CorrectionFactors {
  double q = 0;
  double eta = 0;
  // The order of the factors is the same as the HFaktor enum.
  std::array<double, 4> h = {0, 0, 0, 0};

  int error_flag = 0;

  CorrectionFactors() = default;
  CorrectionFactors(double _q, double _eta, std::array<double, 4> _h)
      : q(q), eta(eta), h(_h) {}
  CorrectionFactors(int _error)
      : q(0), eta(0), h({0, 0, 0, 0}), error_flag(_error) {}
};

}  // namespace impl
}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_BUNDLE_TYPES_H_