/**
 * @file main.cpp
 * @brief Central executable: reads an input file, solves f(x) = 0, prints the result.
 *
 * Usage: ./solver <input_file>
 *
 * Exit codes: 0 converged, 1 error (bad input...), 2 did not converge.
 */

#include <fstream>
#include <iostream>

#include "Config.hpp"
#include "FunctionLibrary.hpp"
#include "ResultWriter.hpp"
#include "SolverFactory.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }
    try {
        // 1. read the input   2. build the objects   3. solve   4. write
        const Config cfg = ConfigReader().read(argv[1]);
        const FunctionLibrary library;
        const SolverFactory factory(library);
        const auto f = factory.makeFunction(cfg);
        const auto solver = factory.makeSolver(cfg);

        const Result result = solver->solve(*f);

        const bool showHistory = cfg.getBool("print_history", false);
        if (cfg.has("output")) {
            std::ofstream file(cfg.getString("output"));
            if (!file) {
                std::cerr << "Error: cannot write to " << cfg.getString("output") << "\n";
                return 1;
            }
            ResultWriter(file).write(solver->name(), result, showHistory);
        } else {
            ResultWriter(std::cout).write(solver->name(), result, showHistory);
        }
        return result.converged() ? 0 : 2;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
