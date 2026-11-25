/**
 * @file Newton.hpp
 * @brief Newton-like methods for f : R^n -> R^m.
 */

#ifndef PCSCPROJECT_NEWTON_HPP
#define PCSCPROJECT_NEWTON_HPP

#include <memory>

#include "IterativeRootFinder.hpp"
#include "JacobianStrategy.hpp"

/**
 * @brief Newton iteration x_{k+1} = x_k - J_k^{-1} f(x_k).
 *
 * The matrix J_k is given by a JacobianStrategy, so the same class
 * implements Newton, modified Newton and the chord method (see
 * JacobianStrategy). The linear system J_k d = f(x_k) is solved with Eigen
 * (column-pivoting QR); J_k is never inverted. When m > n this gives the
 * least-squares step (Gauss-Newton).
 *
 * With the analytical Jacobian, convergence is quadratic near a simple
 * root, and only linear near a multiple root.
 */
class Newton : public IterativeRootFinder {
public:
    /// @param x0       initial guess
    /// @param jacobian how J_k is obtained (owned by the solver)
    /// @throws std::invalid_argument if @p jacobian is null
    Newton(Vector x0, std::unique_ptr<JacobianStrategy> jacobian, double tol, int maxIter);

    /// e.g. "Newton (analytic Jacobian)".
    std::string name() const override;

protected:
    /// @throws DimensionError if x0 does not have size f.inputDim()
    Vector initialize(const Function &f) override;
    /// @throws NumericalBreakdown if J_k is rank deficient
    Vector step(const Function &f, const Vector &x) override;

private:
    Vector x0_;
    std::unique_ptr<JacobianStrategy> jacobian_;
};

#endif // PCSCPROJECT_NEWTON_HPP
