/**
 * @file Newton.cpp
 * @brief Implementation of the Newton iteration.
 */

#include "Newton.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

#include "Exceptions.hpp"

Newton::Newton(Vector x0, std::unique_ptr<JacobianStrategy> jacobian, double tol, int maxIter)
    : IterativeRootFinder(tol, maxIter), x0_(std::move(x0)), jacobian_(std::move(jacobian)) {
    if (!jacobian_) {
        throw std::invalid_argument("Newton needs a Jacobian strategy");
    }
}

std::string Newton::name() const {
    return "Newton (" + jacobian_->name() + ")";
}

Vector Newton::initialize(const Function &f) {
    if (x0_.size() != f.inputDim()) {
        throw DimensionError("initial guess has size " + std::to_string(x0_.size()) +
                             " but the function expects " + std::to_string(f.inputDim()));
    }
    jacobian_->reset(); // forget a Jacobian frozen during a previous solve()
    return x0_;
}

Vector Newton::step(const Function &f, const Vector &x) {
    const Vector fx = f(x);
    const Matrix jac = jacobian_->compute(f, x);
    if (jac.rows() != fx.size() || jac.cols() != x.size()) {
        throw DimensionError("Jacobian has wrong dimensions");
    }
    // Solve J d = f(x) instead of computing J^{-1} (cheaper and more stable).
    const Eigen::ColPivHouseholderQR<Matrix> qr(jac);
    if (qr.rank() < std::min(jac.rows(), jac.cols())) {
        throw NumericalBreakdown("singular Jacobian (or zero derivative) at the current iterate");
    }
    return x - qr.solve(fx);
}
