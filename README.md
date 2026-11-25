# PCSC project 3: non-linear systems

Margaux Fustinoni, Jeanne Benoist — Programming Concepts in Scientific Computing, EPFL.

A small C++ library and program to solve **f(x) = 0** for f : R^n -> R^m.
It contains the bisection, Newton, chord and fixed-point methods, Aitken's
acceleration and the modified Newton method. Newton also works on systems of
equations.

## Compiling

Requirements: a C++20 compiler and CMake >= 3.18. Eigen 3 is used if it is
installed; otherwise CMake downloads Eigen 3.4.0 on the first configuration
(internet connection needed once).

```bash
cmake -S . -B build
cmake --build build
```

Run the tests:

```bash
cd build && ctest --output-on-failure
```

Documentation (needs Doxygen): `doxygen Doxyfile`, then open `docs/html/index.html`.

## Usage

```bash
./build/solver examples/newton.cfg
```

The program reads one input file of `key = value` lines (`#` starts a
comment). Everything is chosen at run time; there is no need to recompile.

| key | used by | meaning |
|---|---|---|
| `method` | all | `bisection`, `newton`, `modified_newton`, `chord`, `fixed_point`, `aitken` |
| `function` | all | name of f in the function library (see below) |
| `tol` | all | tolerance of the stopping criteria (default `1e-10`) |
| `max_iter` | all | maximum number of iterations (default `100`) |
| `a`, `b` | bisection, chord | interval |
| `x0` | all but bisection | initial guess; a vector is written `1, 2` |
| `jacobian` | newton, modified_newton | `analytic` (default) or `finite_difference` |
| `fd_step` | finite differences | relative step (default `6e-6`) |
| `phi` | fixed_point, aitken | iteration function (default: `x - relaxation * f(x)`) |
| `relaxation` | fixed_point, aitken | alpha in the default phi (default `1`) |
| `print_history` | all | `true` to print every iterate |
| `output` | all | write the result to this file instead of the terminal |

Available functions: `x2_minus_2`, `cos_minus_x`, `cubic`, `double_root`,
`cubic_no_derivative` (no derivative given), the iteration functions `cos` and
`cubic_phi`, and the 2x2 system `circle_line`. One example input file per
method is in `examples/`.

Example output:

```
method     : fixed point + Aitken
status     : converged
root       : 0.739085133215161
residual   : 0
iterations : 4
```

Exit code: `0` converged, `2` not converged, `1` error in the input.

## Program flow

```
main
 ├─ ConfigReader::read()       input file          -> Config
 ├─ SolverFactory::makeFunction() "function = ..."   -> Function (from FunctionLibrary)
 ├─ SolverFactory::makeSolver()   "method = ..."     -> RootFinder
 ├─ RootFinder::solve(f)                             -> Result
 └─ ResultWriter::write()      Result              -> terminal or file
```

## Architecture

```
RootFinder (abstract)                solve(f), name()
└── IterativeRootFinder (abstract)   loop, stopping criteria, failure detection
    ├── Bisection                    scalar functions only
    ├── Newton                       uses a JacobianStrategy
    └── FixedPoint                   uses phi (or x - alpha f)
        └── AitkenFixedPoint         only step() changes

JacobianStrategy (abstract)          how Newton gets J(x)
├── AnalyticJacobian                 J given by the Function       -> Newton
├── FiniteDifferenceJacobian         central differences           -> Newton without derivative
├── FrozenJacobian (decorator)       J(x0) reused                  -> modified Newton
└── ChordSlope                       (f(b) - f(a)) / (b - a)       -> chord method

Function (abstract)                  f : R^n -> R^m, optional jacobian()
└── LambdaFunction                   built from C++ lambdas

Result        root, residual, iterations, Status, message, history
Config / ConfigReader, FunctionLibrary, SolverFactory, ResultWriter
```

Main design choices:

- **Vectors everywhere** (`Eigen::VectorXd`): the scalar case is n = m = 1,
  so the same interface works for scalar equations and for systems.
- **`RootFinder` vs `IterativeRootFinder`**: not every root finder is
  iterative, so tolerance and iteration count are only in the second class.
  Its `solve()` is a *template method*, marked `final`: the loop is written
  once, and every method only implements `initialize()` and `step()`.
- **Strategy for derivatives**: Newton, modified Newton and chord differ
  only in how the derivative is obtained. Instead of one class per variant,
  `Newton` owns a `JacobianStrategy`. `FrozenJacobian` wraps any other
  strategy, so modified Newton works with analytic or finite-difference
  derivatives.
- **Derivative is optional**: methods that do not need it never call it.
  Asking for an analytic Jacobian that does not exist raises
  `DerivativeError`; finite differences are the fallback.
- **Errors vs failures**: a wrong input (bad interval, wrong size, unknown
  name) throws an exception from the `SolverException` family. A run that
  does not converge is a normal result: `Status` is `MaxIterationsReached`,
  `Diverged` (inf/NaN) or `Breakdown` (e.g. singular Jacobian, with a
  message).
- **Factory**: `SolverFactory` is the only code that knows the concrete
  classes. Adding a method means writing its class and adding one branch.
- The linear system of Newton is solved with Eigen's column-pivoting QR, and
  the Jacobian is never inverted. When m > n this gives the least-squares
  (Gauss-Newton) step.

## Tests

`ctest` runs four unit test executables and runs the solver on every file of
`examples/`.

- `test_bisection`: root of x^2 - 2, iteration count ~ log2((b-a)/tol),
  reuse of a solver, root at the midpoint, invalid interval, non-scalar
  function, too few iterations.
- `test_newton`: Newton with analytic and finite-difference derivatives,
  **order of convergence ~ 2** (estimated from the history), modified Newton
  and chord with **order ~ 1**, double root (order 1), zero derivative and
  zero chord slope (breakdown), divergence (atan), 2x2 system (order ~ 2),
  wrong dimensions, missing derivative.
- `test_fixed_point`: x = cos(x) (order ~ 1), Aitken (more than 4 times fewer
  iterations, order > 1.5), default relaxed phi, unstable relaxation,
  dimension checks.
- `test_config`: parsing (comments, vectors, booleans), malformed input,
  every method built by the factory solves the cubic, factory errors,
  output format.

## Limitations

- One root is searched; the result depends on the initial guess or interval.
  Multiple roots (e.g. with deflation) are not supported.
- Bisection and chord are scalar only (`DimensionError` otherwise).
- Aitken is applied component by component for systems, which is only a
  heuristic.
- Functions are C++ code selected by name: the input file cannot define a new
  formula.
- Finite differences use a fixed step; they are inaccurate when f is noisy and
  meaningless where f is not differentiable (e.g. |x| at 0).
- The stopping criterion `||x_k - x_{k-1}|| < tol` can stop too early on
  slowly converging methods (fixed point with a rate close to 1).
- The **chord method** follows Quarteroni & Saleri (constant slope over
  [a, b]). Some references call the frozen derivative at x0 the chord method;
  this variant is available as `modified_newton`.

## TODO and perspectives

- Read formulas from the input file and differentiate them symbolically
  (e.g. SymEngine), or accept a function given as a table of values.
- Secant method: it would need the two previous iterates, so the strategy
  needs a small state (a `SecantSlope` strategy).
- Halley's method (second derivative), and Broyden's quasi-Newton update for
  systems (a new `JacobianStrategy`).
- Finite differences with extrapolation (Richardson) for a more robust
  derivative.
- Root finders that are not iterative (closed-form quadratic) as direct
  subclasses of `RootFinder`.

## Use of LLMs

As required, parts generated by an LLM are indicated here and in the commit
messages.

- The class diagram drawing was made with the help of an LLM.
- The refactoring to vector functions, the classes `IterativeRootFinder`,
  `Newton`, the `JacobianStrategy` hierarchy, `FixedPoint`,
  `AitkenFixedPoint`, `Config`, `FunctionLibrary`, `SolverFactory`,
  `ResultWriter`, the tests, the `Doxyfile` and this README were generated
  with Claude (Anthropic), following our design and the feedback of the
  teaching team, then reviewed by us.
- The first version of `RootFinder`, `Bisection`, the config reader and the
  bisection test was written by us.
