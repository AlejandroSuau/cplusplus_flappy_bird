#pragma once

#include "utils/SDLInitializer.hpp"
#include "utils/SDLImageInitializer.hpp"
#include "utils/SDLTTFInitializer.hpp"
#include "utils/SDLMixerInitializer.hpp"
#include "utils/CountdownTimer.hpp"

#include "utils/TextureManager.hpp"
#include "utils/SoundsManager.hpp"

#include "Bird.hpp"
#include "Floor.hpp"
#include "UIManager.hpp"
#include "ScoreManager.hpp"
#include "PipesPairFactory.hpp"
#include "PipesPair.hpp"

#include <memory>
#include <vector>

class Game {
public:
    enum class EGameState {
        READY_TO_PLAY,
        PLAYING,
        FINISHING,
        FINISHED
    };

    Game();
    void Run();

    bool IsReadyToPlay() const;
    bool IsPlaying() const;
    bool IsFinishing() const;
    bool IsFinished() const;

private:
    // SDL Initializers
    std::unique_ptr<SDLInitializer> sdl_;
    std::unique_ptr<SDLImageInitializer> sdl_image_;
    std::unique_ptr<SDLTTFInitializer> sdl_ttf_;
    std::unique_ptr<SDLMixerInitializer> sdl_mixer_;

    // SDL window & render
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;
    bool is_running_;

    SoundsManager sounds_manager_;
    TextureManager texture_manager_;
    ScoreManager score_manager_;
    UIManager ui_manager_;
    EGameState state_;
    SDL_Texture* background_texture_;

    // Game Objects
    Bird bird_;
    Floor floor_;
    CountdownTimer pipes_spawn_timer_;
    PipesPairFactory pipes_pairs_factory_;
    std::vector<std::unique_ptr<PipesPair>> pipes_pairs_;

    void Init();

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
