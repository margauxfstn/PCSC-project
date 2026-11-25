/**
 * @file AnalyticJacobian.hpp
 * @brief Jacobian provided by the user.
 */

#ifndef PCSCPROJECT_ANALYTICJACOBIAN_HPP
#define PCSCPROJECT_ANALYTICJACOBIAN_HPP

#include "JacobianStrategy.hpp"

/// Uses the analytical Jacobian of the function (Function::jacobian()).
class AnalyticJacobian : public JacobianStrategy {
public:
    /// @throws DerivativeError if @p f provides no Jacobian
    Matrix compute(const Function &f, const Vector &x) override;
    std::string name() const override { return "analytic Jacobian"; }
};

#endif // PCSCPROJECT_ANALYTICJACOBIAN_HPP
