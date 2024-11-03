#pragma once

#include <SDL2/SDL.h>

#include "utils/TextureManager.hpp"

#include "Constants.hpp"

class Floor {
public:
    Floor(TextureManager& texture_manager);

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

    bool IsCollidingWith(const SDL_FRect& other) const;

private:
    TextureManager& texture_manager_;
    SDL_Texture* texture_;
    SDL_FRect moving_rect1_;
    SDL_FRect moving_rect2_;
    SDL_FRect hit_box_;

    void LoadTextures();
};
