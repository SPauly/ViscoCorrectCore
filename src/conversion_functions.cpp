#include "spauly/vccore/impl/conversion_functions.h"

namespace spauly {
namespace vccore {
namespace impl {
/// Converts the input value to the base viscosity unit of mm2/s
DoubleT ConvertViscosityTomm2s(
    const DoubleT& value, const ViscosityUnit from, const DoubleT density = 0,
    const DensityUnit d_unit = DensityUnit::kGramPerLiter) noexcept {
  DoubleT out = DoubleT(0.0);

  switch (from) {
    case ViscosityUnit::kcP:
    case ViscosityUnit::kmPas:
      if (density != 0) {
        out = value / (density * kDensityToGPL.at(d_unit));
      }
      break;
    default:  // kcSt is the same as mm^2/s
      out = value;
      break;
  }
  return out;
}
}  // namespace impl

}  // namespace vccore

}  // namespace spauly
