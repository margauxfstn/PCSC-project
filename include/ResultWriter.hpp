/**
 * @file ResultWriter.hpp
 * @brief Prints the outcome of a run.
 */

#ifndef PCSCPROJECT_RESULTWRITER_HPP
#define PCSCPROJECT_RESULTWRITER_HPP

#include <ostream>
#include <string>

#include "Result.hpp"

/// Writes a Result in a human-readable form to any output stream
/// (std::cout, a file, a string stream in the tests).
class ResultWriter {
public:
    /// @param out       destination (must outlive the writer)
    /// @param precision number of significant digits
    explicit ResultWriter(std::ostream &out, int precision = 15);

    /// @param showHistory also print every iterate x_k
    void write(const std::string &methodName, const Result &result, bool showHistory) const;

private:
    std::ostream &out_;
    int precision_;
};

#endif // PCSCPROJECT_RESULTWRITER_HPP
