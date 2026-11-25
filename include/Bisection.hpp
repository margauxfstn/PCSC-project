/**
 * @file Bisection.hpp
 * @brief Bisection method.
 */

#ifndef PCSCPROJECT_BISECTION_HPP
#define PCSCPROJECT_BISECTION_HPP

#include "IterativeRootFinder.hpp"

/**
 * @brief Bisection on a bracketing interval [a,b] with f(a) f(b) <= 0.
 *
 * At each step the interval is halved, keeping the half where f changes
 * sign; x_k is the midpoint. Needs no derivative. Converges linearly (the
 * error bound is divided by 2 at each step).
 *
 * Limitation: only defined for scalar functions R -> R (a DimensionError is
 * thrown otherwise), since there is no sign change in R^n.
 */
class Bisection : public IterativeRootFinder {
public:
    /// @throws InvalidInterval if a >= b
    Bisection(double a, double b, double tol, int maxIter);

    std::string name() const override { return "bisection"; }

protected:
    /// Checks the dimension and the sign condition; returns the midpoint.
    /// @throws DimensionError, InvalidInterval
    Vector initialize(const Function &f) override;
    Vector step(const Function &f, const Vector &x) override;

private:
    double a0_, b0_; ///< initial interval (kept so that solve() can be reused)
    double a_, b_;   ///< current interval
    double fa_;      ///< f(a_)
};

#endif // PCSCPROJECT_BISECTION_HPP
