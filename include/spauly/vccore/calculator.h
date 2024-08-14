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
#ifndef SPAULY_VCCORE_IMPL_CALCULATOR_H_
#define SPAULY_VCCORE_IMPL_CALCULATOR_H_

#include <memory>

#include "spauly/vccore/impl/bundle_types.h"
#include "spauly/vccore/calculation_ctx.h"

namespace spauly {
namespace vccore {
namespace impl {

class Calculator {
 public:
  Calculator() = delete;
  Calculator(std::shared_ptr<CalculationCTX> ctx) : ctx_(ctx) {}

  CorrectionFactors Calculate(const ParametersInternal& params) const;

 private:
  std::shared_ptr<CalculationCTX> ctx_;
};
}  // namespace impl
}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_IMPL_CALCULATOR_H_