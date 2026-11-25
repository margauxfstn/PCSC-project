/**
 * @file AitkenFixedPoint.cpp
 * @brief Implementation of Aitken's acceleration.
 */

#include "AitkenFixedPoint.hpp"

#include <cmath>
#include <limits>

Vector AitkenFixedPoint::step(const Function &f, const Vector &x) {
    const Vector y = applyPhi(f, x);
    const Vector z = applyPhi(f, y);
    Vector next(x.size());
    for (Eigen::Index i = 0; i < x.size(); ++i) {
        const double denom = z(i) - 2.0 * y(i) + x(i);
        const double scale = std::abs(z(i)) + 2.0 * std::abs(y(i)) + std::abs(x(i));
        // If the denominator is only rounding noise, the iteration has
        // (numerically) converged: keep the plain fixed-point value.
        if (std::abs(denom) <= 4.0 * std::numeric_limits<double>::epsilon() * scale) {
            next(i) = z(i);
        } else {
            const double diff = y(i) - x(i);
            next(i) = x(i) - diff * diff / denom;
        }
    }
    return next;
}
