/**
 * @file RootFinder.hpp
 * @brief Most general interface of a root finder.
 */

#ifndef PCSCPROJECT_ROOTFINDER_HPP
#define PCSCPROJECT_ROOTFINDER_HPP

#include <string>

#include "Function.hpp"
#include "Result.hpp"

/**
 * @brief Abstract root finder: "give me f, I return an approximation of a
 * root x such that f(x) = 0".
 *
 * Design: this interface says nothing about iterations or tolerances,
 * because not every root finder is iterative (e.g. the closed-form formula
 * for a quadratic equation). Iterative methods derive from
 * IterativeRootFinder.
 */
class RootFinder {
public:
    virtual ~RootFinder() = default;

    /// Searches a root of @p f.
    virtual Result solve(const Function &f) = 0;

    /// Name of the method, used in the output.
    virtual std::string name() const = 0;
};

#endif // PCSCPROJECT_ROOTFINDER_HPP
