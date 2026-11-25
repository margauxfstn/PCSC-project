/**
 * @file SolverFactory.cpp
 * @brief Implementation of SolverFactory.
 */

#include "SolverFactory.hpp"

#include <utility>

#include "AitkenFixedPoint.hpp"
#include "AnalyticJacobian.hpp"
#include "Bisection.hpp"
#include "ChordSlope.hpp"
#include "Exceptions.hpp"
#include "FiniteDifferenceJacobian.hpp"
#include "FixedPoint.hpp"
#include "FrozenJacobian.hpp"
#include "Newton.hpp"

namespace {
constexpr double defaultTol = 1e-10;
constexpr int defaultMaxIter = 100;
} // namespace

SolverFactory::SolverFactory(const FunctionLibrary &library) : library_(library) {}

std::unique_ptr<Function> SolverFactory::makeFunction(const Config &cfg) const {
    return library_.create(cfg.getString("function"));
}

std::unique_ptr<JacobianStrategy> SolverFactory::makeJacobian(const Config &cfg) const {
    const std::string kind = cfg.has("jacobian") ? cfg.getString("jacobian") : "analytic";
    if (kind == "analytic") {
        return std::make_unique<AnalyticJacobian>();
    }
    if (kind == "finite_difference") {
        return std::make_unique<FiniteDifferenceJacobian>(
            cfg.getDouble("fd_step", FiniteDifferenceJacobian::defaultStep));
    }
    throw ConfigError("unknown jacobian '" + kind + "' (use analytic or finite_difference)");
}

std::unique_ptr<RootFinder> SolverFactory::makeSolver(const Config &cfg) const {
    const std::string &method = cfg.getString("method");
    const double tol = cfg.getDouble("tol", defaultTol);
    const int maxIter = cfg.getInt("max_iter", defaultMaxIter);

    if (method == "bisection") {
        return std::make_unique<Bisection>(cfg.getDouble("a"), cfg.getDouble("b"), tol, maxIter);
    }
    if (method == "newton") {
        return std::make_unique<Newton>(cfg.getVector("x0"), makeJacobian(cfg), tol, maxIter);
    }
    if (method == "modified_newton") {
        return std::make_unique<Newton>(
            cfg.getVector("x0"), std::make_unique<FrozenJacobian>(makeJacobian(cfg)), tol, maxIter);
    }
    if (method == "chord") {
        return std::make_unique<Newton>(
            cfg.getVector("x0"),
            std::make_unique<ChordSlope>(cfg.getDouble("a"), cfg.getDouble("b")), tol, maxIter);
    }
    if (method == "fixed_point" || method == "aitken") {
        // phi is optional: without it, phi(x) = x - relaxation * f(x)
        std::unique_ptr<Function> phi;
        if (cfg.has("phi")) {
            phi = library_.create(cfg.getString("phi"));
        }
        const double relaxation = cfg.getDouble("relaxation", 1.0);
        if (method == "aitken") {
            return std::make_unique<AitkenFixedPoint>(cfg.getVector("x0"), std::move(phi),
                                                      relaxation, tol, maxIter);
        }
        return std::make_unique<FixedPoint>(cfg.getVector("x0"), std::move(phi), relaxation, tol,
                                            maxIter);
    }
    throw ConfigError("unknown method '" + method +
                      "' (bisection, newton, modified_newton, chord, fixed_point, aitken)");
}
