#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include "Bisection.hpp"
#include "Function.hpp"

using Config = std::map<std::string, std::string>;

static std::string trim(const std::string &s) {
  const auto b = s.find_first_not_of(" \t\r");
  if (b == std::string::npos) {
    return "";
  }
  return s.substr(b, s.find_last_not_of(" \t\r") - b + 1);
}

static Config readConfig(const std::string &path) {
  std::ifstream in(path);
  if (!in.is_open()) {
    throw std::runtime_error("cannot open config file: " + path);
  }
  Config cfg;
  std::string line;
  while (std::getline(in, line)) {
    const auto pos = line.find('=');
    if (line.empty() || line[0] == '#' || pos == std::string::npos) {
      continue;
    }
    cfg[trim(line.substr(0, pos))] = trim(line.substr(pos + 1));
  }
  return cfg;
}

static const std::string &get(const Config &cfg, const std::string &key) {
  const auto it = cfg.find(key);
  if (it == cfg.end()) {
    throw std::runtime_error("missing key in config: " + key);
  }
  return it->second;
}

static std::unique_ptr<Function> makeFunction(const std::string &name) {
  if (name == "x2_minus_2") {
    return std::make_unique<LambdaFunction>(
        [](double x) { return x * x - 2.0; }, [](double x) { return 2.0 * x; });
  }
  if (name == "cos_minus_x") {
    return std::make_unique<LambdaFunction>(
        [](double x) { return std::cos(x) - x; },
        [](double x) { return -std::sin(x) - 1.0; });
  }
  throw std::runtime_error("unknown function: " + name);
}

static std::unique_ptr<RootFinder> makeSolver(const Config &cfg) {
  const std::string &method = get(cfg, "method");
  const double tol = std::stod(get(cfg, "tol"));
  const int maxIter = std::stoi(get(cfg, "max_iter"));

  if (method == "bisection") {
    return std::make_unique<Bisection>(std::stod(get(cfg, "a")),
                                       std::stod(get(cfg, "b")), tol, maxIter);
  }
  throw std::runtime_error("unknown method: " + method);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <config_file>\n";
    return 1;
  }
  try {
    const Config cfg = readConfig(argv[1]);
    const auto f = makeFunction(get(cfg, "function"));
    const auto solver = makeSolver(cfg);

    const Result r = solver->solve(*f);
    std::cout.precision(15);
    std::cout << (r.converged ? "converged" : "NOT converged") << "\n"
              << "root       = " << r.root << "\n"
              << "residual   = " << r.residual << "\n"
              << "iterations = " << r.iterations << "\n";
    return r.converged ? 0 : 2;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}