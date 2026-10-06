#ifndef EVERYTHING_H_INCLUDED
#define EVERYTHING_H_INCLUDED

#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

const int GAME_ROW = 11;
const int GAME_COLUMN = 18;

const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS;

const SDL_Rect PLAY_RECT = {500, 450, 200, 100};
const SDL_Rect MAIN_MENU_RECT = {500, 600, 200, 100};

const SDL_Rect NEW_RECT = {1050, 50, 100, 50};
const SDL_Rect PAUSE_RECT = {1050, 125, 100, 50};
const SDL_Rect MENU_RECT = {1050, 200, 100, 50};

const SDL_Rect CHANCE_RECT = {1050, 400, 100, 75};

struct coordinate
{
    int x;
    int y;
};

#endif // EVERYTHING_H_INCLUDED
