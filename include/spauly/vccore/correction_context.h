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
#include <exception>
#include <future>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <string>

namespace spauly {
namespace vccore {

using CoefficientType = double;
template <size_t CoCount>
using CoefficientArray = std::array<CoefficientType, CoCount>;

class CorrectionContext {
 public:
  /// This only sets the initialization flag to false. Initialize() must be
  /// called to initialize the context properly and retrieve the data from the
  /// file.
  CorrectionContext();
  ~CorrectionContext() = default;

  /// The copy constructor simply copies the retrieved data from the original if
  /// it is initialized. Otherwise it constructs the object itself using the
  /// normal constructor.
  CorrectionContext(const CorrectionContext&);
  /// Copies the retrieved data from the original if it is initialized.
  /// Otherwise it constructs the object itself using the normal constructor. If
  /// this is already set to initialzed nothing happens.
  const CorrectionContext& operator=(const CorrectionContext&);

  /// TODO: Add a move constructor and move assignment operator.

  /// Initializes the context with the data from the specified file. Returns
  /// true if the context was initialized properly. Returns false if an error
  /// occurred during initialization.
  bool Initialize() noexcept;

  /// Waits for the context to be initialized. Returns true if the context is
  /// properly initialized.Returns false if an error occurred during
  /// initialization.
  bool WaitInitialization() const noexcept { return false; };

  /// Returns true if the context has been initialized properly. If false the
  /// context should not be used.
  inline const bool is_initialized() const { return is_initialized_; }

  /// Returns true if an error occurred during initialization.
  inline const bool has_error() const { return error_flag_; }

  /// Returns the coefficients for the Q correction.
  inline const CoefficientArray<6>& Q_GetCoefficients() const {
    return q_coefficients_;
  }
  /// Returns the coefficients for the Eta correction.
  inline const CoefficientArray<6>& Eta_GetCoefficients() const {
    return eta_coefficients_;
  }
  /// Returns the an array of the coefficients for each proportional H
  /// correction. Where 0 = 0.6, 1 = 0.8, 2 = 1.0, 3 = 1.2.
  inline const std::array<CoefficientArray<3>, 4>& H_GetCoefficients() const {
    return h_coefficients_;
  }

 protected:
  /// Initializes the context by copying th given data from the other context.
  bool Initialize(const CorrectionContext& other) noexcept { return false; };

  /// A helper struct that ensures that a new mutex is created for each copy of
  /// a CorrectionContext object.
  struct MutexHolder {
    mutable std::mutex mux;
    MutexHolder() : mux() {}
    MutexHolder(const MutexHolder& other) : mux() {}
  };

 private:
  /// Initialization flag.
  bool is_initialized_ = false;
  bool error_flag_ = false;

  // The data must be locked by this mutex during the initialization process
  MutexHolder mux_;
  // Used by WaitInitialization to wait for the initialization process to
  // finish.
  std::condition_variable initialized_condition_;

  CoefficientArray<6> q_coefficients_;
  CoefficientArray<6> eta_coefficients_;
  /// Stores the coefficients for each proportional H correction using these
  /// indices: 0 = 0.6, 1 = 0.8, 2 = 1.0, 3 = 1.2.
  std::array<CoefficientArray<3>, 4> h_coefficients_;
};
}  // namespace vccore

}  // namespace spauly

#endif  // SPAULY_VCCORE_CORRECTION_CONTEXT_H_
