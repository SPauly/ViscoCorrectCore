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
#include "spauly/vccore/project.h"

#include "spauly/vccore/impl/conversion_functions.h"

namespace spauly {
namespace vccore {
Project::Project(std::shared_ptr<CalculationCTX> ctx)
    : ctx_(ctx), calculator_(ctx) {}

Project::Project(std::shared_ptr<CalculationCTX> ctx, PType _flowrate,
                 PType _head, PType _viscosity, PType _density,
                 FlowrateUnit _f_unit, HeadUnit _h_unit, ViscosityUnit _v_unit,
                 DensityUnit _d_unit)
    : ctx_(ctx),
      calculator_(ctx),
      input_flowrate_(_flowrate),
      input_total_head_(_head),
      input_viscosity_(_viscosity),
      input_density_cp_(_density),
      flowrate_unit_(_f_unit),
      head_unit_(_h_unit),
      viscosity_unit_(_v_unit),
      density_unit_(_d_unit) {}

Project::Project(const Project& other) : calculator_(other.ctx_) {
  operator=(other);
}

Project& Project::operator=(const Project& other) {
  ReadLock lock(other.mtx_);
  WriteLock lock2(mtx_);

  ctx_ = other.ctx_;
  name_ = other.name_;

  input_flowrate_ = other.input_flowrate_;
  input_total_head_ = other.input_total_head_;
  input_viscosity_ = other.input_viscosity_;
  input_density_cp_ = other.input_density_cp_;
  flowrate_unit_ = other.flowrate_unit_;
  head_unit_ = other.head_unit_;
  viscosity_unit_ = other.viscosity_unit_;
  density_unit_ = other.density_unit_;

  return *this;
}

bool Project::Calculate() {
  WriteLock lock(mtx_);

  if (was_computed_) return has_error_;

  return CalcImpl();
}

double Project::q() {
  ReadLock lock(mtx_);
  if (!was_computed_) GetterCalcWrapper(lock);

  return res_.q;
}

double Project::eta() {
  ReadLock lock(mtx_);
  if (!was_computed_) GetterCalcWrapper(lock);

  return res_.eta;
}

const std::array<double, 4> Project::h() {
  ReadLock lock(mtx_);
  if (!was_computed_) GetterCalcWrapper(lock);

  return res_.h;
}

double Project::h_06() {
  ReadLock lock(mtx_);
  if (!was_computed_) GetterCalcWrapper(lock);

  return res_.h[0];
}

double Project::h_08() {
  ReadLock lock(mtx_);
  if (!was_computed_) GetterCalcWrapper(lock);

  return res_.h[1];
}

double Project::h_10() {
  ReadLock lock(mtx_);
  if (!was_computed_) GetterCalcWrapper(lock);

  return res_.h[2];
}

double Project::h_12() {
  ReadLock lock(mtx_);
  if (!was_computed_) GetterCalcWrapper(lock);

  return res_.h[3];
}

void Project::set_floating_point_precision(size_t _precision) {
  WriteLock lock(mtx_);
  floating_point_precision_ = _precision;
  IndicateChange();
}

void Project::Set(PType _flowrate, PType _head, PType _viscosity,
                  PType _density, FlowrateUnit _f_unit, HeadUnit _h_unit,
                  ViscosityUnit _v_unit, DensityUnit _d_unit) {
  WriteLock lock(mtx_);
  input_flowrate_ = _flowrate;
  input_total_head_ = _head;
  input_viscosity_ = _viscosity;
  input_density_cp_ = _density;
  flowrate_unit_ = _f_unit;
  head_unit_ = _h_unit;
  viscosity_unit_ = _v_unit;
  density_unit_ = _d_unit;

  was_changed_ = {true, true, true, true};

  IndicateChange();
}

void Project::set_flowrate(const PType& _flowrate) {
  WriteLock lock(mtx_);
  input_flowrate_ = _flowrate;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[0] = true;
}

void Project::set_flowrate_unit(const FlowrateUnit& _unit) {
  WriteLock lock(mtx_);
  flowrate_unit_ = _unit;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[0] = true;
}

void Project::set_total_head(const PType& _head) {
  WriteLock lock(mtx_);
  input_total_head_ = _head;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[1] = true;
}

void Project::set_head_unit(const HeadUnit& _unit) {
  WriteLock lock(mtx_);
  head_unit_ = _unit;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[1] = true;
}

void Project::set_viscosity(const PType& _viscosity) {
  WriteLock lock(mtx_);
  input_viscosity_ = _viscosity;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[2] = true;
}

void Project::set_viscosity_unit(const ViscosityUnit& _unit) {
  WriteLock lock(mtx_);
  viscosity_unit_ = _unit;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[2] = true;
}

void Project::set_density(const PType& _density) {
  WriteLock lock(mtx_);
  input_density_cp_ = _density;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[3] = true;
}

void Project::set_density_unit(const DensityUnit& _unit) {
  WriteLock lock(mtx_);
  density_unit_ = _unit;
  IndicateChange();

  // To speed up the calculation we indicate which parameter has changed.
  was_changed_[3] = true;
}

void Project::IndicateChange() {
  // We assume that mtx_ is locked when this is called since this will only be
  // called after a change.
  has_error_ = false;
  was_computed_ = false;

  res_.q = 0;
  res_.eta = 0;
  res_.h = {0, 0, 0, 0};
  res_.error_flag = 0;
}

bool Project::CalcImpl() {
  // Cast the input to the needed types. And conver them to the needed units.
  // But only if they were changed.

  if (was_changed_[0])
    converted_input_.flowrate_q = impl::ConvertToBaseUnit<FlowrateUnit>(
        impl::IType(input_flowrate_), flowrate_unit_);

  if (was_changed_[1])
    converted_input_.total_head = impl::ConvertToBaseUnit<HeadUnit>(
        converted_input_.total_head, head_unit_);

  if (was_changed_[2] || was_changed_[3]) {
    converted_input_.viscosity_v = impl::ConvertViscosityTomm2s(
        converted_input_.viscosity_v, viscosity_unit_,
        converted_input_.density_cp, density_unit_);
    converted_input_.density_cp = impl::ConvertToBaseUnit<DensityUnit>(
        converted_input_.density_cp, density_unit_);
  }

  // Calculate the correction factors.
  res_ = calculator_.Calculate(converted_input_);

  was_computed_ = true;
  was_changed_ = {false, false, false, false};
  return has_error_ = res_.error_flag != 0;
}

bool Project::GetterCalcWrapper(ReadLock& lock) {
  // To avoid deadlock the Readlock must be released.
  lock.unlock();

  WriteLock lock2(mtx_);
  bool res = CalcImpl();

  // Reacquire the Readlock before returning.
  lock2.unlock();
  lock.lock();

  return res;
}

}  // namespace vccore
}  // namespace spauly