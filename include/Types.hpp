/**
 * @file Types.hpp
 * @brief Linear-algebra types shared by the whole library.
 *
 * Design: every quantity is an Eigen vector, even in the scalar case (a
 * vector of size 1). A single interface then covers functions
 * f : R^n -> R^m, as asked by the teaching team, and the scalar methods are
 * the special case n = m = 1.
 */

#ifndef PCSCPROJECT_TYPES_HPP
#define PCSCPROJECT_TYPES_HPP

#include <Eigen/Dense>

/// Column vector of doubles (unknowns x, values f(x)).
using Vector = Eigen::VectorXd;

/// Dense matrix of doubles (Jacobian matrices).
using Matrix = Eigen::MatrixXd;

#endif // PCSCPROJECT_TYPES_HPP
