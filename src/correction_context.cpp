#include <spauly/vccore/correction_context.h>

#include "third_party/fast-cpp-csv-parser/csv.h"

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

  try {
    // Start the threads to read the csv files
    coef_future_ = std::async(std::launch::async,
                              &CorrectionContext::ReadCoefficients, this);

    // wait for the return of all functions without error
    return is_initialized_ = coef_future_.get();

  } catch (const std::exception& e) {
    csv_error_ = e;
    error_flag_ = true;
    return is_initialized_ = false;
  }
}

bool CorrectionContext::ReadCoefficients() {
  try {
    CSVReaderType reader(kCoefPath);

    /// TODO: verify that the version of the file is valid and that the file is
    /// not corrupted

    reader.read_header(io::ignore_no_column, "ID", "C0", "C1", "C2", "C3", "C4",
                       "C5");

    unsigned int id;
    CoefficientArray<6> c;

    while (reader->read_row(id, c[0], c[1], c[2], c[3], c[4], c[5])) {
      switch (id) {
        case 0:
          q_coefficients_ = c;
          break;
        case 1:
          eta_coefficients_ = c;
          break;
        default:
          // This means we have to handle the h correction factors. The id must
          // be offset by 2 to determine which h correction factor has been
          // read.
          int pos = id - 2;
          if (pos >= 0 && pos <= 3) {
            h_coefficients_.at(pos)[0] = c[0];
            h_coefficients_.at(pos)[1] = c[1];
            h_coefficients_.at(pos)[2] = c[2];
          }
          break;
      }
    }
    // only return true if all the coefficients have been read meaning they
    // are != 0
    return q_coefficients_[0] && eta_coefficients_[0] &&
           h_coefficients_[0][0] && h_coefficients_[1][0] &&
           h_coefficients_[2][0] && h_coefficients_[3][0];
  } catch (const std::exception& e) {
    throw e;
  }
}

}  // namespace vccore
}  // namespace spauly