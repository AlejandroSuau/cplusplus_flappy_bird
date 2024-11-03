#include "Game.hpp"

#include "Constants.hpp"

#include <ranges>
#include <stdexcept>
#include <string>

Game::Game()
    : sdl_(std::make_unique<SDLInitializer>())
    , sdl_image_(std::make_unique<SDLImageInitializer>())
    , sdl_ttf_(std::make_unique<SDLTTFInitializer>())
    , window_(
        SDL_CreateWindow(
            "Flappy Bird",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            kGameWidth,
            kGameHeight,
            0),
        SDL_DestroyWindow)
    , renderer_(
        SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer)
    , is_running_(false)
    , texture_manager_(*renderer_.get())
    , state_(EGameState::READY_TO_PLAY)
    , background_texture_(nullptr)
    , tutorial_texture_(nullptr)
    , gameover_texture_(nullptr)
    , bird_(texture_manager_)
    , floor_(texture_manager_)
    , score_manager_(texture_manager_)
    , pipes_spawn_timer_(kMillisecondsToSpawnPipe)
    , pipes_pairs_factory_(texture_manager_) {

    if (!window_ || !renderer_) {
        throw std::runtime_error(
            std::string("Error creating the game") + SDL_GetError());
    }
    
    SDL_SetRenderDrawBlendMode(renderer_.get(), SDL_BLENDMODE_BLEND);
}

Game::~Game() {}

void Game::Run() {
    is_running_ = true;
    SDL_ShowWindow(window_.get());

    // Game Start
    Init();
    // END Game Start

    Uint64 previous_time = SDL_GetTicks64();
    Uint64 accumulated_time = 0;
    while (is_running_) {
        Uint64 current_time = SDL_GetTicks64();
        Uint64 frame_time = current_time - previous_time; // Delta time in milliseconds
        previous_time = current_time;
        accumulated_time += frame_time;

        HandleEvents();

        // Fixed Update Loop
        while (accumulated_time >= fixed_timestep) {
            Update(fixed_timestep / 1000.0f); // Pass in seconds as a float
            accumulated_time -= fixed_timestep;
        }

        Render();

        Uint64 frame_end = SDL_GetTicks64();
        Uint64 frame_duration = frame_end - current_time;
        if (frame_duration < frame_delay) {
            SDL_Delay(static_cast<Uint32>(frame_delay - frame_duration));
        }
    }
}

void Game::Init() {
    LoadTextures();
    bird_.Init();
}

void Game::LoadTextures() {
    background_texture_ = texture_manager_.LoadTexture(kAssetsFolder + "background-night.png");
    tutorial_texture_ = texture_manager_.LoadTexture(kAssetsFolder + "message.png");
    gameover_texture_ = texture_manager_.LoadTexture(kAssetsFolder + "gameover.png");
}

void Game::Update(float dt) {
    if (state_ == EGameState::FINISHED) return;

    bird_.Update(dt);
    
    ProcessCheckPointCollisions();
    ProcessPipeCollisions();
    ProcessFloorCollisions();

    // If floor.Stop()?
    bool should_move_floor = (state_ == EGameState::READY_TO_PLAY || state_ == EGameState::PLAYING);
    if (should_move_floor) {
        floor_.Update(dt);
    }

    if (state_ == EGameState::PLAYING) {
        SpawnPipesIfNeeded(dt);
        UpdatePipesPairs(dt);
        RemoveOutOfScreenPipes();
    }
}

void Game::SpawnPipesIfNeeded(float dt) {
    if (pipes_spawn_timer_.HasElapsed()) {
        SpawnPipesPair();
    }
}

void Game::UpdatePipesPairs(float dt) {
    for (auto& pipe_pair : pipes_pairs_) {
        pipe_pair->Update(dt);
    }
}

void Game::ProcessCheckPointCollisions() {
    auto bird_collides_with_checkpoint = [&](const auto& pipes_pair) {
        return pipes_pair->IsRectCollidingWithScoreCheck(bird_.GetHitBox());
    };
    for (auto& pipes : pipes_pairs_ | std::views::filter(bird_collides_with_checkpoint)) {
        pipes->RemoveCheckPoint();
        score_manager_.IncreaseScoreOneUnit();
    }
}

void Game::ProcessPipeCollisions() {
    auto bird_collides_with_pipe = [&](const auto& pipes_pair) {
        return pipes_pair->IsRectCollidingWithPipe(bird_.GetHitBox());
    };
    for (auto& pipes : pipes_pairs_ | std::views::filter(bird_collides_with_pipe)) {
        state_ = EGameState::FINISHING;
        bird_.ProcessCollisionWithPipe();
    }
}

void Game::ProcessFloorCollisions() {
    if (floor_.IsCollidingWith(bird_.GetHitBox())) {
        state_ = EGameState::FINISHED;
        bird_.ProcessCollisionWithFloor();
    }
}

void Game::RemoveOutOfScreenPipes() {
    auto should_remove_pipe_pair = [](const auto& pipe_pair) { return pipe_pair->CanBeDestroyed(); };
    pipes_pairs_.erase(
        std::remove_if(pipes_pairs_.begin(), pipes_pairs_.end(), should_remove_pipe_pair),
        pipes_pairs_.end()
    );
}

void Game::SpawnPipesPair() {
    pipes_pairs_.emplace_back(pipes_pairs_factory_.CreatePipesPair());
}

void Game::Render() {
    auto* renderer = renderer_.get();
    SDL_RenderClear(renderer);
    
    SDL_RenderCopyF(renderer, background_texture_, nullptr, &kTextureRectBackground);
    
    for (auto& pipe_pair : pipes_pairs_) {
        pipe_pair->Render(*renderer);
    }

    if (state_ == EGameState::READY_TO_PLAY) {
        SDL_RenderCopyF(renderer, tutorial_texture_, nullptr, &kTextureRectTutorial);
    } else {
        score_manager_.Render(*renderer);   
    }
    
    floor_.Render(*renderer);
    bird_.Render(*renderer);

    if (state_ == EGameState::FINISHED) {
        SDL_RenderCopyF(renderer, gameover_texture_, nullptr, &kTextureRectGameOver);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

void Game::HandleEvents() {
     SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            is_running_ = false;
        }

        const auto is_key_down = (event.type == SDL_KEYDOWN);
        const auto is_key_space = (event.key.keysym.scancode == SDL_SCANCODE_SPACE);
        if (is_key_down && is_key_space) {
            HandlePressedKeySpace();
        }
    }
}

void Game::HandlePressedKeySpace() {
    switch(state_) {
        case EGameState::FINISHED:
            Reset();
        break;
        case EGameState::READY_TO_PLAY:
            state_ = EGameState::PLAYING;
        case EGameState::PLAYING:
            bird_.Jump();
        break;
    }
}

void Game::Reset() {
    bird_.Reset();
    state_ = EGameState::READY_TO_PLAY;
    score_manager_.Reset();
    pipes_pairs_.clear();
}