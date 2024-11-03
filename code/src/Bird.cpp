#include "Bird.hpp"

#include "Constants.hpp"

#include <cmath>

const std::array<std::string, 3> Bird::kTexturePaths {
    "bluebird-downflap.png",
    "bluebird-midflap.png",
    "bluebird-upflap.png"
};

Bird::Bird(TextureManager& texture_manager)
    : texture_manager_(texture_manager)
    , flying_animation_(texture_manager, BirdParameters::kAnimationSpeed, BirdParameters::kDimensions)
    , starting_position_(BirdParameters::kStartingPosition) 
    , state_(EBirdState::OSCILLATING) 
    , hit_box_{starting_position_.x, starting_position_.y, BirdParameters::kDimensions.x, BirdParameters::kDimensions.y}
    , oscillating_time_(0.f)
    , velocity_y_(0.f) {}

void Bird::Init() {
    flying_animation_.LoadTextures();
    flying_animation_.Start();
}

void Bird::Reset() {
    state_ = EBirdState::OSCILLATING;
    flying_animation_.Start();
    hit_box_.x = starting_position_.x;
    hit_box_.y = starting_position_.y;
}

void Bird::Jump() {
    velocity_y_ = BirdParameters::kJumpForceY;
    state_ = EBirdState::FLYING;
}

void Bird::Update(float dt) {
    if (IsDead()) return;
    
    switch(state_) {
        case EBirdState::DYING:
            ApplyGravityForce(dt);
        break;
        case EBirdState::FLYING:
            ApplyGravityForce(dt);
            AnimateFlying(dt);
        break;
        case EBirdState::OSCILLATING:
            UpdateStateOscillating(dt);
            AnimateFlying(dt);
        break;
    }
}

void Bird::UpdateStateOscillating(float dt) {
    oscillating_time_ += dt;
    hit_box_.y = (
        starting_position_.y + 
        BirdParameters::kFloatingAmplitude * 
        std::sin(BirdParameters::kFloatingVelocity * oscillating_time_));
}

void Bird::ApplyGravityForce(float dt) {
    velocity_y_ += BirdParameters::kGravityForce * dt;
    hit_box_.y += velocity_y_ * dt;
}

void Bird::AnimateFlying(float dt) {
    flying_animation_.Update();
}

void Bird::ProcessCollisionWithFloor() {
    state_ = EBirdState::DEAD;
    hit_box_.y = kGameHeight - kFloorHeight - hit_box_.h;
}

void Bird::ProcessCollisionWithPipe() {
    state_ = EBirdState::DYING;
}

const SDL_FRect& Bird::GetHitBox() const {
    return hit_box_;
}

void Bird::Render(SDL_Renderer& renderer) {
    flying_animation_.Render(renderer, {hit_box_.x, hit_box_.y});
}


bool Bird::IsDead() const {
    return (state_ == EBirdState::DEAD);
}