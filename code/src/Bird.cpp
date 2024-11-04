#include "Bird.hpp"

#include "Constants.hpp"

#include <cmath>

const std::array<std::string, 3> Bird::kTexturePaths {
    "bluebird-downflap.png",
    "bluebird-midflap.png",
    "bluebird-upflap.png"
};

Bird::Bird(SoundsManager& sounds_manager, TextureManager& texture_manager)
    : sounds_manager_(sounds_manager)
    , texture_manager_(texture_manager)
    , flying_animation_(texture_manager, BirdParameters::kAnimationSpeed, BirdParameters::kDimensions)
    , starting_position_(BirdParameters::kStartingPosition) 
    , state_(EBirdState::OSCILLATING) 
    , hit_box_{starting_position_.x, starting_position_.y, BirdParameters::kDimensions.x, BirdParameters::kDimensions.y}
    , oscillating_time_(0.f)
    , velocity_y_(0.f)
    , jump_sound_(nullptr)
    , hit_sound_(nullptr)
    , die_sound_(nullptr)
    , point_sound_(nullptr) {}

void Bird::Init() {
    flying_animation_.LoadTextures();
    flying_animation_.Start();
    LoadSounds();
}

void Bird::LoadSounds() {
    jump_sound_ = sounds_manager_.LoadSoundEffect(kAssetsFolderSounds + "flap.mp3");
    hit_sound_ = sounds_manager_.LoadSoundEffect(kAssetsFolderSounds + "hit.mp3");
    die_sound_ = sounds_manager_.LoadSoundEffect(kAssetsFolderSounds + "die.mp3");
    point_sound_ = sounds_manager_.LoadSoundEffect(kAssetsFolderSounds + "point.mp3");
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
    
    Mix_PlayChannel(-1, jump_sound_, 0);
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

bool Bird::IsDead() const {
    return (state_ == EBirdState::DEAD);
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
    if (!IsDying()) {
        Mix_PlayChannel(-1, hit_sound_, 0);
    }
    state_ = EBirdState::DEAD;
    hit_box_.y = kGameHeight - kFloorHeight - hit_box_.h;
}

bool Bird::IsDying() const {
    return (state_ == EBirdState::DYING);
}

void Bird::ProcessCollisionWithPipe() {
    state_ = EBirdState::DYING;
    Mix_PlayChannel(-1, hit_sound_, 0);
    Mix_PlayChannel(-1, die_sound_, 0);
}

void Bird::ProcessCollisionWithScoreCheck() {
    Mix_PlayChannel(-1, point_sound_, 0);
}

const SDL_FRect& Bird::GetHitBox() const {
    return hit_box_;
}

void Bird::Render(SDL_Renderer& renderer) {
    flying_animation_.Render(renderer, {hit_box_.x, hit_box_.y});
}
