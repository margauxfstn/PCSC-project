/**
 * @file test_newton.cpp
 * @brief Tests of Newton, modified Newton and chord (Newton + strategies).
 */

#include <cmath>
#include <memory>

#include "AnalyticJacobian.hpp"
#include "ChordSlope.hpp"
#include "Exceptions.hpp"
#include "FiniteDifferenceJacobian.hpp"
#include "FrozenJacobian.hpp"
#include "Newton.hpp"
#include "TestUtils.hpp"

namespace {
Vector scalarVec(double x) { return Vector::Constant(1, x); }
} // namespace

int main() {
    const Vector sqrt2 = scalarVec(std::sqrt(2.0));
    const auto f = LambdaFunction::scalar([](double x) { return x * x - 2.0; },
                                          [](double x) { return 2.0 * x; });

    // Newton, analytic derivative: correct root, quadratic convergence.
    {
        Newton newton(scalarVec(1.0), std::make_unique<AnalyticJacobian>(), 1e-14, 50);
        const Result r = newton.solve(f);
        CHECK(r.converged());
        CHECK_NEAR(r.root(0), sqrt2(0), 1e-14);
        CHECK(r.iterations <= 7);
        CHECK_NEAR(estimateOrder(r.history, sqrt2), 2.0, 0.2);
    }

    // Newton with finite differences: same root without a derivative.
    {
        const auto g = LambdaFunction::scalar([](double x) { return x * x - 2.0; });
        Newton newton(scalarVec(1.0), std::make_unique<FiniteDifferenceJacobian>(), 1e-12, 50);
        const Result r = newton.solve(g);
        CHECK(r.converged());
        CHECK_NEAR(r.root(0), sqrt2(0), 1e-12);
        // ...but asking for the analytic derivative is an error.
        Newton analytic(scalarVec(1.0), std::make_unique<AnalyticJacobian>(), 1e-12, 50);
        CHECK_THROWS(analytic.solve(g), DerivativeError);
    }

    // Modified Newton (frozen derivative): converges, linearly.
    {
        Newton modified(scalarVec(1.5),
                        std::make_unique<FrozenJacobian>(std::make_unique<AnalyticJacobian>()),
                        1e-14, 200);
        const Result r = modified.solve(f);
        CHECK(r.converged());
        CHECK_NEAR(r.root(0), sqrt2(0), 1e-13);
        CHECK_NEAR(estimateOrder(r.history, sqrt2), 1.0, 0.2);
        // A second solve() recomputes the frozen derivative (reset()).
        CHECK(modified.solve(f).iterations == r.iterations);
    }

    // Chord method: constant slope over [1, 2], linear convergence.
    {
        Newton chord(scalarVec(1.0), std::make_unique<ChordSlope>(1.0, 2.0), 1e-14, 200);
        const Result r = chord.solve(f);
        CHECK(r.converged());
        CHECK_NEAR(r.root(0), sqrt2(0), 1e-13);
        CHECK_NEAR(estimateOrder(r.history, sqrt2), 1.0, 0.2);
        // Zero slope: breakdown reported in the result.
        const auto even = LambdaFunction::scalar([](double x) { return x * x - 1.0; });
        Newton bad(scalarVec(0.5), std::make_unique<ChordSlope>(-2.0, 2.0), 1e-12, 50);
        CHECK(bad.solve(even).status == Status::Breakdown);
    }

    // Double root: Newton only converges linearly.
    {
        const auto d = LambdaFunction::scalar([](double x) { return (x - 1.0) * (x - 1.0); },
                                              [](double x) { return 2.0 * (x - 1.0); });
        Newton newton(scalarVec(2.0), std::make_unique<AnalyticJacobian>(), 1e-12, 100);
        const Result r = newton.solve(d);
        CHECK(r.converged());
        CHECK_NEAR(estimateOrder(r.history, scalarVec(1.0)), 1.0, 0.2);
    }

    // Zero derivative at the starting point: breakdown, not a crash.
    {
        Newton newton(scalarVec(0.0), std::make_unique<AnalyticJacobian>(), 1e-12, 50);
        const Result r = newton.solve(f);
        CHECK(r.status == Status::Breakdown);
        CHECK(!r.message.empty());
    }

    // 2x2 system: x^2 + y^2 = 4, x = y. Root (sqrt 2, sqrt 2).
    {
        const LambdaFunction sys(
            2, 2,
            [](const Vector &v) {
                Vector r(2);
                r << v(0) * v(0) + v(1) * v(1) - 4.0, v(0) - v(1);
                return r;
            },
            [](const Vector &v) {
                Matrix j(2, 2);
                j << 2.0 * v(0), 2.0 * v(1), 1.0, -1.0;
                return j;
            });
        Vector x0(2);
        x0 << 1.0, 2.0;
        const Vector root = Vector::Constant(2, std::sqrt(2.0));

        Newton newton(x0, std::make_unique<AnalyticJacobian>(), 1e-14, 50);
        const Result r = newton.solve(sys);
        CHECK(r.converged());
        CHECK((r.root - root).norm() < 1e-13);
        CHECK_NEAR(estimateOrder(r.history, root), 2.0, 0.3);

        Newton fd(x0, std::make_unique<FiniteDifferenceJacobian>(), 1e-12, 50);
        CHECK((fd.solve(sys).root - root).norm() < 1e-10);

        // Initial guess of the wrong size.
        Newton wrong(scalarVec(1.0), std::make_unique<AnalyticJacobian>(), 1e-12, 50);
        CHECK_THROWS(wrong.solve(sys), DimensionError);
    }

    // Finite differences approximate a known Jacobian.
    {
        FiniteDifferenceJacobian fdj;
        const Matrix j = fdj.compute(f, scalarVec(3.0));
        CHECK_NEAR(j(0, 0), 6.0, 1e-8);
    }

    // Divergence is detected: f(x) = atan(x) from far away.
    {
        const auto at = LambdaFunction::scalar([](double x) { return std::atan(x); },
                                               [](double x) { return 1.0 / (1.0 + x * x); });
        Newton newton(scalarVec(3.0), std::make_unique<AnalyticJacobian>(), 1e-12, 100);
        const Result r = newton.solve(at);
        CHECK(!r.converged());
    }

    return report("test_newton");
}
