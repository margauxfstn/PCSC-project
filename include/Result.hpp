//
// Created by marga on 13/11/2026.
//

#ifndef PCSCPROJECT_RESULT_HPP
#define PCSCPROJECT_RESULT_HPP

#include <vector>

/// Outcome of a root-finding run.
struct Result {
    double root = 0.0;           ///< last approximation
    double residual = 0.0;       ///< |f(root)|
    int iterations = 0;          ///< number of iterations performed
    bool converged = false;      ///< true if a stopping criterion was met
    std::vector<double> history; ///< successive approximations
};

#endif //PCSCPROJECT_RESULT_HPP
