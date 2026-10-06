#ifndef CREATE_WINDOW_H_INCLUDED
#define CREATE_WINDOW_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "texture_manager.h"
#include "menu.h"
#include "game.h"
#include "sub_menu.h"
#include "button.h"
#include "everything.h"

class Create
{
public:
    Create();
    ~Create();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Window* &win, SDL_Renderer* &render);

    void handle(SDL_Renderer* &render);
    void render(SDL_Renderer* &ren);
    void clean(SDL_Window* &win, SDL_Renderer* &render);

    int get_game_state()
    {
        return state;
    }

    void play_soundtrack();

private:
    int state;
    int sub_state;
    Menu* menu;
    Game* game;
    Sub_Menu* winning_menu;
    int counting;
    Mix_Music* soundtrack;
};

#endif // CREATE_WINDOW_H_INCLUDED
