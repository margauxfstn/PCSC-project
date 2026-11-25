/**
 * @file JacobianStrategy.hpp
 * @brief Interface of the ways to obtain the Jacobian of a function.
 */

#ifndef PCSCPROJECT_JACOBIANSTRATEGY_HPP
#define PCSCPROJECT_JACOBIANSTRATEGY_HPP

#include <string>

#include "Function.hpp"

/**
 * @brief How a Newton-like method obtains the Jacobian J(x) (or f'(x)).
 *
 * Design ("strategy" pattern): Newton *has a* JacobianStrategy instead of
 * having one subclass per way of computing the derivative. Combining Newton
 * with different strategies gives:
 *  - AnalyticJacobian          -> Newton method
 *  - FiniteDifferenceJacobian  -> Newton with approximated derivative
 *  - FrozenJacobian            -> modified Newton (Jacobian frozen at x_0)
 *  - ChordSlope                -> chord method (slope of f over [a,b])
 */
class JacobianStrategy {
public:
    virtual ~JacobianStrategy() = default;

    /// Clears any cached state. Called at the start of every solve().
    virtual void reset() {}

    /// Returns J(x), an m x n matrix. Non-const because some strategies
    /// cache the matrix.
    virtual Matrix compute(const Function &f, const Vector &x) = 0;

    /// Short description, used in the solver name.
    virtual std::string name() const = 0;
};

#endif // PCSCPROJECT_JACOBIANSTRATEGY_HPP
