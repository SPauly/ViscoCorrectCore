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

#include "spauly/vccore/impl/accuracy_type.h"

namespace spauly {
namespace vccore {
// Forward declarations
class ConversionCTX;

// PType is the type used for the input parameters. Since double may not be
// accurate enough for the calculations, std::string is used instead. Internally
// this will be converted to a more accurate representation.
using PType = std::string;
// DensityInputType is only used when the viscosity is given in centipoise.
using DensityInputType = std::string;

/// FlowrateUnit determines the unit of the flowrate. Available units are: m³/h,
/// l/min, gpm.
enum class FlowrateUnit : int {
  kCubicMetersPerHour,
  kLitersPerMinute,
  kGallonsPerMinute
} StandardFlowrateUnit = kCubicMetersPerHour;

/// HeadUnit determines the unit of the total head. Available units are: m, ft.
enum class HeadUnit : int { kMeters, kFeet } StandardHeadUnit = kMeters;

/// ViscosityUnit determines the unit of the viscosity. Available units are:
/// mm²/s, cSt, cP, mPa·s.
enum class ViscosityUnit : int {
  kSquareMilPerSecond,  // mm²/s
  kcSt,                 // Centistokes
  kcP,                  // Centipoise
  kmPas                 // Millipascal seconds
} StandardViscosityUnit = kSquareMilPerSecond;

/// DensityUnit determines the unit of the density. Available units are: g/l,
/// kg/m³.
enum class DensityUnit : int {
  kGramPerLiter,
  kKilogramsPerCubicMeter
} StandardDensityUnit = kGramPerLiter;

class Project {
 public:
  // ConversionCTX needs access to the internals of Project.
  friend class ConversionCTX;

  Project() = default;
  Project(PType _flowrate, PType _head, PType _viscosity,
          DensityInputType _density = 0,
          FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
          HeadUnit _h_unit = HeadUnit::kMeters,
          ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
          DensityUnit _d_unit = DensityUnit::kGramPerLiter)
      : input_flowrate_(_flowrate),
        input_total_head_(_head),
        input_viscosity_(_viscosity),
        input_density_cp_(_density),
        flowrate_unit_(_f_unit),
        head_unit_(_h_unit),
        viscosity_unit_(_v_unit),
        density_unit_(_d_unit) {}

  ~Project() = default;

  Project(const Project &other);
  Project &operator=(const Project &other);

  // Getters
  const double q() const;
  const double eta() const;
  const std::array<double, 4> h() const;
  const double h_06() const;
  const double h_08() const;
  const double h_10() const;
  const double h_12() const;

  /// Returns the name of the project.
  const std::string &name() const;
  /// Returns the floating point precision for the calculations.
  size_t floating_point_precision() const;
  /// Returns the ID of the project.
  const size_t id() const;
  /// Returns whether the project has an error.
  bool has_error() const;
  /// Returns the flowrate.
  const PType &flowrate() const;
  /// Returns the flowrate unit.
  const FlowrateUnit &flowrate_unit() const;
  /// Returns the total head.
  const PType &total_head() const;
  /// Returns the head unit.
  const HeadUnit &head_unit() const;
  /// Returns the viscosity.
  const PType &viscosity() const;
  /// Returns the viscosity unit.
  const ViscosityUnit &viscosity_unit() const;
  /// Returns the density.
  const DensityInputType &density() const;
  /// Returns the density unit.
  const DensityUnit &density_unit() const;

  // Setters

  /// Sets the name of the project.
  void set_name(const std::string &_name);

  /// Sets the floating point precision for the calculations.
  void set_floating_point_precision(size_t _precision);

  /// Sets the input parameters
  void Set(PType _flowrate, PType _head, PType _viscosity,
           DensityInputType _density = 0,
           FlowrateUnit _f_unit = FlowrateUnit::kCubicMetersPerHour,
           HeadUnit _h_unit = HeadUnit::kMeters,
           ViscosityUnit _v_unit = ViscosityUnit::kSquareMilPerSecond,
           DensityUnit _d_unit = DensityUnit::kGramPerLiter);

  /// Sets the flowrate to the specified value.
  void set_flowrate(const PType &_flowrate);
  /// Sets the flowrate unit to either m³/h, l/min or gpm.
  void set_flowrate_unit(const FlowrateUnit &_unit);

  /// Sets the total head to the specified value.
  void set_total_head(const PType &_head);
  /// Sets the head unit to either m or ft.
  void set_head_unit(const HeadUnit &_unit);

  /// Sets the viscosity to the specified value.
  void set_viscosity(const PType &_viscosity);
  /// Sets the viscosity unit to either mm²/s, cSt, cP or mPa·s.
  void set_viscosity_unit(const ViscosityUnit &_unit);

  /// Sets the density to the specified value.
  void set_density(const DensityInputType &_density);
  /// Sets the density unit to either g/l or kg/m³.
  void set_density_unit(const DensityUnit &_unit);

 private:
  mutable std::shared_mutex mtx_;
  bool has_error_ = false;

  // Settings for the project
  std::string name_;
  size_t id_;
  size_t floating_point_precision_;

  // Input parameters
  // This is used to avoid recomputing unchanged values. 0 = flowrate_, 1 =
  // total_head_, 2 = viscosity_, 3 = density_cp_
  std::array<bool, 4> was_changed_ = {true, true, true, true};

  PType input_flowrate_ = "";
  PType input_total_head_ = "";
  PType input_viscosity_ = "";
  /// Must be provided when the viscosity is given in centipoise.
  DensityInputType input_density_cp_ = "";

  FlowrateUnit flowrate_unit_ = FlowrateUnit::kCubicMetersPerHour;
  HeadUnit head_unit_ = HeadUnit::kMeters;
  ViscosityUnit viscosity_unit_ = ViscosityUnit::kSquareMilPerSecond;
  DensityUnit density_unit_ = DensityUnit::kGramPerLiter;

  // Internal representation of the input parameters
  impl::IType flowrate_;
  impl::IType total_head_;
  impl::IType viscosity_;
  impl::IType density_cp_;

  // Correction factors
  double q_;
  double eta_;
  std::array<double, 4> h_;

  // Computed helper functions for the correction factors
};

}  // namespace vccore
}  // namespace spauly

#endif  // SPAULY_VCCORE_PROJECT_H_