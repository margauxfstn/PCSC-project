/**
 * @file Result.cpp
 * @brief Implementation of the Result helpers.
 */

#include "Result.hpp"

std::string toString(Status status) {
    switch (status) {
    case Status::Converged:
        return "converged";
    case Status::MaxIterationsReached:
        return "maximum number of iterations reached";
    case Status::Diverged:
        return "diverged (non-finite values)";
    case Status::Breakdown:
        return "breakdown";
    }
    return "unknown";
}
