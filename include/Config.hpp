/**
 * @file Config.hpp
 * @brief Key/value parameters of a run, and the reader of input files.
 */

#ifndef PCSCPROJECT_CONFIG_HPP
#define PCSCPROJECT_CONFIG_HPP

#include <istream>
#include <map>
#include <string>

#include "Types.hpp"

/**
 * @brief Parameters of a run as "key = value" pairs, with typed getters.
 *
 * Every getter throws ConfigError when the key is missing or the value
 * cannot be converted; the overloads with a default value never complain
 * about a missing key.
 */
class Config {
public:
    /// Sets (or overwrites) a key.
    void set(const std::string &key, const std::string &value);
    /// True if the key is present.
    bool has(const std::string &key) const;

    const std::string &getString(const std::string &key) const;
    double getDouble(const std::string &key) const;
    double getDouble(const std::string &key, double defaultValue) const;
    int getInt(const std::string &key) const;
    int getInt(const std::string &key, int defaultValue) const;
    /// Accepts true/false, yes/no, 1/0.
    bool getBool(const std::string &key, bool defaultValue) const;
    /// Values separated by commas and/or spaces, e.g. "1.0, 2.5".
    Vector getVector(const std::string &key) const;

private:
    std::map<std::string, std::string> values_;
};

/**
 * @brief Reads a Config from a text file.
 *
 * Format: one "key = value" per line; empty lines and lines starting with
 * '#' are ignored.
 */
class ConfigReader {
public:
    /// @throws ConfigError if the file cannot be opened or a line is invalid
    Config read(const std::string &path) const;
    /// Same as read() on an already opened stream (handy for tests).
    Config parse(std::istream &in) const;
};

#endif // PCSCPROJECT_CONFIG_HPP
