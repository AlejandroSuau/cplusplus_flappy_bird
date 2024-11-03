#pragma once

#include "utils/SDLInitializer.hpp"
#include "utils/SDLImageInitializer.hpp"
#include "utils/SDLTTFInitializer.hpp"
#include "utils/CountdownTimer.hpp"

#include "Bird.hpp"
#include "Floor.hpp"
#include "ScoreManager.hpp"
#include "PipesPairFactory.hpp"
#include "PipesPair.hpp"

#include <memory>
#include <vector>

class Game {
public:
    Game();
    ~Game();

    void Run();

private:
    enum class EGameState {
        READY_TO_PLAY,
        PLAYING,
        FINISHING,
        FINISHED
    };

    std::unique_ptr<SDLInitializer> sdl_;
    std::unique_ptr<SDLImageInitializer> sdl_image_;
    std::unique_ptr<SDLTTFInitializer> sdl_ttf_;

    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;
    bool is_running_;

    // Update
    const Uint64 target_fps = 60;
    const Uint64 fixed_timestep = 1000 / target_fps;
    const Uint64 frame_delay = 1000 / target_fps; // Frame delay in milliseconds

    // Game Objects
    TextureManager texture_manager_;
    EGameState state_;
    SDL_Texture* background_texture_;
    SDL_Texture* tutorial_texture_;
    SDL_Texture* gameover_texture_;
    Bird bird_;
    Floor floor_;
    ScoreManager score_manager_;
    CountdownTimer pipes_spawn_timer_;
    PipesPairFactory pipes_pairs_factory_;
    std::vector<std::unique_ptr<PipesPair>> pipes_pairs_;

    void Init();

    void LoadTextures();

    void Update(float dt);
    void Render();
    void HandleEvents();
    void HandlePressedKeySpace();

    void SpawnPipesPair();
    void Reset();

    void RemoveOutOfScreenPipes();
    
    void SpawnPipesIfNeeded(float dt);
    void UpdatePipesPairs(float dt);

    void ProcessCheckPointCollisions();
    void ProcessPipeCollisions();
    void ProcessFloorCollisions();
};
