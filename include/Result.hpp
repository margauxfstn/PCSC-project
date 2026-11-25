/**
 * @file Result.hpp
 * @brief Outcome of a root-finding run.
 */

#ifndef PCSCPROJECT_RESULT_HPP
#define PCSCPROJECT_RESULT_HPP

#include <string>
#include <vector>

#include "Types.hpp"

/// How a run ended. A run that fails to converge is a valid outcome, not an
/// error, so it is reported here instead of throwing.
enum class Status {
    Converged,            ///< a stopping criterion was met
    MaxIterationsReached, ///< the iteration budget was exhausted
    Diverged,             ///< an iterate or f(iterate) became inf/NaN
    Breakdown             ///< the method could not continue (see message)
};

/// Human-readable name of a Status.
std::string toString(Status status);

/// Result of RootFinder::solve(). Assumes one root is searched.
struct Result {
    Vector root;                 ///< last approximation of the root
    double residual = 0.0;       ///< ||f(root)||
    int iterations = 0;          ///< number of iterations performed
    Status status = Status::MaxIterationsReached; ///< how the run ended
    std::string message;         ///< details when status is Breakdown
    std::vector<Vector> history; ///< x_0, x_1, ... (used for convergence studies)

    /// True if the run converged.
    bool converged() const { return status == Status::Converged; }
};

#endif // PCSCPROJECT_RESULT_HPP
