/**
 * @file FiniteDifferenceJacobian.hpp
 * @brief Jacobian approximated by central finite differences.
 */

#ifndef PCSCPROJECT_FINITEDIFFERENCEJACOBIAN_HPP
#define PCSCPROJECT_FINITEDIFFERENCEJACOBIAN_HPP

#include "JacobianStrategy.hpp"

/**
 * @brief Central finite differences, column by column:
 * J_{:,j} ~ (f(x + h_j e_j) - f(x - h_j e_j)) / (2 h_j),
 * with h_j = h * max(1, |x_j|) (relative step for large x_j).
 *
 * Fallback when no analytical Jacobian is available. Costs 2n evaluations
 * of f. Limitation: the error is O(h^2) plus rounding errors O(eps/h), and
 * the result is meaningless where f is not differentiable (e.g. |x| at 0).
 */
class FiniteDifferenceJacobian : public JacobianStrategy {
public:
    /// Default step ~ eps^(1/3), which balances truncation and rounding.
    static constexpr double defaultStep = 6.0e-6;

    /// @throws std::invalid_argument if @p step <= 0
    explicit FiniteDifferenceJacobian(double step = defaultStep);

    Matrix compute(const Function &f, const Vector &x) override;
    std::string name() const override { return "finite-difference Jacobian"; }

private:
    double step_;
};

#endif // PCSCPROJECT_FINITEDIFFERENCEJACOBIAN_HPP
