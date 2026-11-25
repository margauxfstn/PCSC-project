/**
 * @file FiniteDifferenceJacobian.cpp
 * @brief Implementation of FiniteDifferenceJacobian.
 */

#include "FiniteDifferenceJacobian.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

FiniteDifferenceJacobian::FiniteDifferenceJacobian(double step) : step_(step) {
    if (!(step > 0.0)) {
        throw std::invalid_argument("finite-difference step must be positive");
    }
}

Matrix FiniteDifferenceJacobian::compute(const Function &f, const Vector &x) {
    Matrix jac(f.outputDim(), x.size());
    for (Eigen::Index j = 0; j < x.size(); ++j) {
        const double h = step_ * std::max(1.0, std::abs(x(j)));
        Vector xPlus = x;
        Vector xMinus = x;
        xPlus(j) += h;
        xMinus(j) -= h;
        jac.col(j) = (f(xPlus) - f(xMinus)) / (2.0 * h);
    }
    return jac;
}
