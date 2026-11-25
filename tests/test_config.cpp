/**
 * @file test_config.cpp
 * @brief Tests of the input reading, the factory and the output.
 */

#include <sstream>

#include "Config.hpp"
#include "Exceptions.hpp"
#include "FunctionLibrary.hpp"
#include "ResultWriter.hpp"
#include "SolverFactory.hpp"
#include "TestUtils.hpp"

namespace {
Config parse(const std::string &text) {
    std::istringstream in(text);
    return ConfigReader().parse(in);
}
} // namespace

int main() {
    // Parsing: comments, spaces, typed values, vectors.
    const Config cfg = parse("# comment\n\n method = newton \nx0 = 1, 2.5 3\nn = 4\nflag = yes\n");
    CHECK(cfg.getString("method") == "newton");
    CHECK(cfg.getVector("x0").size() == 3);
    CHECK_NEAR(cfg.getVector("x0")(1), 2.5, 0.0);
    CHECK(cfg.getInt("n") == 4);
    CHECK(cfg.getBool("flag", false));
    CHECK_NEAR(cfg.getDouble("missing", 7.0), 7.0, 0.0);

    // Errors in the input.
    CHECK_THROWS(cfg.getString("missing"), ConfigError);
    CHECK_THROWS(parse("tol = 1e-3abc").getDouble("tol"), ConfigError);
    CHECK_THROWS(parse("n = 2.5").getInt("n"), ConfigError);
    CHECK_THROWS(parse("this line has no equal sign"), ConfigError);
    CHECK_THROWS(ConfigReader().read("does/not/exist.cfg"), ConfigError);

    // Factory: every method can be built and solves its problem.
    const FunctionLibrary library;
    const SolverFactory factory(library);
    const char *inputs[] = {
        "method = bisection\nfunction = cubic\na = 1\nb = 2",
        "method = newton\nfunction = cubic\nx0 = 2",
        "method = newton\nfunction = cubic_no_derivative\nx0 = 2\njacobian = finite_difference",
        "method = modified_newton\nfunction = cubic\nx0 = 1.5",
        "method = chord\nfunction = cubic\na = 1\nb = 2\nx0 = 1.5",
        "method = fixed_point\nfunction = cubic\nphi = cubic_phi\nx0 = 1",
        "method = aitken\nfunction = cubic\nphi = cubic_phi\nx0 = 1",
    };
    for (const char *text : inputs) {
        const Config c = parse(text);
        const Result r = factory.makeSolver(c)->solve(*factory.makeFunction(c));
        CHECK(r.converged());
        CHECK_NEAR(r.root(0), 1.5213797068045676, 1e-8);
    }

    // Factory errors.
    CHECK_THROWS(factory.makeSolver(parse("method = magic")), ConfigError);
    CHECK_THROWS(factory.makeSolver(parse("method = newton")), ConfigError); // no x0
    CHECK_THROWS(factory.makeSolver(parse("method = newton\nx0 = 1\njacobian = guess")),
                 ConfigError);
    CHECK_THROWS(factory.makeFunction(parse("function = unknown")), ConfigError);

    // Writer: prints the status and the method name.
    const Config c = parse("method = newton\nfunction = x2_minus_2\nx0 = 1");
    const auto solver = factory.makeSolver(c);
    const Result r = solver->solve(*factory.makeFunction(c));
    std::ostringstream out;
    ResultWriter(out).write(solver->name(), r, true);
    CHECK(out.str().find("converged") != std::string::npos);
    CHECK(out.str().find("Newton (analytic Jacobian)") != std::string::npos);
    CHECK(out.str().find("x_0 = 1") != std::string::npos);

    return report("test_config");
}
