/**
 * @file ChordSlope.cpp
 * @brief Implementation of ChordSlope.
 */

#include "ChordSlope.hpp"

#include "Exceptions.hpp"

ChordSlope::ChordSlope(double a, double b) : a_(a), b_(b) {
    if (a == b) {
        throw InvalidInterval("chord method needs a != b");
    }
}

Matrix ChordSlope::compute(const Function &f, const Vector & /*x*/) {
    if (!f.isScalar()) {
        throw DimensionError("the chord method only applies to scalar functions");
    }
    if (!slope_) {
        const double fa = f(Vector::Constant(1, a_))(0);
        const double fb = f(Vector::Constant(1, b_))(0);
        if (fa == fb) {
            throw NumericalBreakdown("chord slope is zero (f(a) == f(b))");
        }
        slope_ = (fb - fa) / (b_ - a_);
    }
    return Matrix::Constant(1, 1, *slope_);
}
