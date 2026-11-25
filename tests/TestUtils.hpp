/**
 * @file TestUtils.hpp
 * @brief Minimal test helpers (checks and convergence-order estimate).
 */

#ifndef PCSCPROJECT_TESTUTILS_HPP
#define PCSCPROJECT_TESTUTILS_HPP

#include <cmath>
#include <iostream>
#include <vector>

#include "Types.hpp"

/// Number of failed checks in the current test executable.
inline int failures = 0;

/// Records a failure (with the line number) if @p cond is false.
#define CHECK(cond)                                                            \
    do {                                                                       \
        if (!(cond)) {                                                         \
            std::cerr << "FAILED: " #cond " (" __FILE__ ":" << __LINE__        \
                      << ")\n";                                                \
            ++failures;                                                        \
        }                                                                      \
    } while (0)

/// Checks |a - b| <= tol.
#define CHECK_NEAR(a, b, tol) CHECK(std::abs((a) - (b)) <= (tol))

/// Checks that @p statement throws an exception of type @p ExceptionType.
#define CHECK_THROWS(statement, ExceptionType)                                 \
    do {                                                                       \
        bool thrown_ = false;                                                  \
        try {                                                                  \
            statement;                                                         \
        } catch (const ExceptionType &) {                                      \
            thrown_ = true;                                                    \
        }                                                                      \
        CHECK(thrown_ && #statement " should throw " #ExceptionType);          \
    } while (0)

/// Exit code of a test executable: prints a summary and returns 0 on success.
inline int report(const char *name) {
    if (failures == 0) {
        std::cout << name << ": all checks passed\n";
        return 0;
    }
    std::cerr << name << ": " << failures << " check(s) failed\n";
    return 1;
}

/**
 * @brief Estimates the order of convergence p from the history of a run.
 *
 * With e_k = ||x_k - root||, p ~ log(e_{k+1}/e_k) / log(e_k/e_{k-1}),
 * computed on the last three errors above @p floor (smaller errors are
 * dominated by rounding). Returns NaN if fewer than three errors remain.
 */
inline double estimateOrder(const std::vector<Vector> &history, const Vector &root,
                            double floor = 1e-13) {
    std::vector<double> errors;
    for (const auto &x : history) {
        const double e = (x - root).norm();
        if (e > floor) {
            errors.push_back(e);
        }
    }
    const std::size_t n = errors.size();
    if (n < 3) {
        return NAN;
    }
    return std::log(errors[n - 1] / errors[n - 2]) / std::log(errors[n - 2] / errors[n - 3]);
}

#endif // PCSCPROJECT_TESTUTILS_HPP
