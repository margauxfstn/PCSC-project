/**
 * @file FixedPoint.hpp
 * @brief Fixed-point iteration x_{k+1} = phi(x_k).
 */

#ifndef PCSCPROJECT_FIXEDPOINT_HPP
#define PCSCPROJECT_FIXEDPOINT_HPP

#include <memory>

#include "IterativeRootFinder.hpp"

/**
 * @brief Fixed-point iteration x_{k+1} = phi(x_k).
 *
 * The problem is still f(x) = 0 (f is used by the stopping criterion); phi
 * is an iteration function whose fixed points are the roots of f. It is
 * given by the user, or defaults to the relaxed iteration
 * phi(x) = x - alpha f(x).
 *
 * Converges linearly if phi is a contraction near the root
 * (|phi'(root)| < 1 in the scalar case).
 */
class FixedPoint : public IterativeRootFinder {
public:
    /// @param x0         initial guess
    /// @param phi        iteration function R^n -> R^n (owned); nullptr to
    ///                   use phi(x) = x - relaxation * f(x)
    /// @param relaxation alpha, only used when @p phi is null
    FixedPoint(Vector x0, std::unique_ptr<Function> phi, double relaxation,
               double tol, int maxIter);

    std::string name() const override { return "fixed point"; }

protected:
    /// @throws DimensionError if f, phi and x0 have incompatible sizes
    Vector initialize(const Function &f) override;
    Vector step(const Function &f, const Vector &x) override;

    /// Evaluates phi(x). Protected so that derived methods (Aitken) can
    /// build on the plain fixed-point map.
    Vector applyPhi(const Function &f, const Vector &x) const;

private:
    Vector x0_;
    std::unique_ptr<Function> phi_;
    double relaxation_;
};

#endif // PCSCPROJECT_FIXEDPOINT_HPP
