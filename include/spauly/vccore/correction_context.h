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

#include <array>
#include <condition_variable>
#include <cstddef>
#include <mutex>

namespace spauly {
namespace vccore {

using CoefficientType = double;
template <size_t CoCount>
using CoefficientArray = std::array<CoefficientType, CoCount>;

class CorrectionContext {
 public:
  /// Retrieves the necessary data from a specified file and sets the
  /// initialized flag.
  CorrectionContext();
  ~CorrectionContext() = default;

  /// The copy constructor simply copies the retrieved data from the original if
  /// it is initialized. Otherwise it constructs the object itself using the
  /// normal constructor.
  CorrectionContext(const CorrectionContext&);
  /// Copies the retrieved data from the original if it is initialized.
  /// Otherwise it constructs the object itself using the normal constructor.
  const CorrectionContext& operator=(const CorrectionContext&);

  /// TODO: Add a move constructor and move assignment operator.

  /// Returns true if the context has been initialized properly. If false the
  /// context should not be used.
  bool IsInitialized() const;

  /// Waits for the context to be initialized. Returns true if the context is
  /// properly initialized.Returns false if an error occurred during
  /// initialization.
  bool WaitInitialization() const;

  /// Returns the coefficients for the Q correction.
  const CoefficientArray<6>& Q_GetCoefficients() const;
  /// Returns the coefficients for the Eta correction.
  const CoefficientArray<6>& Eta_GetCoefficients() const;
  /// Returns the an array of the coefficients for each proportional H
  /// correction. Where 0 = 0.6, 1 = 0.8, 2 = 1.0, 3 = 1.2.
  const std::array<CoefficientArray<6>, 4>& H_GetCoefficients() const;

 protected:
  /// Initializes the context with the data from the specified file. Returns
  /// true if the context was initialized properly. Returns false if an error
  /// occurred during initialization.
  bool Initialize();

 private:
  /// Initialization flag.
  bool is_initialized_;
  // The data must be locked by this mutex during the initialization process
  std::mutex initialized_mutex_;
  // Used by WaitInitialization to wait for the initialization process to
  // finish.
  std::condition_variable initialized_condition_;

  CoefficientArray<6> q_coefficients_;
  CoefficientArray<6> eta_coefficients_;
  /// Stores the coefficients for each proportional H correction using these
  /// indices: 0 = 0.6, 1 = 0.8, 2 = 1.0, 3 = 1.2.
  std::array<CoefficientArray<6>, 4> h_coefficients_;
};
}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_CORRECTION_CONTEXT_H_
