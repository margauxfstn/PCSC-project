//
// Created by marga on 13/11/2026.
//

#ifndef PCSCPROJECT_BISECTION_HPP
#define PCSCPROJECT_BISECTION_HPP

#include "RootFinder.hpp"

/// Bisection method on a bracketing interval [a,b] with f(a)*f(b) <= 0.
class Bisection : public RootFinder {
public:
    Bisection(double a, double b, double tol, int maxIter);

protected:
    /// Checks the sign condition; throws InvalidInterval if violated.
    double initialize(const Function &f) override;
    double step(const Function &f, double x) override;

private:
    double a0_, b0_; ///< initial interval
    double a_, b_;   ///< current interval
    double fa_;      ///< f(a_)
};

#endif //PCSCPROJECT_BISECTION_HPP
