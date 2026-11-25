/**
 * @file FunctionLibrary.hpp
 * @brief Catalogue of named functions selectable from the input file.
 */

#ifndef PCSCPROJECT_FUNCTIONLIBRARY_HPP
#define PCSCPROJECT_FUNCTIONLIBRARY_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Function.hpp"

/**
 * @brief Maps a name ("cos_minus_x", ...) to a function factory.
 *
 * Functions are C++ code, so the input file chooses among them by name; a
 * new function is added with add() (or in the constructor) without
 * touching any solver. Parsing formulas from the input file (e.g. with
 * SymEngine) would be another source of functions: see README.
 */
class FunctionLibrary {
public:
    /// Something that builds a new Function.
    using Maker = std::function<std::unique_ptr<Function>()>;

    /// Registers the built-in test functions.
    FunctionLibrary();

    /// Registers (or replaces) a function.
    void add(const std::string &name, Maker maker);

    /// @throws ConfigError if @p name is unknown
    std::unique_ptr<Function> create(const std::string &name) const;

    /// Names of all registered functions (sorted).
    std::vector<std::string> names() const;

private:
    std::map<std::string, Maker> makers_;
};

#endif // PCSCPROJECT_FUNCTIONLIBRARY_HPP
