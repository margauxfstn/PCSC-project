/**
 * @file Config.cpp
 * @brief Implementation of Config and ConfigReader.
 */

#include "Config.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <vector>

#include "Exceptions.hpp"

namespace {

std::string trim(const std::string &s) {
    const auto begin = s.find_first_not_of(" \t\r");
    if (begin == std::string::npos) {
        return "";
    }
    return s.substr(begin, s.find_last_not_of(" \t\r") - begin + 1);
}

/// Strict conversion: the whole string must be a number.
double toDouble(const std::string &text, const std::string &key) {
    try {
        std::size_t used = 0;
        const double value = std::stod(text, &used);
        if (used == text.size()) {
            return value;
        }
    } catch (const std::exception &) {
        // handled below
    }
    throw ConfigError("value of '" + key + "' is not a number: '" + text + "'");
}

} // namespace

void Config::set(const std::string &key, const std::string &value) {
    values_[key] = value;
}

bool Config::has(const std::string &key) const {
    return values_.count(key) > 0;
}

const std::string &Config::getString(const std::string &key) const {
    const auto it = values_.find(key);
    if (it == values_.end()) {
        throw ConfigError("missing key in input file: '" + key + "'");
    }
    return it->second;
}

double Config::getDouble(const std::string &key) const {
    return toDouble(getString(key), key);
}

double Config::getDouble(const std::string &key, double defaultValue) const {
    return has(key) ? getDouble(key) : defaultValue;
}

int Config::getInt(const std::string &key) const {
    const double value = getDouble(key);
    if (value != static_cast<int>(value)) {
        throw ConfigError("value of '" + key + "' must be an integer");
    }
    return static_cast<int>(value);
}

int Config::getInt(const std::string &key, int defaultValue) const {
    return has(key) ? getInt(key) : defaultValue;
}

bool Config::getBool(const std::string &key, bool defaultValue) const {
    if (!has(key)) {
        return defaultValue;
    }
    std::string v = getString(key);
    std::transform(v.begin(), v.end(), v.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (v == "true" || v == "yes" || v == "1") {
        return true;
    }
    if (v == "false" || v == "no" || v == "0") {
        return false;
    }
    throw ConfigError("value of '" + key + "' must be true or false");
}

Vector Config::getVector(const std::string &key) const {
    std::string text = getString(key);
    std::replace(text.begin(), text.end(), ',', ' ');
    std::istringstream stream(text);
    std::vector<double> values;
    std::string token;
    while (stream >> token) {
        values.push_back(toDouble(token, key));
    }
    if (values.empty()) {
        throw ConfigError("value of '" + key + "' is empty");
    }
    return Eigen::Map<Vector>(values.data(), static_cast<Eigen::Index>(values.size()));
}

Config ConfigReader::read(const std::string &path) const {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw ConfigError("cannot open input file: " + path);
    }
    return parse(in);
}

Config ConfigReader::parse(std::istream &in) const {
    Config cfg;
    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line)) {
        ++lineNumber;
        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }
        const auto pos = line.find('=');
        if (pos == std::string::npos || trim(line.substr(0, pos)).empty()) {
            throw ConfigError("line " + std::to_string(lineNumber) +
                              ": expected 'key = value'");
        }
        cfg.set(trim(line.substr(0, pos)), trim(line.substr(pos + 1)));
    }
    return cfg;
}
