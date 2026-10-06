#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "texture_manager.h"
#include "button.h"
#include "everything.h"

using namespace std;

Button::Button()
{}

Button::Button(SDL_Rect rect, const char* s, const char* r, SDL_Renderer* &render)
{
    button_rect = rect;
    button_texture_up = texture_manager::load_texture(s, render);
    button_texture_down = texture_manager::load_texture(r, render);
}

Button::~Button()
{
    if(button_texture_up != NULL)
    {
        SDL_DestroyTexture(button_texture_up);
        button_texture_up = NULL;
    }

    if(button_texture_down != NULL)
    {
        SDL_DestroyTexture(button_texture_down);
        button_texture_down = NULL;
    }
}

bool Button::check_mouse(SDL_Event* e)
{
    bool inside = true;

    if(e->type ==  SDL_MOUSEBUTTONDOWN)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        if(x < button_rect.x)
        {
            inside = false;
        }

        else if(x > button_rect.x + button_rect.w)
        {
            inside = false;
        }

        else if(y < button_rect.y)
        {
            inside = false;
        }

        else if(y > button_rect.y + button_rect.h)
        {
            inside = false;
        }
    }

    return inside;
}

void Button::render_up(SDL_Renderer* &render)
{
    SDL_RenderCopy(render, button_texture_up, NULL, &button_rect);
}

void Button::render_down(SDL_Renderer* &render)
{
    SDL_RenderCopy(render, button_texture_down, NULL, &button_rect);
}

void Button::set_alpha(int x)
{
    SDL_SetTextureAlphaMod(button_texture_up, x);
}
