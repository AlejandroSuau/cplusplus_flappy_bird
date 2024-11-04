#include "Floor.hpp"

#include "utils/Collisions.hpp"

Floor::Floor(TextureManager& texture_manager)
    : texture_manager_(texture_manager)
    , texture_(nullptr)
    , hit_box_{0.f,
               kGameHeight - kFloorHeight,
               kGameWidth,
               kFloorHeight}
    , moving_rect1_{0.f,
                    kGameHeight - kFloorHeight,
                    kGameWidth,
                    kFloorHeight}
    , moving_rect2_{kGameWidth,
                    kGameHeight - kFloorHeight,
                    kGameWidth,
                    kFloorHeight} {
    moving_rect2_.x = hit_box_.w;
    LoadTextures();
}

void Floor::LoadTextures() {
    texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "base.png");
}

bool Floor::IsCollidingWith(const SDL_FRect& other) const {
    return AreColliding(hit_box_, other);
}

void Floor::Update(float dt) {
    moving_rect1_.x -= dt * kScrollingVelocityX;
    moving_rect2_.x -= dt * kScrollingVelocityX;
    
    if (moving_rect1_.x + moving_rect1_.w < 0) {
        moving_rect1_.x = kGameWidth;
    }

    if (moving_rect2_.x + moving_rect2_.w < 0) {
        moving_rect2_.x = kGameWidth;
    }
}

void Floor::Render(SDL_Renderer& renderer) {
    SDL_RenderDrawRectF(&renderer, &hit_box_);
    SDL_RenderDrawRectF(&renderer, &moving_rect1_);
    SDL_RenderDrawRectF(&renderer, &moving_rect2_);
    
    SDL_RenderCopyF(&renderer, texture_, nullptr, &moving_rect1_);
    SDL_RenderCopyF(&renderer, texture_, nullptr, &moving_rect2_);
}
