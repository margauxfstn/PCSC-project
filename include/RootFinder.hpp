//
// Created by marga on 13/11/2026.
//

#ifndef PCSCPROJECT_ROOTFINDER_HPP
#define PCSCPROJECT_ROOTFINDER_HPP

#include "Function.hpp"
#include "Result.hpp"

/// Abstract root finder: owns the iteration loop and the stopping criteria.
/// Derived classes only define how to compute the next approximation.
class RootFinder {
public:
    /// @param tol     tolerance on |f(x)| and on |x_{k+1}-x_k|
    /// @param maxIter maximum number of iterations
    RootFinder(double tol, int maxIter);
    virtual ~RootFinder() = default;

    /// Runs the iteration on f and returns the outcome.
    Result solve(const Function &f);

protected:
    /// Prepares the method (checks inputs, resets state).
    /// @return the initial approximation x_0
    virtual double initialize(const Function &f) = 0;

    /// Computes the next approximation from the current one.
    virtual double step(const Function &f, double x) = 0;

private:
    double tol_;
    int maxIter_;
};

#endif //PCSCPROJECT_ROOTFINDER_HPP
