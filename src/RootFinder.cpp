//
// Created by marga on 13/11/2026.
//

#include "RootFinder.hpp"

#include <cmath>

RootFinder::RootFinder(double tol, int maxIter)
    : tol_(tol), maxIter_(maxIter) {}

Result RootFinder::solve(const Function &f) {
    Result res;
    double x = initialize(f);
    res.history.push_back(x);

    for (int k = 1; k <= maxIter_; ++k) {
        const double xNew = step(f, x);
        res.history.push_back(xNew);
        res.iterations = k;

        const double residual = std::abs(f(xNew));
        const bool smallStep = std::abs(xNew - x) < tol_;
        x = xNew;

        if (residual < tol_ || smallStep) {
            res.converged = true;
            break;
        }
    }

    res.root = x;
    res.residual = std::abs(f(x));
    return res;
}