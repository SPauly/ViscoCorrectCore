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
#include <spauly/vccore/correction_context.h>

namespace spauly {
namespace vccore {
CorrectionContext::CorrectionContext() : is_initialized_(false) {}

CorrectionContext::CorrectionContext(const CorrectionContext& other)
    : is_initialized_(false) {
  if (other.is_initialized_)
    is_initialized_ = Initialize(other);
  else
    is_initialized_ = Initialize();
}

const CorrectionContext& CorrectionContext::operator=(
    const CorrectionContext& other) {
  if (!is_initialized_) {
    if (other.is_initialized_)
      is_initialized_ = Initialize(other);
    else
      is_initialized_ = Initialize();
  }
  return *this;
}

bool CorrectionContext::Initialize() noexcept { return is_initialized_ = true; }

}  // namespace vccore
}  // namespace spauly