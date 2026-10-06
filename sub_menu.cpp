#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "sub_menu.h"
#include "button.h"
#include "everything.h"

using namespace std;

Sub_Menu::Sub_Menu()
    : play_button(nullptr), main_menu_button(nullptr), sub_menu_texture(nullptr)
{}

Sub_Menu::Sub_Menu(const char* s, const char* r, SDL_Renderer* &render)
{
    sub_menu_texture = texture_manager::load_texture(s, render);

    play_button = new Button(PLAY_RECT, r, r, render);

    main_menu_button = new Button(MAIN_MENU_RECT, "data/pokemon_screen/main_menu_button.png", "data/pokemon_screen/main_menu_button.png", render);
}

Sub_Menu::~Sub_Menu()
{
    if(play_button != NULL)
    {
        delete(play_button);
        play_button = NULL;
    }

    if(main_menu_button != NULL)
    {
        delete(main_menu_button);
        main_menu_button = NULL;
    }

    if(sub_menu_texture != NULL)
    {
        SDL_DestroyTexture(sub_menu_texture);
        sub_menu_texture = NULL;
    }
}

bool Sub_Menu::play(SDL_Event* e)
{
    return play_button->check_mouse(e);
}

bool Sub_Menu::main_menu(SDL_Event* e)
{
    return main_menu_button->check_mouse(e);
}

void Sub_Menu::render(SDL_Renderer* &render)
{
    SDL_RenderCopy(render, sub_menu_texture, NULL, NULL);
    play_button->render_up(render);
    main_menu_button->render_up(render);
}
