/**
 * @file Function.cpp
 * @brief Implementation of Function and LambdaFunction.
 */

#include "Function.hpp"

#include <string>
#include <utility>

#include "Exceptions.hpp"

Function::Function(int inputDim, int outputDim)
    : inputDim_(inputDim), outputDim_(outputDim) {
    if (inputDim <= 0 || outputDim <= 0) {
        throw DimensionError("function dimensions must be positive");
    }
}

Matrix Function::jacobian(const Vector & /*x*/) const {
    throw DerivativeError("this function has no analytical Jacobian "
                          "(use finite differences instead)");
}

LambdaFunction::LambdaFunction(int inputDim, int outputDim, Fn f, JacobianFn jacobian)
    : Function(inputDim, outputDim), f_(std::move(f)), jacobian_(std::move(jacobian)) {}

LambdaFunction LambdaFunction::scalar(ScalarFn f, ScalarFn df) {
    // Wrap the double -> double callables into Vector -> Vector ones.
    Fn vf = [f](const Vector &x) { return Vector::Constant(1, f(x(0))); };
    JacobianFn vdf = nullptr;
    if (df) {
        vdf = [df](const Vector &x) { return Matrix::Constant(1, 1, df(x(0))); };
    }
    return LambdaFunction(1, 1, std::move(vf), std::move(vdf));
}

Vector LambdaFunction::operator()(const Vector &x) const {
    if (x.size() != inputDim()) {
        throw DimensionError("function expects an input of size " +
                             std::to_string(inputDim()) + ", got " +
                             std::to_string(x.size()));
    }
    return f_(x);
}

Matrix LambdaFunction::jacobian(const Vector &x) const {
    if (!jacobian_) {
        return Function::jacobian(x); // throws DerivativeError
    }
    return jacobian_(x);
}
