/**
 * @file IterativeRootFinder.cpp
 * @brief Implementation of the generic iteration loop.
 */

#include "IterativeRootFinder.hpp"

#include <cmath>
#include <stdexcept>

#include "Exceptions.hpp"

IterativeRootFinder::IterativeRootFinder(double tol, int maxIter)
    : tol_(tol), maxIter_(maxIter) {
    if (!(tol > 0.0)) {
        throw std::invalid_argument("tolerance must be positive");
    }
    if (maxIter <= 0) {
        throw std::invalid_argument("maximum number of iterations must be positive");
    }
}

Result IterativeRootFinder::solve(const Function &f) {
    Result res;
    Vector x = initialize(f);
    res.history.push_back(x);
    double residual = f(x).norm();

    if (residual < tol_) { // x_0 is already a root
        res.status = Status::Converged;
    }

    for (int k = 1; k <= maxIter_ && !res.converged(); ++k) {
        Vector xNew;
        try {
            xNew = step(f, x);
        } catch (const NumericalBreakdown &e) {
            res.status = Status::Breakdown;
            res.message = e.what();
            break;
        }
        res.history.push_back(xNew);
        res.iterations = k;

        const double increment = (xNew - x).norm();
        x = xNew;
        residual = x.allFinite() ? f(x).norm() : NAN;

        if (!std::isfinite(residual)) {
            res.status = Status::Diverged;
        } else if (residual < tol_ || increment < tol_) {
            res.status = Status::Converged;
        }
    }

    res.root = x;
    res.residual = residual;
    return res;
}
