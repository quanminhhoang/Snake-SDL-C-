#include "Game.hpp"



Game::Game()
{
    // SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error("SDL could not initialize!");
    }

    // Window
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    SDL_Surface* icon = IMG_Load("snake.png");
    SDL_SetWindowIcon(window, icon);


    if (window == nullptr)
    {
        throw std::runtime_error("Window could not be created!");
    }

    //renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        throw std::runtime_error("Renderer could not be created!");
    }

    SDL_Init(SDL_INIT_AUDIO);
    Uint16 audio_format = AUDIO_S16SYS;
    if (Mix_OpenAudio(22050, audio_format, 2, 4096) != 0)
    {
        throw std::runtime_error("Audio could not be created!");

    }
    TTF_Init();
}

void Game::Run()
{
    
    
    for (int i = 0; i < GRID_WIDTH; ++i)
        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            grid[i][j] = Block::empty;
        }

    srand(static_cast<int>(time(0)));


    GameLoop(); 
    
}

void Game::SpawnFood()
{
    
    int x, y;
    while (true)
    {
        x = rand() % GRID_WIDTH;
        y = rand() % GRID_HEIGHT;

        if (grid[x][y] == Block::empty)
        {
            grid[x][y] = Block::food;
            food.x = x;
            food.y = y;
            break;
        }
    }
}

void Game::GameLoop()
{   
    playMusic("music.mp3");
    while ( running ==  false     ) {
        startmenu(renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {   
                running = false;
                Close();
                break;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_SPACE:
                    running = true;
                    break;
                case SDLK_ESCAPE:
                    running = false;
                    Close();
                    break;
                }
            }
        }
   }

   
    Uint32 before,after;
    int frame_time;
    SpawnFood();
   
    
    while (running)
    {
        before = SDL_GetTicks();

        PollEvents();
        Update();
        Render();
        SDL_RenderPresent(renderer);
        if (!alive) {
            gameover(renderer);
            SDL_Delay(4000);
            running = false;
        }
       
        after = SDL_GetTicks();
        frame_time = after - before;
        
        if (FRAME_RATE > frame_time)
        {
            SDL_Delay(FRAME_RATE - frame_time);
        }

    }
    Close();
}

void Game::PollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
       
        if (e.type == SDL_QUIT)
        {   
            running = false;
            Close();
            break;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
            case SDLK_SPACE:
                stop = true;
                while (stop) {
                    showmenu();
                }
        
                    break;
                
                case SDLK_UP:
                case SDLK_w:
                    if (last_dir != Move::down) {
                        vel.y = -speed;
                        vel.x = 0;
                        dir = Move::up;
                    }
                break;

                case SDLK_DOWN:
                case SDLK_s:
                    if (last_dir != Move::up) {
                        vel.y = speed;
                        vel.x = 0;
                        dir = Move::down;
                    }
                    break;

                case SDLK_LEFT:
                case SDLK_a:
                    if (last_dir != Move::right) {
                        dir = Move::left;
                        vel.x = -speed;
                        vel.y = 0;
                    }
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    if (last_dir != Move::left) {
                        dir = Move::right;
                        vel.x = speed;
                        vel.y = 0;
                    }
                    break;
            }
        }
    }
}


void Game::GrowBody()
{
    body.push_back(head);
    growing--;
    grid[head.x][head.y] = Block::body;
}

void Game::Update()
{
    if (!alive) {
        return;
    }
    pos.x += vel.x;
    pos.y += vel.y;
 
    // xuyen tuong
    if (pos.x < 0) pos.x = GRID_WIDTH - 1;
    else if (pos.x > GRID_WIDTH  ) pos.x = 0;

    if (pos.y < 0) pos.y = GRID_HEIGHT - 1;
    else if (pos.y > GRID_HEIGHT ) pos.y = 0;

    int new_x = static_cast<int>(pos.x);
    int new_y = static_cast<int>(pos.y);

    // dau di chuyen ?
    if (new_x != head.x || new_y != head.y)
    {
        last_dir = dir;

        // them co neu dang an
        if (growing > 0)
        {
            GrowBody();
        }
        else
        {
            // di chuyen nguoi
            SDL_Point free = head;
            std::vector<SDL_Point>::reverse_iterator rit = body.rbegin();
            for (; rit != body.rend(); ++rit)
            {
                grid[free.x][free.y] = Block::body;
                std::swap(*rit, free);
            }

            grid[free.x][free.y] = Block::empty;
        }

    }

    head.x = new_x;
    head.y = new_y;

    Block& next = grid[head.x][head.y];

    // kiem tra do an
    if (next == Block::food)
    {
        playWAV("eat.wav");
        SpawnFood();
        growing++;
        score++;
    }
    // toang
    else if (next == Block::body)
    {   
        Mix_PauseMusic();
        playWAV("gameover.wav");
        alive = false;
    }
    next = Block::head;
}


void Game::Render()
{
    SDL_Rect block;
    block.w = SCREEN_WIDTH / GRID_WIDTH;
    block.h = SCREEN_WIDTH / GRID_HEIGHT;

    // clear 
    SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(renderer);

    // ve do an
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.x * block.w;
    block.y = food.y * block.h;
    SDL_RenderFillRect(renderer, &block);

    // ve than ran trang
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point& point : body)
    {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(renderer, &block);
    }

    // ve dau ran 
    block.x = head.x * block.w;
    block.y = head.y * block.h;
    if (alive) { 
        SDL_SetRenderDrawColor(renderer, 0x00, 0x7A, 0xCC, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
       SDL_RenderFillRect(renderer, &block); 
}


void Game::playWAV(std::string link)
{   
    Mix_Chunk* chunk = NULL;
    chunk = Mix_LoadWAV(link.c_str());
    Mix_PlayChannel(-1, chunk, 0);
}


void Game::playMusic(std::string link) {
    Mix_Music* nhac = NULL;
    nhac = Mix_LoadMUS(link.c_str());
    Mix_PlayMusic(nhac, -1);
}

void Game::showmenu()
{
    SDL_Event e;
    menu(renderer);
    
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = false;
            Close();
            break;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_SPACE:
                stop = false;
                break;

            case SDLK_ESCAPE:
                running = false;
                Close();
                break;


            }
        }
    }

   
}

void Game::menu(SDL_Renderer* renderer) {
    
   
    TTF_Font* font = TTF_OpenFont("word.ttf", 23);
    SDL_Color color = { 157,188,212 };

    SDL_Surface* surface = TTF_RenderText_Blended(font, "(Space) Continue", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {3* SCREEN_WIDTH / 10,2* SCREEN_HEIGHT /10, 256, 64 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);

    SDL_Surface* surface2 = TTF_RenderText_Blended(font, "(Esc) Exit", color);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    rect = { 4*SCREEN_WIDTH / 10, 4*SCREEN_HEIGHT / 10, 128, 64 };
    SDL_RenderCopy(renderer, texture2, NULL, &rect);
    SDL_RenderPresent(renderer);
}

void Game::startmenu(SDL_Renderer* renderer) {
    
    TTF_Font* font = TTF_OpenFont("word.ttf", 23);
    SDL_Color color = { 157,188,212 };
   
    SDL_Surface* surface2 = TTF_RenderText_Solid(font, "Snake", color);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Rect rect = { 3*SCREEN_WIDTH / 10, 3*SCREEN_HEIGHT / 10, 256, 64 };
    SDL_RenderCopy(renderer, texture2, NULL, &rect);
    SDL_RenderPresent(renderer);
    
    font = TTF_OpenFont("word.ttf", 22);
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Play", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect = { 4 * SCREEN_WIDTH / 10, 5 * SCREEN_HEIGHT / 10, 128, 64 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);

}


void Game::gameover(SDL_Renderer* renderer) {
    
    TTF_Font* font = TTF_OpenFont("word.ttf", 23);
    SDL_Color color = { 157,188,212 };

    SDL_Surface* surface2 = TTF_RenderText_Blended(font, "Game Over", color);
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Rect rect = { 3 * SCREEN_WIDTH / 10, 2 * SCREEN_HEIGHT / 10, 256, 128 };
    SDL_RenderCopy(renderer, texture2, NULL, &rect);
    SDL_RenderPresent(renderer);
    
    
    std::string title = "Your Score: " + std::to_string(score) ;
    font = TTF_OpenFont("word.ttf", 20);
    SDL_Surface* surface = TTF_RenderText_Solid(font, title.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect = { 4 * SCREEN_WIDTH / 10, 4* SCREEN_HEIGHT / 10, 128, 96 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);

}


    
void Game::Close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_QUIT;
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    exit(0);
  }

    

    

