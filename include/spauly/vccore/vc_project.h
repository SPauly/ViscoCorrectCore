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
#ifndef SPAULY_VCCORE_VC_PROJECT_H_
#define SPAULY_VCCORE_VC_PROJECT_H_

namespace spauly {
namespace vccore {

class Project {
 public:
  Project() = delete;
  /// Project needs a valid CalculationCTX object to work with.
  Project(const CalculationCTX& ctx);

 private:
  const CalculationCTX& ctx_;
};

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_VC_PROJECT_H_