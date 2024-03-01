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

}  // namespace vccore
}  // namespace spauly