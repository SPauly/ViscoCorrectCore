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
#ifndef SPAULY_VCCORE_PROJECT_H_
#define SPAULY_VCCORE_PROJECT_H_

#include <array>
#include <cstddef>
#include <string>
#include <shared_mutex>

#include "spauly/vccore/data.h"

namespace spauly {
namespace vccore {

class Project {
 private:
  using ReadLock = std::shared_lock<std::shared_mutex>;
  using WriteLock = std::unique_lock<std::shared_mutex>;

 public:
  Project() = default;
  Project(InputT _flowrate, InputT _head, InputT _viscosity, InputT _density,
          FlowrateUnit _f_unit = kStandardFlowrateUnit,
          HeadUnit _h_unit = kStandardHeadUnit,
          ViscosityUnit _v_unit = kStandardViscosityUnit,
          DensityUnit _d_unit = kStandardDensityUnit);

  ~Project() = default;

  Project(const Project &other);
  Project &operator=(const Project &other);

  /// @brief Converts the given input parameters to the internal representation.
  /// This can be used to validate on which bases the correction factors were
  /// calculated.
  /// @return The converted input parameters.
  const Parameters ShowConverted() const;

  // Getters

  /// @brief Returns the q correction factor.
  /// @return The q correction factor.
  double q();

  /// @brief Returns the eta correction factor.
  /// @return The h correction factor to return.
  double eta();

  /// @brief Returns the h correction factors.
  /// @return The h correction factors as an array. Where the index 0 = kH06, 1
  /// = kH08, 2 = kH10, 3 = kH12.
  const std::array<double, 4> h();

  /// @brief Returns the h correction factor for kH06.
  /// @return The h correction factor for kH06.
  double h_06();

  /// @brief Returns the h correction factor for kH08.
  /// @return The h correction factor for kH08.
  double h_08();

  /// @brief Returns the h correction factor for kH10.
  /// @return The h correction factor for kH10.
  double h_10();

  /// @brief Returns the h correction factor for kH12.
  /// @return The h correction factor for kH12.
  double h_12();

  /// @brief Returns wheter there is an error.
  /// @return True if there is an error, false otherwise.
  inline bool has_error() const {
    ReadLock lock(mux_);
    return has_error_;
  }

  /// @brief Returns the flowrate.
  /// @return The flowrate.
  inline const InputT &flowrate() const {
    ReadLock lock(mux_);
    return input_flowrate_;
  }

  /// @brief Returns the flowrate unit.
  /// @return The flowrate unit.
  inline const FlowrateUnit &flowrate_unit() const {
    ReadLock lock(mux_);
    return flowrate_unit_;
  }

  /// @brief Returns the total head.
  /// @return The total head.
  inline const InputT &total_head() const {
    ReadLock lock(mux_);
    return input_total_head_;
  }

  /// @brief Returns the head unit.
  /// @return The head unit.
  inline const HeadUnit &head_unit() const {
    ReadLock lock(mux_);
    return head_unit_;
  }

  /// @brief Returns the viscosity.
  /// @return The viscosity.
  inline const InputT &viscosity() const {
    ReadLock lock(mux_);
    return input_viscosity_;
  }

  /// @brief Returns the viscosity unit.
  /// @return The viscosity unit.
  inline const ViscosityUnit &viscosity_unit() const {
    ReadLock lock(mux_);
    return viscosity_unit_;
  }

  /// @brief Returns the density.
  /// @return The density.
  inline const InputT &density() const {
    ReadLock lock(mux_);
    return input_density_cp_;
  }

  /// @brief Returns the density unit.
  /// @return The density unit.
  inline const DensityUnit &density_unit() const {
    ReadLock lock(mux_);
    return density_unit_;
  }

  /// @brief Sets the input parameters.
  /// @param _flowrate The flowrate.
  /// @param _head The total head.
  /// @param _viscosity The viscosity.
  /// @param _density The density.
  /// @param _f_unit The flowrate unit.
  /// @param _h_unit The head unit.
  /// @param _v_unit The viscosity unit.
  /// @param _d_unit The density unit.
  void Set(InputT _flowrate, InputT _head, InputT _viscosity, InputT _density,
           FlowrateUnit _f_unit = kStandardFlowrateUnit,
           HeadUnit _h_unit = kStandardHeadUnit,
           ViscosityUnit _v_unit = kStandardViscosityUnit,
           DensityUnit _d_unit = kStandardDensityUnit);

  /// @brief Sets the flowrate to the specified value.
  /// @param _flowrate The flowrate value.
  void set_flowrate(const InputT &_flowrate);

  /// @brief Sets the flowrate unit to either m³/h, l/min or gpm.
  /// @param _unit The flowrate unit.
  void set_flowrate_unit(const FlowrateUnit &_unit);

  /// @brief Sets the total head to the specified value.
  /// @param _head The total head value.
  void set_total_head(const InputT &_head);

  /// @brief Sets the head unit to either m or ft.
  /// @param _unit The head unit.
  void set_head_unit(const HeadUnit &_unit);

  /// @brief Sets the viscosity to the specified value.
  /// @param _viscosity The viscosity value.
  void set_viscosity(const InputT &_viscosity);

  /// @brief Sets the viscosity unit to either mm²/s, cSt, cP or mPa·s.
  /// @param _unit The viscosity unit.
  void set_viscosity_unit(const ViscosityUnit &_unit);

  /// @brief Sets the density to the specified value.
  /// @param _density The density value.
  void set_density(const InputT &_density);

  /// @brief Sets the density unit to either g/l or kg/m³.
  /// @param _unit The density unit.
  void set_density_unit(const DensityUnit &_unit);

 protected:
  /// @brief Helper function that resets the correction factors and metadata to
  /// indicate an uncomputed change. Should be called whenever an input value
  /// changes. Assumes that mux_ is locked!
  void IndicateChange();

  /// @brief Helper function that handles the actual calculation.
  /// Assumes that mux_ is locked!
  /// @return True if the calculation was successful, false otherwise.
  bool CalcImpl();

  /// @brief This wrapper ensures that the given read lock is unlocked and a
  /// write lock is acquired before calling CalcImpl.
  /// @param lock The read lock.
  /// @return True if the calculation was successful, false otherwise.
  bool GetterCalcWrapper(ReadLock &lock);

 private:
  mutable std::shared_mutex mux_;
  bool was_computed_ = false;
  bool has_error_ = false;
  // Settings for the project
  std::string name_ = "";

  // Input parameters
  // This is used to avoid recomputing unchanged values. 0 = flowrate_, 1 =
  // total_head_, 2 = viscosity_, 3 = density_cp_
  std::array<bool, 4> was_changed_ = {true, true, true, true};

  Parameters input_;
  Units units_;

  CorrectionFactors res_;

  // Computed helper functions for the correction factors
};

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_PROJECT_H_