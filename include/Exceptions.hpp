//
// Created by marga on 13/11/2026.
//

#ifndef PCSCPROJECT_EXCEPTIONS_HPP
#define PCSCPROJECT_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

/// Base class of all errors raised by the root-finding solvers.
class SolverException : public std::runtime_error {
public:
    explicit SolverException(const std::string &msg) : std::runtime_error(msg) {}
};

/// The bracketing interval [a,b] does not satisfy f(a)*f(b) <= 0.
class InvalidInterval : public SolverException {
public:
    explicit InvalidInterval(const std::string &msg) : SolverException(msg) {}
};

/// A derivative was requested but is not available or is zero.
class DerivativeError : public SolverException {
public:
    explicit DerivativeError(const std::string &msg) : SolverException(msg) {}
};

#endif //PCSCPROJECT_EXCEPTIONS_HPP
