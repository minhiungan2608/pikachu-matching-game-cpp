#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "texture_manager.h"
#include "button.h"

using namespace std;

class Menu
{
public:
    Menu();
    Menu(SDL_Renderer* &render);
    ~Menu();

    bool handle(SDL_Event* e);
    void render(SDL_Renderer* &render);

private:
    Button* play_button;
    SDL_Texture* menu_texture;
};

#endif // MENU_H_INCLUDED
