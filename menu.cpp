#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "menu.h"
#include "texture_manager.h"
#include "everything.h"

using namespace std;

Menu::Menu()
    : play_button(nullptr), menu_texture(nullptr)
{}

Menu::Menu(SDL_Renderer* &render)
{
    menu_texture = texture_manager::load_texture("data/pokemon_screen/mewtwo.png", render);

    play_button = new Button(PLAY_RECT, "data/pokemon_screen/play_button.png", "data/pokemon_screen/play_button.png", render);
}

Menu::~Menu()
{
    delete play_button;
    SDL_DestroyTexture(menu_texture);
}

bool Menu::handle(SDL_Event* e)
{
    return play_button->check_mouse(e);
}

void Menu::render(SDL_Renderer* &render)
{
    SDL_RenderCopy(render, menu_texture, NULL, NULL);
    play_button->render_up(render);
}
