/**
 * @file test_fixed_point.cpp
 * @brief Tests of the fixed-point iteration and of Aitken's acceleration.
 */

#include <cmath>
#include <memory>

#include "AitkenFixedPoint.hpp"
#include "Exceptions.hpp"
#include "FixedPoint.hpp"
#include "TestUtils.hpp"

namespace {
Vector scalarVec(double x) { return Vector::Constant(1, x); }
std::unique_ptr<Function> cosPhi() {
    return std::make_unique<LambdaFunction>(
        LambdaFunction::scalar([](double x) { return std::cos(x); }));
}
} // namespace

int main() {
    const auto f = LambdaFunction::scalar([](double x) { return std::cos(x) - x; });
    const Vector root = scalarVec(0.7390851332151607);

    // x = cos(x): linear convergence with rate |phi'(root)| = sin(root) ~ 0.67.
    FixedPoint fixedPoint(scalarVec(1.0), cosPhi(), 1.0, 1e-12, 200);
    const Result plain = fixedPoint.solve(f);
    CHECK(plain.converged());
    CHECK_NEAR(plain.root(0), root(0), 1e-11);
    CHECK_NEAR(estimateOrder(plain.history, root), 1.0, 0.2);

    // Aitken: same root, much fewer iterations, superlinear convergence.
    AitkenFixedPoint aitken(scalarVec(1.0), cosPhi(), 1.0, 1e-12, 50);
    const Result fast = aitken.solve(f);
    CHECK(fast.converged());
    CHECK_NEAR(fast.root(0), root(0), 1e-12);
    CHECK(fast.iterations * 4 < plain.iterations);
    CHECK(estimateOrder(fast.history, root) > 1.5);

    // Default phi(x) = x - alpha f(x) (no user phi).
    const auto g = LambdaFunction::scalar([](double x) { return x * x - 2.0; });
    FixedPoint relaxed(scalarVec(1.0), nullptr, 0.3, 1e-12, 500);
    const Result r = relaxed.solve(g);
    CHECK(r.converged());
    CHECK_NEAR(r.root(0), std::sqrt(2.0), 1e-10);

    // Too large a relaxation: phi is no contraction, the iteration fails.
    FixedPoint unstable(scalarVec(1.0), nullptr, 2.0, 1e-12, 500);
    CHECK(!unstable.solve(g).converged());

    // Dimension checks: non-square f, and phi of the wrong size.
    const LambdaFunction rect(2, 1, [](const Vector &x) { return Vector::Constant(1, x.sum()); });
    CHECK_THROWS(FixedPoint(Vector::Zero(2), nullptr, 1.0, 1e-12, 10).solve(rect), DimensionError);
    CHECK_THROWS(FixedPoint(scalarVec(1.0), std::make_unique<LambdaFunction>(
                                                2, 2, [](const Vector &x) { return x; }),
                            1.0, 1e-12, 10)
                     .solve(f),
                 DimensionError);

    return report("test_fixed_point");
}
