/**
 * @file SolverFactory.hpp
 * @brief Builds functions and solvers from the parameters of a run.
 */

#ifndef PCSCPROJECT_SOLVERFACTORY_HPP
#define PCSCPROJECT_SOLVERFACTORY_HPP

#include <memory>

#include "Config.hpp"
#include "FunctionLibrary.hpp"
#include "JacobianStrategy.hpp"
#include "RootFinder.hpp"

/**
 * @brief The only place that knows the concrete solver classes.
 *
 * Design ("factory"): main() and the rest of the program only manipulate
 * RootFinder and Function through pointers to the base classes, so adding
 * a method means writing its class and one branch in makeSolver().
 *
 * Keys read: see the README (method, function, phi, x0, a, b, tol,
 * max_iter, jacobian, fd_step, relaxation).
 */
class SolverFactory {
public:
    /// @param library where function names are looked up (must outlive the factory)
    explicit SolverFactory(const FunctionLibrary &library);

    /// Builds the function named by the key "function".
    std::unique_ptr<Function> makeFunction(const Config &cfg) const;

    /// Builds the solver named by the key "method".
    /// @throws ConfigError for an unknown method or a missing parameter
    std::unique_ptr<RootFinder> makeSolver(const Config &cfg) const;

private:
    /// Strategy named by the key "jacobian" (analytic | finite_difference).
    std::unique_ptr<JacobianStrategy> makeJacobian(const Config &cfg) const;

    const FunctionLibrary &library_;
};

#endif // PCSCPROJECT_SOLVERFACTORY_HPP
