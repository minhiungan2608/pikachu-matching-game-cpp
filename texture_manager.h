#ifndef TEXTURE_MANAGER_H_INCLUDED
#define TEXTURE_MANAGER_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>

class texture_manager
{
public:
    static SDL_Texture* load_texture(const char* file_name, SDL_Renderer* &render);
};

#endif // TEXTURE_MANAGER_H_INCLUDED
