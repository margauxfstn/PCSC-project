//
// Created by marga on 13/11/2026.
//

#include "Bisection.hpp"

#include <string>

Bisection::Bisection(double a, double b, double tol, int maxIter)
    : RootFinder(tol, maxIter), a0_(a), b0_(b), a_(a), b_(b), fa_(0.0) {}

double Bisection::initialize(const Function &f) {
    a_ = a0_;
    b_ = b0_;
    fa_ = f(a_);
    const double fb = f(b_);
    if (fa_ * fb > 0.0) {
        throw InvalidInterval("f(a) and f(b) must have opposite signs on [" +
                              std::to_string(a_) + ", " + std::to_string(b_) +
                              "]");
    }
    return 0.5 * (a_ + b_);
}

double Bisection::step(const Function &f, double x) {
    // x is the midpoint of the current interval: keep the half containing a root
    const double fm = f(x);
    if (fa_ * fm <= 0.0) {
        b_ = x;
    } else {
        a_ = x;
        fa_ = fm;
    }
    return 0.5 * (a_ + b_);
}