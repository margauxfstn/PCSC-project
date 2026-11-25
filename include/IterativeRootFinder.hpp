/**
 * @file IterativeRootFinder.hpp
 * @brief Base class of iterative root finders: owns the iteration loop.
 */

#ifndef PCSCPROJECT_ITERATIVEROOTFINDER_HPP
#define PCSCPROJECT_ITERATIVEROOTFINDER_HPP

#include "RootFinder.hpp"

/**
 * @brief Iterative root finder x_{k+1} = step(x_k).
 *
 * Design ("template method" pattern): solve() contains the loop, the
 * stopping criteria, the history and the failure detection, written once
 * for all methods. A derived class only says how to start (initialize())
 * and how to go from x_k to x_{k+1} (step()).
 *
 * Stopping criteria: the run converges when ||f(x_k)|| < tol or
 * ||x_k - x_{k-1}|| < tol. It is reported as diverged if a non-finite value
 * appears, and as a breakdown if step() throws NumericalBreakdown.
 */
class IterativeRootFinder : public RootFinder {
public:
    /// @param tol     tolerance of the stopping criteria (> 0)
    /// @param maxIter maximum number of iterations (> 0)
    /// @throws std::invalid_argument for non-positive values
    IterativeRootFinder(double tol, int maxIter);

    /// Runs the iteration. Marked @c final: derived classes customise
    /// initialize() and step(), never the loop itself.
    Result solve(const Function &f) final;

    double tolerance() const { return tol_; }        ///< stopping tolerance
    int maxIterations() const { return maxIter_; }   ///< iteration budget

protected:
    /// Checks the inputs against @p f, resets the internal state and
    /// returns the initial approximation x_0.
    virtual Vector initialize(const Function &f) = 0;

    /// Computes x_{k+1} from x_k.
    /// @throws NumericalBreakdown if the method cannot continue
    virtual Vector step(const Function &f, const Vector &x) = 0;

private:
    double tol_;
    int maxIter_;
};

#endif // PCSCPROJECT_ITERATIVEROOTFINDER_HPP
