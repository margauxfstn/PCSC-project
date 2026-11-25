/**
 * @file Function.hpp
 * @brief Abstract function f : R^n -> R^m and a lambda-based implementation.
 */

#ifndef PCSCPROJECT_FUNCTION_HPP
#define PCSCPROJECT_FUNCTION_HPP

#include <functional>

#include "Types.hpp"

/**
 * @brief Abstract vector function f : R^n -> R^m.
 *
 * Only the evaluation is mandatory. The Jacobian is optional: methods that
 * do not need it (bisection, fixed point) never ask for it, and methods that
 * need it get it through a JacobianStrategy, which can either call
 * jacobian() or approximate it by finite differences.
 */
class Function {
public:
    /// @param inputDim  n, size of x
    /// @param outputDim m, size of f(x)
    /// @throws DimensionError if a dimension is not positive
    Function(int inputDim, int outputDim);

    /// Virtual destructor: required in any polymorphic base class, so that
    /// deleting a derived object through a Function pointer is correct.
    virtual ~Function() = default;

    /// Evaluates f(x). @p x must have size inputDim().
    virtual Vector operator()(const Vector &x) const = 0;

    /// True if jacobian() is implemented by this function.
    virtual bool hasJacobian() const { return false; }

    /// Analytical Jacobian J(x) (m x n matrix, J_ij = df_i/dx_j).
    /// @throws DerivativeError by default (no analytical Jacobian).
    virtual Matrix jacobian(const Vector &x) const;

    int inputDim() const { return inputDim_; }   ///< n
    int outputDim() const { return outputDim_; } ///< m
    /// True for a scalar function R -> R.
    bool isScalar() const { return inputDim_ == 1 && outputDim_ == 1; }

private:
    int inputDim_;
    int outputDim_;
};

/**
 * @brief Function defined by C++ callables (lambdas, std::function).
 *
 * Example (scalar case):
 * @code
 * auto f = LambdaFunction::scalar([](double x) { return x * x - 2.0; },
 *                                 [](double x) { return 2.0 * x; });
 * @endcode
 */
class LambdaFunction : public Function {
public:
    using Fn = std::function<Vector(const Vector &)>;    ///< x -> f(x)
    using JacobianFn = std::function<Matrix(const Vector &)>; ///< x -> J(x)
    using ScalarFn = std::function<double(double)>;      ///< x -> f(x) in R

    /// @param jacobian may be empty: then hasJacobian() is false.
    LambdaFunction(int inputDim, int outputDim, Fn f, JacobianFn jacobian = nullptr);

    /// Builds a scalar function R -> R from double -> double callables.
    /// @param df derivative, may be empty.
    static LambdaFunction scalar(ScalarFn f, ScalarFn df = nullptr);

    Vector operator()(const Vector &x) const override;
    bool hasJacobian() const override { return static_cast<bool>(jacobian_); }
    Matrix jacobian(const Vector &x) const override;

private:
    Fn f_;
    JacobianFn jacobian_;
};

#endif // PCSCPROJECT_FUNCTION_HPP
