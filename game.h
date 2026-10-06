#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "button.h"
#include "sub_menu.h"
#include "delete.h"
#include "timer.h"
#include "everything.h"

class Game
{
public:
    Game();
    ~Game();

    void init(int lv, SDL_Renderer* &render, int cnt);

    void handle(SDL_Event* e, SDL_Renderer* &render);
    void update(int xa, int ya, int xb, int yb, SDL_Renderer* &render);
    void render(SDL_Renderer* &render);
    void render_pause(SDL_Renderer* &render);
    void render_win(SDL_Renderer* &render);
    void render_lose(SDL_Renderer* &render);

    bool check_new(SDL_Event* e);
    bool check_pause(SDL_Event* e);
    bool check_menu(SDL_Event* e);

    bool check_pause_continue(SDL_Event* e);
    bool check_pause_main_menu(SDL_Event* e);
    bool check_win_continue(SDL_Event* e);
    bool check_win_main_menu(SDL_Event* e);
    bool check_lose_play(SDL_Event* e);
    bool check_lose_main_menu(SDL_Event* e);

    bool check_complete();
    bool check_x(int xa, int ya, int xb, int yb);
    bool check_y(int xa, int ya, int xb, int yb);
    bool find_way(int xa, int ya, int xb, int yb);
    bool check_possible();
    void change();

    int get_count();
    int get_time();
    void game_time_pause();
    void game_time_unpause();

private:
    coordinate first_move;
    int board[GAME_ROW][GAME_COLUMN];
    Button* icon_button[GAME_ROW][GAME_COLUMN];
    Button* new_button;
    Button* pause_button;
    Button* menu_button;
    Button* chance_button;
    Button* time_button[420];
    SDL_Texture* level_texture;
    Sub_Menu* sub_win_menu;
    Sub_Menu* sub_lose_menu;
    Sub_Menu* pause_menu;
    int level;
    int count_change;
    bool check_first_move;
    bool check_delete;
    bool check_no_delete;
    Mix_Chunk* sound_first_move;
    Mix_Chunk* sound_delete;
    Mix_Chunk* sound_no_delete;
    Timer* game_time;
    int in_game_time;
};

#endif // GAME_H_INCLUDED
