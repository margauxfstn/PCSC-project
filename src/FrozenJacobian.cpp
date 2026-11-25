/**
 * @file FrozenJacobian.cpp
 * @brief Implementation of FrozenJacobian.
 */

#include "FrozenJacobian.hpp"

#include <stdexcept>
#include <utility>

FrozenJacobian::FrozenJacobian(std::unique_ptr<JacobianStrategy> inner)
    : inner_(std::move(inner)) {
    if (!inner_) {
        throw std::invalid_argument("FrozenJacobian needs an inner strategy");
    }
}

void FrozenJacobian::reset() {
    frozen_.reset();
    inner_->reset();
}

Matrix FrozenJacobian::compute(const Function &f, const Vector &x) {
    if (!frozen_) {
        frozen_ = inner_->compute(f, x);
    }
    return *frozen_;
}

std::string FrozenJacobian::name() const {
    return "frozen " + inner_->name();
}
