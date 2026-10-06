#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "create_window.h"
#include "menu.h"
#include "button.h"
#include "game.h"
#include "texture_manager.h"
#include "everything.h"

using namespace std;

SDL_Texture* menu_texture;

Create::Create()
{}

Create::~Create()
{}

void Create::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Window* &win, SDL_Renderer* &render)
{
    int flags = 0;

    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        cout << "Subsystems Initialised!..." << endl;

        win = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if(win)
        {
            cout << "Window Created!" << endl;
        }

        render = SDL_CreateRenderer(win, -1, 0);

        if(render)
        {
            SDL_SetRenderDrawColor(render, 255, 255, 255, 255);

            cout << "Renderer Created!" << endl;
        }

        menu = new Menu(render);

        game = new Game();

        winning_menu = new Sub_Menu("data/pokemon_screen/7-sub-win.png", "data/pokemon_screen/play_again_button.png", render);

        state = 0;
        sub_state = -1;
        counting = -1;
    }

    else
    {
        state = 12;
    }

    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) == 0 )
    {
        cout << "Audio And Video Initialised!" << endl;
    }

    else
    {
        state = 12;
    }

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == 0)
    {
        cout << "Mixer Initialised!" << endl;

        soundtrack = Mix_LoadMUS("data/audio/soundtrack.wav");
    }

    else
    {
        state = 12;
    }
}

void Create::handle(SDL_Renderer* &render)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    if(event.type == SDL_QUIT)
    {
        state = 12;
    }

    else if(event.type == SDL_MOUSEBUTTONDOWN)
    {
        if(state == 0)
        {
            if(menu->handle(&event))
            {
                state++;
                game->init(state, render, 0);
            }
        }

        else if(state != 0 && state != 8 && state != 9 && state != 10 && state != 11 && state != 12)
        {
            if(game->get_count() == -1)
            {
                state = 11;
            }

            if(game->check_new(&event))
            {
                state = 1;
                game->init(state, render, 0);
            }

            if(game->check_pause(&event))
            {
                game->game_time_pause();
                sub_state = state;
                state = 9;
            }

            if(game->check_menu(&event))
            {
                state = 0;
            }

            game->handle(&event, render);

            if(game->check_complete())
            {
                if(state != 7)
                {
                    counting = game->get_count();
                    sub_state = ++state;
                    state = 10;
                }

                else
                {
                    state = 8;
                }
            }

        }

        else if(state == 8)
        {
            if(winning_menu->play(&event))
            {
                state = 1;
                game->init(state, render, 0);
            }

            else
            {
                if(winning_menu->main_menu(&event))
                {
                    state = 0;
                }
            }
        }

        else if(state == 9)
        {
            if(game->check_pause_continue(&event))
            {
                game->game_time_unpause();
                state = sub_state;
                sub_state = -1;
            }

            else
            {
                if(game->check_pause_main_menu(&event))
                {
                    state = 0;
                    sub_state = -1;
                }
            }
        }

        else if(state == 10)
        {
            if(game->check_win_continue(&event))
            {
                state = sub_state;
                sub_state = -1;
                game->init(state, render, counting);
            }

            if(game->check_win_main_menu(&event))
            {
                state = 0;
                sub_state = -1;
            }
        }

        else if(state == 11)
        {
            if(game->check_lose_play(&event))
            {
                state = 1;
                sub_state = -1;
                game->init(state, render, 0);
            }

            if(game->check_lose_main_menu(&event))
            {
                state = 0;
                sub_state = -1;
            }
        }
    }

    if(state != 0 && state != 8 && state != 9 && state != 10 && state != 11 && state != 12)
    {
        if(game->get_time() == 0)
        {
            state = 11;
        }
    }
}

void Create::render(SDL_Renderer* &render)
{
    SDL_RenderClear(render);

    if(state == 0)
    {
        menu->render(render);
    }

    else if(state != 0 && state != 8 && state != 9 && state != 10 && state != 11 && state != 12)
    {
        game->render(render);
    }

    else if(state == 8)
    {
        winning_menu->render(render);
    }

    else if(state == 9)
    {
        game->render_pause(render);
    }

    else if(state == 10)
    {
        game->render_win(render);
    }

    else if(state == 11)
    {
        game->render_lose(render);
    }

    else return;
    SDL_RenderPresent(render);
}

void Create::play_soundtrack()
{
    Mix_PlayMusic(soundtrack, -1);
}

void Create::clean(SDL_Window* &win, SDL_Renderer* &render)
{
    SDL_DestroyTexture(menu_texture);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(render);
    SDL_Quit();
    cout << "Game Cleaned!" << endl;
}
