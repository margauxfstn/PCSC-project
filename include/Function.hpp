//
// Created by marga on 13/11/2026.
//

#ifndef PCSCPROJECT_FUNCTION_HPP
#define PCSCPROJECT_FUNCTION_HPP

#include <functional>
#include <utility>

#include "Exceptions.hpp"

/// Scalar real function f(x), with an optional analytical derivative.
class Function {
public:
    virtual ~Function() = default;

    /// Evaluates f at x.
    virtual double operator()(double x) const = 0;

    /// Evaluates f'(x). Throws DerivativeError if not provided.
    virtual double derivative(double /*x*/) const {
        throw DerivativeError("this function has no analytical derivative");
    }
};

/// Function built from lambdas / std::function.
class LambdaFunction : public Function {
public:
    using Fn = std::function<double(double)>;

    explicit LambdaFunction(Fn f, Fn df = nullptr)
        : f_(std::move(f)), df_(std::move(df)) {}

    double operator()(double x) const override { return f_(x); }

    double derivative(double x) const override {
        if (!df_) {
            return Function::derivative(x); // throws
        }
        return df_(x);
    }

private:
    Fn f_;
    Fn df_;
};

#endif //PCSCPROJECT_FUNCTION_HPP
