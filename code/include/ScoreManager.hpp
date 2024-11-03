#pragma once

#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"
#include "utils/Vec2.hpp"

#include <string>
#include <array>

class ScoreManager {
public:
    ScoreManager(TextureManager& texture_manager);

    void Reset();
    void IncreaseScoreOneUnit();
    void Render(SDL_Renderer& renderer);

private:
    TextureManager& texture_manager_;
    int score_;
    std::string score_string_;
    Vec2 position_;

    std::array<SDL_Texture*, 10> number_textures_;

    void UpdateScoreValue(int val);
    void LoadNumberTextures();
};
