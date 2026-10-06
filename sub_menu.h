#ifndef SUB_MENU_H_INCLUDED
#define SUB_MENU_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "button.h"
#include "everything.h"

class Sub_Menu
{
public:
    Sub_Menu();
    Sub_Menu(const char* s, const char* r, SDL_Renderer* &render);
    ~Sub_Menu();

    bool play(SDL_Event* e);
    bool main_menu(SDL_Event* e);
    void render(SDL_Renderer* &render);

private:
    Button* play_button;
    Button* main_menu_button;
    SDL_Texture* sub_menu_texture;
};

#endif // SUB_MENU_H_INCLUDED
