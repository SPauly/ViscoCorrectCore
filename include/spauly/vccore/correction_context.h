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
#ifndef SPAULY_VCCORE_CORRECTION_CONTEXT_H_
#define SPAULY_VCCORE_CORRECTION_CONTEXT_H_

namespace spauly {
namespace vccore {
class CorrectionContext {
 public:
  /// This only sets the initialization flag to false. Initialize() must be
  /// called to initialize the context properly.
  CorrectionContext() = default;
  ~CorrectionContext() = default;

  /// The copy constructor simply copies the retrieved data from the original if
  /// it is initialized. Otherwise it constructs the object itself using the
  /// normal constructor.
  CorrectionContext(const CorrectionContext&);
  /// Copies the retrieved data from the original if it is initialized.
  /// Otherwise it constructs the object itself using the normal constructor. If
  /// this is already set to initialzed nothing happens.
  const CorrectionContext& operator=(const CorrectionContext&);

 private:
};
}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_CORRECTION_CONTEXT_H_
