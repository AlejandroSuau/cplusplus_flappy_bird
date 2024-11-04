#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"

#include <string>

// Update
static const Uint64 kTargetFPS = 60;
static const Uint64 kFixedTimeStep = 1000 / kTargetFPS;
static const Uint64 kFrameDelay = 1000 / kTargetFPS; // Frame delay in milliseconds

// Game
static const std::string kAssetsFolderImages = "assets/images/";
static const std::string kAssetsFolderSounds = "assets/sounds/";

static const int kGameWidth = 336;
static const int kGameHeight = 480;
static const float kScrollingVelocityX = 100.f;
static const Uint64 kMillisecondsToSpawnPipe = 1500;
static const SDL_FRect kTextureRectBackground {0.f, 0.f, kGameWidth, kGameHeight};
static const SDL_FRect kTextureRectTutorial {75.f, 10.f, 185.f, 267.f};
static const SDL_FRect kTextureRectGameOver {85.f, 100.f, 192.f, 42.f};

// Floor
static const float kFloorHeight = 112.f;

// Bird
namespace BirdParameters {
static const Vec2 kStartingPosition {150.f, 240.f};
static const Vec2 kDimensions {34.f, 24.f};

static const Uint64 kAnimationSpeed = 200;

static const float kGravityForce = 1000.f;
static const float kJumpForceY = -300.f;
static const float kFloatingAmplitude = 25.f;
static const float kFloatingVelocity = 5.f;
}

// Score
namespace ScoreParamenters {
static const Vec2 kTextureDimensions {24.f, 36.f};
static const Vec2 kPosition {0.f, 25.f};
}

// Pipe
namespace PipeParameters {
static const float kWidth = 52.f;
static const float kMinimumHeight = 50.f;
static const SDL_Color kHitBoxColor {100, 163, 119, 255};
static const float kPipesPairGap = 105.f;
}