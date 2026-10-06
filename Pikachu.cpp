#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include "create_window.h"
#include "button.h"
#include "game.h"
#include "menu.h"
#include "sub_menu.h"
#include "everything.h"

using namespace std;
Create* nwindow = nullptr;

void play_game()
{
    SDL_Renderer* renderer;
    SDL_Window* window;

    Uint32 frameStart;
    int frameTime;

    nwindow = new Create();

    nwindow->init("PIKACHU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false, window, renderer);
    nwindow->play_soundtrack();

    while(nwindow->get_game_state() != 12)
    {
        frameStart = SDL_GetTicks();

        nwindow->handle(renderer);
        nwindow->render(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        if(FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    nwindow->clean(window, renderer);
}

int main(int argc, char* argv[])
{
    play_game();

    return 0;
}

