/**
 * @file Exceptions.hpp
 * @brief Exception hierarchy of the root-finding library.
 *
 * Two kinds of errors are distinguished:
 *  - wrong use of the library (bad interval, wrong dimensions, missing
 *    derivative, bad input file): an exception reaches the caller;
 *  - numerical breakdown during the iterations (e.g. singular Jacobian):
 *    IterativeRootFinder::solve() catches it and reports it in the Result,
 *    because it is a legitimate outcome of a run, not a programming error.
 */

#ifndef PCSCPROJECT_EXCEPTIONS_HPP
#define PCSCPROJECT_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

/// Base class of all errors raised by the library.
class SolverException : public std::runtime_error {
public:
    explicit SolverException(const std::string &msg) : std::runtime_error(msg) {}
};

/// The bracketing interval [a,b] is invalid (a >= b or no sign change).
class InvalidInterval : public SolverException {
public:
    explicit InvalidInterval(const std::string &msg) : SolverException(msg) {}
};

/// A derivative (Jacobian) was requested but is not available.
class DerivativeError : public SolverException {
public:
    explicit DerivativeError(const std::string &msg) : SolverException(msg) {}
};

/// Sizes of vectors/functions do not match (e.g. bisection on R^2).
class DimensionError : public SolverException {
public:
    explicit DimensionError(const std::string &msg) : SolverException(msg) {}
};

/// The input file is malformed (missing key, bad number, unknown name).
class ConfigError : public SolverException {
public:
    explicit ConfigError(const std::string &msg) : SolverException(msg) {}
};

/// The iteration cannot continue (e.g. singular Jacobian, zero slope).
/// Caught by IterativeRootFinder::solve() and turned into Status::Breakdown.
class NumericalBreakdown : public SolverException {
public:
    explicit NumericalBreakdown(const std::string &msg) : SolverException(msg) {}
};

#endif // PCSCPROJECT_EXCEPTIONS_HPP
