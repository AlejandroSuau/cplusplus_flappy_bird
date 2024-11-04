#include "PipesPair.hpp"

#include "utils/Collisions.hpp"

#include "Constants.hpp"

#include <iostream>

PipesPair::PipesPair(TextureManager& texture_manager, float x, float height_top, float height_bottom)
    : texture_manager_(texture_manager)
    , pipe_top_{x, 0.f, PipeParameters::kWidth, height_top}
    , pipe_bottom_{x, height_top + PipeParameters::kPipesPairGap, PipeParameters::kWidth, height_bottom}
    , pipe_texture_(nullptr)
    , score_check_hit_box_(std::make_unique<SDL_FRect>(
        pipe_top_.x + PipeParameters::kWidth / 2.f,
        height_top,
        1.f,
        PipeParameters::kPipesPairGap)) {
    LoadTextures();
}

void PipesPair::LoadTextures() {
    pipe_texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "pipe-green.png");
}

bool PipesPair::IsRectCollidingWithPipe(const SDL_FRect& rect) const {
    return (AreColliding(pipe_top_, rect) || AreColliding(pipe_bottom_, rect));
}

bool PipesPair::IsRectCollidingWithScoreCheck(const SDL_FRect& rect) const {
    return (score_check_hit_box_ && AreColliding(*score_check_hit_box_, rect));
}

void PipesPair::RemoveCheckPoint() {
    score_check_hit_box_ = nullptr;
}

bool PipesPair::CanBeDestroyed() const {
    return ((pipe_top_.x + pipe_top_.w) <= 0);
}

void PipesPair::Update(float dt) {
    const float dx = kScrollingVelocityX * dt;
    pipe_top_.x -= dx;
    pipe_bottom_.x -= dx;
    
    if (score_check_hit_box_) {
        score_check_hit_box_->x -= dx;
    }
}

void PipesPair::Render(SDL_Renderer& renderer) {
    SDL_Rect rt {
        0,
        0,
        static_cast<int>(pipe_top_.w),
        static_cast<int>(pipe_top_.h)};
    SDL_RenderCopyExF(
        &renderer,
        pipe_texture_,
        &rt,
        &pipe_top_,
        0.0,
        nullptr,
        SDL_FLIP_VERTICAL
    );

    SDL_Rect rb {
        0,
        0,
        static_cast<int>(pipe_bottom_.w),
        static_cast<int>(pipe_bottom_.h)};
    SDL_RenderCopyExF(
        &renderer,
        pipe_texture_,
        &rb,
        &pipe_bottom_,
        0.0,
        nullptr,
        SDL_FLIP_NONE
    );
}
