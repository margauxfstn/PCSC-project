/**
 * @file FrozenJacobian.hpp
 * @brief Jacobian computed once and then reused (modified Newton).
 */

#ifndef PCSCPROJECT_FROZENJACOBIAN_HPP
#define PCSCPROJECT_FROZENJACOBIAN_HPP

#include <memory>
#include <optional>

#include "JacobianStrategy.hpp"

/**
 * @brief Decorator: asks another strategy for J(x_0) at the first call,
 * then returns that same matrix until reset().
 *
 * Newton + FrozenJacobian is the modified Newton method. Each step is
 * cheaper (no new Jacobian) but convergence is only linear.
 *
 * Design: being a decorator, it works with any other strategy (analytic or
 * finite differences) without code duplication.
 */
class FrozenJacobian : public JacobianStrategy {
public:
    /// @param inner strategy used to compute the Jacobian once (owned)
    explicit FrozenJacobian(std::unique_ptr<JacobianStrategy> inner);

    void reset() override;
    Matrix compute(const Function &f, const Vector &x) override;
    std::string name() const override;

private:
    std::unique_ptr<JacobianStrategy> inner_;
    std::optional<Matrix> frozen_; ///< empty until the first compute()
};

#endif // PCSCPROJECT_FROZENJACOBIAN_HPP
