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

#include "spauly/vccore/input_types.h"
#include "spauly/vccore/impl/bundle_types.h"
#include "spauly/vccore/impl/calculator.h"

namespace spauly {
namespace vccore {
// Forward declarations
class Project;
class CalculationCTX;

/// Project is the main class of the library the user will interact with. It
/// stores the input parameters and the correction factors. The calculation is
/// done by the Calculator class. The Project class is thread-safe.
/// Project expexts a CalculationCTX to be provided. The context holds necessary
/// constants and helpers for the calculation and should be shared across
/// multiple projects.
class Project {
 private:
  using ReadLock = std::shared_lock<std::shared_mutex>;
  using WriteLock = std::unique_lock<std::shared_mutex>;

 public:
  Project() = delete;
  Project(std::shared_ptr<CalculationCTX> ctx);
  Project(std::shared_ptr<CalculationCTX> ctx, InputT _flowrate, InputT _head,
          InputT _viscosity, InputT _density,
          FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
          HeadUnit _h_unit = HeadUnit::kMeters,
          ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
          DensityUnit _d_unit = DensityUnit::kGramPerLiter);

  ~Project() = default;

  Project(const Project &other);
  Project &operator=(const Project &other);

  /// Calculates the correction factors for the given input parameters. Calling
  /// this before trying to access any of the correction factors is more
  /// efficient. Returns false if the input parameters are invalid.
  bool Calculate();

  /// Converts the given input parameters to the internal representation. This
  /// can be used to validate on which bases the correction factors were
  /// calculated.
  const Project ShowConverted() const;

  // Getters
  double q();
  double eta();
  const std::array<double, 4> h();
  double h_06();
  double h_08();
  double h_10();
  double h_12();

  /// Returns the name of the project.
  inline const std::string &name() const {
    ReadLock lock(mtx_);
    return name_;
  }

  /// Returns the floating point precision for the calculations.
  inline size_t floating_point_precision() const {
    ReadLock lock(mtx_);
    return floating_point_precision_;
  }

  /// Returns the ID of the project.
  inline const size_t id() const {
    ReadLock lock(mtx_);
    return id_;
  }

  /// Returns whether the project has an error.
  inline bool has_error() const {
    ReadLock lock(mtx_);
    return has_error_;
  }

  /// Returns the flowrate.
  inline const InputT &flowrate() const {
    ReadLock lock(mtx_);
    return input_flowrate_;
  }

  /// Returns the flowrate unit.
  inline const FlowrateUnit &flowrate_unit() const {
    ReadLock lock(mtx_);
    return flowrate_unit_;
  }

  /// Returns the total head.
  inline const InputT &total_head() const {
    ReadLock lock(mtx_);
    return input_total_head_;
  }

  /// Returns the head unit.
  inline const HeadUnit &head_unit() const {
    ReadLock lock(mtx_);
    return head_unit_;
  }

  /// Returns the viscosity.
  inline const InputT &viscosity() const {
    ReadLock lock(mtx_);
    return input_viscosity_;
  }

  /// Returns the viscosity unit.
  inline const ViscosityUnit &viscosity_unit() const {
    ReadLock lock(mtx_);
    return viscosity_unit_;
  }

  /// Returns the density.
  inline const InputT &density() const {
    ReadLock lock(mtx_);
    return input_density_cp_;
  }

  /// Returns the density unit.
  inline const DensityUnit &density_unit() const {
    ReadLock lock(mtx_);
    return density_unit_;
  }

  // Setters

  /// Sets the name of the project.
  inline void set_name(const std::string &_name) {
    WriteLock lock(mtx_);
    name_ = _name;
  }

  /// Sets the floating point precision for the calculations.
  void set_floating_point_precision(size_t _precision);

  /// Sets the input parameters
  void Set(InputT _flowrate, InputT _head, InputT _viscosity, InputT _density,
           FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
           HeadUnit _h_unit = HeadUnit::kMeters,
           ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
           DensityUnit _d_unit = DensityUnit::kGramPerLiter);

  /// Sets the flowrate to the specified value.
  void set_flowrate(const InputT &_flowrate);
  /// Sets the flowrate unit to either m³/h, l/min or gpm.
  void set_flowrate_unit(const FlowrateUnit &_unit);

  /// Sets the total head to the specified value.
  void set_total_head(const InputT &_head);
  /// Sets the head unit to either m or ft.
  void set_head_unit(const HeadUnit &_unit);

  /// Sets the viscosity to the specified value.
  void set_viscosity(const InputT &_viscosity);
  /// Sets the viscosity unit to either mm²/s, cSt, cP or mPa·s.
  void set_viscosity_unit(const ViscosityUnit &_unit);

  /// Sets the density to the specified value.
  void set_density(const InputT &_density);
  /// Sets the density unit to either g/l or kg/m³.
  void set_density_unit(const DensityUnit &_unit);

 protected:
  /// Helper function that resets the correction factors and metadata to
  /// indicate an uncomputed change. Should be called whenever an input_ value
  /// changes. Assumes that mtx_ is locked!
  void IndicateChange();

  /// Helper function that handles the actual calculation. Assumes that mtx_ is
  /// locked!
  bool CalcImpl();

  /// This wrapper ensures that the given read lock is unlocked and a write lock
  /// is aquired before calling CalcImpl.
  bool GetterCalcWrapper(ReadLock &lock);

 private:
  std::shared_ptr<CalculationCTX> ctx_ = nullptr;
  const impl::Calculator calculator_;

  mutable std::shared_mutex mtx_;
  bool was_computed_ = false;
  bool has_error_ = false;

  // Settings for the project
  std::string name_ = "";
  size_t id_ = 0;
  size_t floating_point_precision_ = 17;

  // Input parameters
  // This is used to avoid recomputing unchanged values. 0 = flowrate_, 1 =
  // total_head_, 2 = viscosity_, 3 = density_cp_
  std::array<bool, 4> was_changed_ = {true, true, true, true};

  InputT input_flowrate_;
  InputT input_total_head_;
  InputT input_viscosity_;
  /// Must be provided when the viscosity is given in centipoise.
  InputT input_density_cp_;

  FlowrateUnit flowrate_unit_ = FlowrateUnit::kCubicMetersPerHour;
  HeadUnit head_unit_ = HeadUnit::kMeters;
  ViscosityUnit viscosity_unit_ = ViscosityUnit::kSquareMilPerSecond;
  DensityUnit density_unit_ = DensityUnit::kGramPerLiter;

  // Converted Parameters
  impl::ParametersInternal converted_input_;

  // Result
  impl::CorrectionFactors res_;

  // Computed helper functions for the correction factors
};

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_PROJECT_H_