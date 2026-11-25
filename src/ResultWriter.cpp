/**
 * @file ResultWriter.cpp
 * @brief Implementation of ResultWriter.
 */

#include "ResultWriter.hpp"

#include <iomanip>

namespace {
/// Prints a vector as "(x1, x2, ...)", or as a plain number in 1D.
void printVector(std::ostream &out, const Vector &v) {
    if (v.size() == 1) {
        out << v(0);
        return;
    }
    out << "(";
    for (Eigen::Index i = 0; i < v.size(); ++i) {
        out << (i > 0 ? ", " : "") << v(i);
    }
    out << ")";
}
} // namespace

ResultWriter::ResultWriter(std::ostream &out, int precision)
    : out_(out), precision_(precision) {}

void ResultWriter::write(const std::string &methodName, const Result &result,
                         bool showHistory) const {
    const auto oldPrecision = out_.precision(precision_);
    out_ << "method     : " << methodName << "\n"
         << "status     : " << toString(result.status);
    if (!result.message.empty()) {
        out_ << " (" << result.message << ")";
    }
    out_ << "\nroot       : ";
    printVector(out_, result.root);
    out_ << "\nresidual   : " << result.residual << "\n"
         << "iterations : " << result.iterations << "\n";
    if (showHistory) {
        out_ << "history    :\n";
        for (std::size_t k = 0; k < result.history.size(); ++k) {
            out_ << "  x_" << k << " = ";
            printVector(out_, result.history[k]);
            out_ << "\n";
        }
    }
    out_.precision(oldPrecision);
}
