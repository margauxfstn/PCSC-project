/**
 * @file FixedPoint.cpp
 * @brief Implementation of the fixed-point iteration.
 */

#include "FixedPoint.hpp"

#include <utility>

#include "Exceptions.hpp"

FixedPoint::FixedPoint(Vector x0, std::unique_ptr<Function> phi, double relaxation,
                       double tol, int maxIter)
    : IterativeRootFinder(tol, maxIter), x0_(std::move(x0)), phi_(std::move(phi)),
      relaxation_(relaxation) {}

Vector FixedPoint::initialize(const Function &f) {
    // A fixed point x = phi(x) only makes sense when phi maps R^n to R^n.
    if (f.inputDim() != f.outputDim()) {
        throw DimensionError("fixed-point iteration needs a square system (n == m)");
    }
    if (phi_ && (phi_->inputDim() != f.inputDim() || phi_->outputDim() != f.inputDim())) {
        throw DimensionError("phi must map R^n to R^n with the same n as f");
    }
    if (x0_.size() != f.inputDim()) {
        throw DimensionError("initial guess has the wrong size");
    }
    return x0_;
}

Vector FixedPoint::step(const Function &f, const Vector &x) {
    return applyPhi(f, x);
}

Vector FixedPoint::applyPhi(const Function &f, const Vector &x) const {
    if (phi_) {
        return (*phi_)(x);
    }
    return x - relaxation_ * f(x);
}
