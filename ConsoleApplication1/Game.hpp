#pragma once
#include <vector>
#include <cstdlib>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include <ctime>
#include <stdlib.h>
class Game
{

public:

    Game();
   
    void Run();
    
   
    void startmenu(SDL_Renderer* renderer);
    void GameLoop();
private:

    bool stop = true;
    bool choi = false;
    bool running = false;
    bool alive = true;
    int growing = 0;
    int score = 0;
    float speed = 0.35f;
    

    static const int FRAME_RATE = 1000 / 60;
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 640;
    static const int GRID_WIDTH = 32;
    static const int GRID_HEIGHT = 32;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    enum class Block { head, body, food, empty };
    enum class Move { up, down, left, right , still };
    Move last_dir = Move::still;
    Move dir = Move::still;

    struct { float x = GRID_WIDTH / 2, y = GRID_HEIGHT / 2; } pos;

    struct { float x = 0, y = 0; } vel;

    SDL_Point head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    SDL_Point food;
    std::vector<SDL_Point> body;

    Block grid[GRID_WIDTH][GRID_HEIGHT];

    void SpawnFood();
    void GrowBody();
    void playWAV(std::string link);
    void playMusic(std::string link);

    void Render();
    void Update();
    void PollEvents();
    void showmenu();
    void menu(SDL_Renderer* renderer);
    void gameover(SDL_Renderer* renderer);
    void restart();
    void Close();
};
