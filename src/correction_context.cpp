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

bool CorrectionContext::Initialize() {
  std::lock_guard<std::mutex> lock(mux_.mux);
  if (is_initialized_) return true;

  // Open file
  // Read csv in for the coefficients

  // concurrently read the csv file for the graph coordinates and function

  // concurrently read the init file with all input and output restrictions

  // wait for the return of all functions and copy the data the the regarding
  // storage
}

}  // namespace vccore
}  // namespace spauly