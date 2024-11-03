#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"

#include <memory>

class PipesPair {
public:
    PipesPair(
        TextureManager& texture_manager,
        float x,
        float height_top,
        float height_bottom);

    void LoadTextures();

    bool IsRectCollidingWithScoreCheck(const SDL_FRect& rect) const;
    bool IsRectCollidingWithPipe(const SDL_FRect& rect) const;
    void RemoveCheckPoint();

    bool CanBeDestroyed() const;

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

private:
    TextureManager& texture_manager_;
    SDL_Texture* pipe_texture_;
    SDL_FRect pipe_top_;
    SDL_FRect pipe_bottom_;
    std::unique_ptr<SDL_FRect> score_check_hit_box_;
};
