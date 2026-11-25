/**
 * @file ChordSlope.hpp
 * @brief Constant slope of the chord method.
 */

#ifndef PCSCPROJECT_CHORDSLOPE_HPP
#define PCSCPROJECT_CHORDSLOPE_HPP

#include <optional>

#include "JacobianStrategy.hpp"

/**
 * @brief Constant slope q = (f(b) - f(a)) / (b - a) (chord method).
 *
 * Newton + ChordSlope gives the chord method as defined in Quarteroni,
 * Saleri, "Scientific Computing with MATLAB and Octave":
 * x_{k+1} = x_k - f(x_k) / q. The slope is a finite-difference
 * approximation of f' over the whole interval, computed once. Converges
 * linearly (if |1 - f'(root)/q| < 1).
 *
 * Limitation: scalar functions only.
 */
class ChordSlope : public JacobianStrategy {
public:
    /// @throws InvalidInterval if a == b
    ChordSlope(double a, double b);

    void reset() override { slope_.reset(); }
    /// @throws DimensionError for non-scalar f,
    ///         NumericalBreakdown if f(a) == f(b) (zero slope)
    Matrix compute(const Function &f, const Vector &x) override;
    std::string name() const override { return "chord slope"; }

private:
    double a_, b_;
    std::optional<double> slope_; ///< computed at the first call
};

#endif // PCSCPROJECT_CHORDSLOPE_HPP
