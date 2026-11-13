//
// Created by marga on 13/11/2026.
//

#include <cmath>
#include <iostream>

#include "Bisection.hpp"
#include "Exceptions.hpp"
#include "Function.hpp"

static int failures = 0;

#define CHECK(cond)                                                            \
  do {                                                                         \
    if (!(cond)) {                                                             \
      std::cerr << "FAILED: " #cond " (line " << __LINE__ << ")\n";            \
      ++failures;                                                              \
    }                                                                          \
  } while (0)

int main() {
    LambdaFunction f([](double x) { return x * x - 2.0; });

    // known root: sqrt(2)
    Bisection b(0.0, 2.0, 1e-10, 100);
    const Result r = b.solve(f);
    CHECK(r.converged);
    CHECK(std::abs(r.root - std::sqrt(2.0)) < 1e-9);

    // same solver object can be reused
    const Result r2 = b.solve(f);
    CHECK(r2.iterations == r.iterations);

    // invalid interval: no sign change
    bool thrown = false;
    try {
        Bisection bad(2.0, 3.0, 1e-10, 100);
        bad.solve(f);
    } catch (const InvalidInterval &) {
        thrown = true;
    }
    CHECK(thrown);

    // too few iterations: must report non-convergence
    Bisection few(0.0, 2.0, 1e-14, 3);
    CHECK(!few.solve(f).converged);

    return failures == 0 ? 0 : 1;
}