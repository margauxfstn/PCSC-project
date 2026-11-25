/**
 * @file FunctionLibrary.cpp
 * @brief Built-in test functions.
 */

#include "FunctionLibrary.hpp"

#include <cmath>
#include <utility>

#include "Exceptions.hpp"

namespace {

/// Helper: maker of a scalar function from two double -> double callables.
FunctionLibrary::Maker scalar(LambdaFunction::ScalarFn f, LambdaFunction::ScalarFn df) {
    return [f, df]() { return std::make_unique<LambdaFunction>(LambdaFunction::scalar(f, df)); };
}

} // namespace

FunctionLibrary::FunctionLibrary() {
    // --- scalar problems f(x) = 0 -------------------------------------
    // root sqrt(2)
    add("x2_minus_2", scalar([](double x) { return x * x - 2.0; },
                             [](double x) { return 2.0 * x; }));
    // root ~ 0.7390851332
    add("cos_minus_x", scalar([](double x) { return std::cos(x) - x; },
                              [](double x) { return -std::sin(x) - 1.0; }));
    // root ~ 1.5213797068
    add("cubic", scalar([](double x) { return x * x * x - x - 2.0; },
                        [](double x) { return 3.0 * x * x - 1.0; }));
    // double root at 1: Newton only converges linearly
    add("double_root", scalar([](double x) { return (x - 1.0) * (x - 1.0); },
                              [](double x) { return 2.0 * (x - 1.0); }));
    // same cubic without derivative: needs finite differences
    add("cubic_no_derivative", scalar([](double x) { return x * x * x - x - 2.0; }, nullptr));

    // --- iteration functions phi for the fixed-point method -------------
    // fixed point of cos = root of cos_minus_x
    add("cos", scalar([](double x) { return std::cos(x); },
                      [](double x) { return -std::sin(x); }));
    // fixed point = root of cubic: x = (x + 2)^(1/3)
    add("cubic_phi", scalar([](double x) { return std::cbrt(x + 2.0); }, nullptr));

    // --- systems R^2 -> R^2 ---------------------------------------------
    // x^2 + y^2 = 4 and x = y: root (sqrt 2, sqrt 2)
    add("circle_line", []() {
        return std::make_unique<LambdaFunction>(
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
    });
}

void FunctionLibrary::add(const std::string &name, Maker maker) {
    makers_[name] = std::move(maker);
}

std::unique_ptr<Function> FunctionLibrary::create(const std::string &name) const {
    const auto it = makers_.find(name);
    if (it == makers_.end()) {
        std::string known;
        for (const auto &n : names()) {
            known += " " + n;
        }
        throw ConfigError("unknown function '" + name + "'. Available:" + known);
    }
    return it->second();
}

std::vector<std::string> FunctionLibrary::names() const {
    std::vector<std::string> result;
    for (const auto &entry : makers_) {
        result.push_back(entry.first);
    }
    return result;
}
