/**
 * @file AnalyticJacobian.cpp
 * @brief Implementation of AnalyticJacobian.
 */

#include "AnalyticJacobian.hpp"

#include "Exceptions.hpp"

Matrix AnalyticJacobian::compute(const Function &f, const Vector &x) {
    if (!f.hasJacobian()) {
        throw DerivativeError("the analytic Jacobian was requested but the "
                              "function does not provide one");
    }
    return f.jacobian(x);
}
