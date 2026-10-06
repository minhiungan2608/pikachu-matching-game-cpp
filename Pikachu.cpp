#include <iostream>
#include <exception>
#include <SDL.h>
#include "create_window.h"
#include "everything.h"

int main(int, char**)
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Create game;
    try
    {
        game.init("Pikachu Matching Game", SDL_WINDOWPOS_CENTERED,
                  SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
                  false, window, renderer);
        game.play_soundtrack();
        while(game.get_game_state() != 12)
        {
            const Uint32 frameStart = SDL_GetTicks();
            game.handle(renderer);
            game.render(renderer);
            const Uint32 frameTime = SDL_GetTicks() - frameStart;
            if(frameTime < FRAME_DELAY) SDL_Delay(FRAME_DELAY - frameTime);
        }
        game.clean(window, renderer);
        return 0;
    }
    catch(const std::exception& error)
    {
        std::cerr << "Unable to run game: " << error.what() << '\n'
                  << "Run from the repository root so data/ is available.\n";
        game.clean(window, renderer);
        return 1;
    }
}
