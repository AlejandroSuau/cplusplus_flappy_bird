#pragma once

#include "utils/RandomGenerator.hpp"

#include "PipesPair.hpp"

#include <memory>

class PipesPairFactory {
public:
    PipesPairFactory(TextureManager& texture_manager);

    std::unique_ptr<PipesPair> CreatePipesPair();

private:
    TextureManager& texture_manager_;
    RandomGenerator random_generator_;
};
