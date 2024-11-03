#include "PipesPairFactory.hpp"

#include "Constants.hpp"

PipesPairFactory::PipesPairFactory(TextureManager& texture_manager) 
    : texture_manager_(texture_manager) {}

std::unique_ptr<PipesPair> PipesPairFactory::CreatePipesPair() {
    const float x = static_cast<float>(kGameWidth);
    
    const int height_without_floor = kGameHeight - static_cast<int>(kFloorHeight);
    const int pipe_top_max_height = height_without_floor - 
                                    static_cast<int>(PipeParameters::kPipesPairGap) -
                                    static_cast<int>(PipeParameters::kMinimumHeight);
    const float pipe_height_top = static_cast<float>(random_generator_.Generate(
        static_cast<int>(PipeParameters::kMinimumHeight), pipe_top_max_height));

    const float pipe_height_bottom = static_cast<float>(height_without_floor) - pipe_height_top - PipeParameters::kPipesPairGap;
    
    return std::make_unique<PipesPair>(texture_manager_, x, pipe_height_top, pipe_height_bottom);
}

