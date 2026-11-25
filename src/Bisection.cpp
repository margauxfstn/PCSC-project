/**
 * @file Bisection.cpp
 * @brief Implementation of the bisection method.
 */

#include "Bisection.hpp"

#include <string>

#include "Exceptions.hpp"

namespace {
/// Evaluates a scalar function at a double (helper to avoid Vector noise).
double evalScalar(const Function &f, double x) {
    return f(Vector::Constant(1, x))(0);
}
} // namespace

Bisection::Bisection(double a, double b, double tol, int maxIter)
    : IterativeRootFinder(tol, maxIter), a0_(a), b0_(b), a_(a), b_(b), fa_(0.0) {
    if (!(a < b)) {
        throw InvalidInterval("bisection needs a < b");
    }
}

Vector Bisection::initialize(const Function &f) {
    if (!f.isScalar()) {
        throw DimensionError("bisection only applies to scalar functions R -> R");
    }
    a_ = a0_;
    b_ = b0_;
    fa_ = evalScalar(f, a_);
    const double fb = evalScalar(f, b_);
    if (fa_ * fb > 0.0) {
        throw InvalidInterval("f(a) and f(b) must have opposite signs on [" +
                              std::to_string(a_) + ", " + std::to_string(b_) + "]");
    }
    return Vector::Constant(1, 0.5 * (a_ + b_));
}

Vector Bisection::step(const Function &f, const Vector &x) {
    // x is the midpoint of the current interval: keep the half containing a root
    const double mid = x(0);
    const double fm = evalScalar(f, mid);
    if (fa_ * fm <= 0.0) {
        b_ = mid;
    } else {
        a_ = mid;
        fa_ = fm;
    }
    return Vector::Constant(1, 0.5 * (a_ + b_));
}
