#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdexcept>
#include "texture_manager.h"

using namespace std;

SDL_Texture* texture_manager::load_texture(const char* file_name, SDL_Renderer* &render)
{
    SDL_Surface* nsurface = IMG_Load(file_name);
    if(!nsurface)
        throw std::runtime_error(std::string("Cannot load asset ") + file_name + ": " + IMG_GetError());
    SDL_Texture* ntexture = SDL_CreateTextureFromSurface(render, nsurface);
    SDL_FreeSurface(nsurface);
    if(!ntexture)
        throw std::runtime_error(std::string("Cannot create texture: ") + SDL_GetError());

    return ntexture;
}
