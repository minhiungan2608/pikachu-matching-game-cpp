#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "texture_manager.h"
#include "everything.h"

class Button
{
public:
    Button();
    Button(SDL_Rect rect, const char* s, const char* r, SDL_Renderer* &render);
    ~Button();

    bool check_mouse(SDL_Event* e);
    void render_up(SDL_Renderer* &render);
    void render_down(SDL_Renderer* &render);

    void set_alpha(int x);

private:
    SDL_Rect button_rect;
    SDL_Texture* button_texture_up;
    SDL_Texture* button_texture_down;
};

#endif // BUTTON_H_INCLUDED
