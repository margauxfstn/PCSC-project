/**
 * @file AitkenFixedPoint.hpp
 * @brief Fixed-point iteration accelerated by Aitken's delta-squared process.
 */

#ifndef PCSCPROJECT_AITKENFIXEDPOINT_HPP
#define PCSCPROJECT_AITKENFIXEDPOINT_HPP

#include "FixedPoint.hpp"

/**
 * @brief Aitken-accelerated fixed point (Steffensen's form).
 *
 * From x_k, compute y = phi(x_k), z = phi(y), then
 * x_{k+1} = x_k - (y - x_k)^2 / (z - 2y + x_k).
 * Each step costs two evaluations of phi. Turns a linearly convergent
 * fixed-point iteration into a (generally) quadratically convergent one.
 *
 * Design: derives from FixedPoint and reuses applyPhi(); only step()
 * changes. Limitation: for systems the formula is applied component by
 * component, which is a heuristic, not the true vector extrapolation.
 */
class AitkenFixedPoint : public FixedPoint {
public:
    using FixedPoint::FixedPoint; ///< same constructor as FixedPoint

    std::string name() const override { return "fixed point + Aitken"; }

protected:
    Vector step(const Function &f, const Vector &x) override;
};

#endif // PCSCPROJECT_AITKENFIXEDPOINT_HPP
