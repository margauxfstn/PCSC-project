/**
 * @file test_bisection.cpp
 * @brief Tests of the bisection method.
 */

#include <cmath>

#include "Bisection.hpp"
#include "Exceptions.hpp"
#include "TestUtils.hpp"

int main() {
    const auto f = LambdaFunction::scalar([](double x) { return x * x - 2.0; });

    // Known root sqrt(2), within the tolerance.
    Bisection solver(0.0, 2.0, 1e-10, 100);
    const Result r = solver.solve(f);
    CHECK(r.converged());
    CHECK_NEAR(r.root(0), std::sqrt(2.0), 1e-9);

    // The error bound halves at each step: about log2((b-a)/tol) iterations.
    CHECK(r.iterations <= 36);

    // The same solver object can be reused and gives the same result.
    const Result r2 = solver.solve(f);
    CHECK(r2.iterations == r.iterations);

    // Root exactly at the midpoint: detected at once.
    const auto g = LambdaFunction::scalar([](double x) { return x - 1.0; });
    CHECK(Bisection(0.0, 2.0, 1e-10, 100).solve(g).iterations == 0);

    // Invalid inputs.
    CHECK_THROWS(Bisection(2.0, 3.0, 1e-10, 100).solve(f), InvalidInterval); // no sign change
    CHECK_THROWS(Bisection(1.0, 1.0, 1e-10, 100), InvalidInterval);          // a == b
    const LambdaFunction f2(2, 2, [](const Vector &x) { return x; });
    CHECK_THROWS(Bisection(0.0, 1.0, 1e-10, 100).solve(f2), DimensionError);  // not scalar

    // Too few iterations: reported, not thrown.
    const Result few = Bisection(0.0, 2.0, 1e-14, 3).solve(f);
    CHECK(few.status == Status::MaxIterationsReached);
    CHECK(few.iterations == 3);

    return report("test_bisection");
}
