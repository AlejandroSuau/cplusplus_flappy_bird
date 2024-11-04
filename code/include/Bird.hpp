#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "utils/SoundsManager.hpp"
#include "utils/TextureManager.hpp"
#include "utils/Vec2.hpp"
#include "utils/Animation.hpp"

#include "Constants.hpp"

#include <array>

class Bird {
public:
    Bird(SoundsManager& sounds_manager, TextureManager& texture_manager);

    void Init();

    void Jump();

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

    void ProcessCollisionWithScoreCheck();
    void ProcessCollisionWithFloor();
    void ProcessCollisionWithPipe();
    const SDL_FRect& GetHitBox() const;

    void Reset();

private:
    enum class EBirdState {
        OSCILLATING, FLYING, DYING, DEAD
    };

    static const std::array<std::string, 3> kTexturePaths;
    SoundsManager& sounds_manager_;
    TextureManager& texture_manager_;

    Animation<kTexturePaths.size(), kTexturePaths> flying_animation_;
    Vec2 starting_position_;
    EBirdState state_;
    SDL_FRect hit_box_;
    float oscillating_time_;
    float velocity_y_;
    
    Mix_Chunk* jump_sound_;
    Mix_Chunk* hit_sound_;
    Mix_Chunk* die_sound_;
    Mix_Chunk* point_sound_;

    bool IsDead() const;
    bool IsDying() const;

    void UpdateStateOscillating(float dt);
    void ApplyGravityForce(float dt);
    void AnimateFlying(float dt);

    void LoadSounds();
};
