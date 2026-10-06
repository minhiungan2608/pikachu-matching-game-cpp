#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "game.h"
#include "texture_manager.h"
#include "sub_menu.h"
#include "delete.h"
#include "timer.h"
#include "everything.h"

using namespace std;

Game::Game()
{}

Game::~Game()
{}

void Game::init(int lv, SDL_Renderer* &render, int cnt)
{
    level = lv;

    srand(time(NULL));

    for(int i = 0; i < GAME_ROW; i++)
    {
        board[i][0] = 0;
        board[i][GAME_COLUMN - 1] = 0;
    }

    for(int i = 0; i < GAME_COLUMN; i++)
    {
        board[0][i] = 0;
        board[GAME_ROW - 1][i] = 0;
    }

    vector<coordinate> coor;
    vector<int> val;

    for(int i = 1; i < GAME_ROW - 1; i++)
    {
        for(int j = 1; j < GAME_COLUMN - 1; j++)
        {
            coor.push_back({i, j});
        }
    }

    for(int i = 1; i < 25; i++)
    {
        for(int j = 1; j < 7; j++)
        {
            val.push_back(i);
        }
    }

    random_shuffle(val.begin(), val.end());

    int n = val.size();
    for(int i = 0; i < n; i++)
    {
        board[coor[i].x][coor[i].y] = val[i];
    }

    for(int i = 1; i < GAME_ROW - 1; i++)
    {
        for(int j = 1; j < GAME_COLUMN - 1; j++)
        {
            stringstream s, r;
            s << "data/pokemon_icon/" << board[i][j] << "-up.png";
            r << "data/pokemon_icon/" << board[i][j] << "-down.png";

            SDL_Rect icon_rect = {184 + 52 * (j - 1), 140 + 52 * (i - 1), 52, 52};

            icon_button[i][j] = new Button(icon_rect, s.str().c_str(), r.str().c_str(), render);
        }
    }

    stringstream s, r, t, u;
    s << "data/pokemon_screen/" << level << ".png";
    r << "data/pokemon_screen/" << level << "-sub-win.png";
    t << "data/pokemon_screen/" << level << "-sub-lose.png";
    u << "data/pokemon_screen/" << level << "-pause.png";
    level_texture = texture_manager::load_texture(s.str().c_str(), render);

    sub_win_menu = new Sub_Menu(r.str().c_str(), "data/pokemon_screen/next_level_button.png", render);
    sub_lose_menu = new Sub_Menu(t.str().c_str(), "data/pokemon_screen/play_again_button.png", render);
    pause_menu = new Sub_Menu(u.str().c_str(), "data/pokemon_screen/continue_button.png", render);

    new_button = new Button(NEW_RECT, "data/pokemon_screen/new_button.png", "data/pokemon_screen/new_button.png", render);
    pause_button = new Button(PAUSE_RECT, "data/pokemon_screen/pause_button.png", "data/pokemon_screen/pause_button.png", render);
    menu_button = new Button(MENU_RECT, "data/pokemon_screen/menu_button.png", "data/pokemon_screen/menu_button.png", render);

    first_move.x = -1;
    first_move.y = -1;

    count_change = cnt + 1;

    stringstream v;
    v << "data/chance/" << count_change << "-chance.png";
    chance_button = new Button(CHANCE_RECT, v.str().c_str(), v.str().c_str(), render);

    for(int i = 0; i < 420; i++)
    {
        SDL_Rect rect = {180 + 2 * i, 50, 2, 30};
        time_button[i] = new Button(rect, "data/time/run_time.png", "data/time/run_time.png", render);
    }

    check_delete = false;
    check_no_delete = false;
    check_first_move = false;

    sound_first_move = Mix_LoadWAV("data/audio/first_move.wav");
    sound_delete = Mix_LoadWAV("data/audio/delete.wav");
    sound_no_delete = Mix_LoadWAV("data/audio/no_delete.wav");

    game_time = new Timer();
    game_time->start();

    in_game_time = 0;
}

void Game::handle(SDL_Event* event, SDL_Renderer* &render)
{
    int x, y;
    x = 1 + (event->button.y - 140) / 52;
    y = 1 + (event->button.x - 184) / 52;

    if(x < 0 || x >= GAME_ROW - 1 || y < 0 || y >= GAME_COLUMN - 1 || board[x][y] == 0) return;

    if(first_move.x == x && first_move.y == y)
    {
        first_move.x = -1;
        first_move.y = -1;
        return;
    }

    if(first_move.x == -1)
    {
        first_move.x = x;
        first_move.y = y;

        check_first_move = true;
    }

    else
    {
        if(find_way(first_move.x, first_move.y, x, y))
        {
            update(first_move.x, first_move.y, x, y, render);
            first_move.x = -1;
            first_move.y = -1;

            check_delete = true;
        }

        else
        {
            first_move.x = -1;
            first_move.y = -1;

            check_no_delete = true;
        }
    }
}

void Game::update(int xa, int ya, int xb, int yb, SDL_Renderer* &render)
{
    if(level == 1)
    {
        Game_Delete::lv1_delete(board, xa, ya, xb, yb);
    }

    else if(level == 2)
    {
        Game_Delete::lv2_delete(board, xa, ya, xb, yb);
    }

    else if(level == 3)
    {
        Game_Delete::lv3_delete(board, xa, ya, xb, yb);
    }

    else if(level == 4)
    {
        Game_Delete::lv4_delete(board, xa, ya, xb, yb);
    }

    else if(level == 5)
    {
        Game_Delete::lv5_delete(board, xa, ya, xb, yb);
    }

    else if(level == 6)
    {
        Game_Delete::lv6_delete(board, xa, ya, xb, yb);
    }

    else if(level == 7)
    {
        Game_Delete::lv7_delete(board, xa, ya, xb, yb);
    }

    if(!check_complete() && !check_possible())
    {
        count_change--;

        stringstream s;
        s << "data/chance/" << count_change << "-chance.png";
        chance_button = new Button(CHANCE_RECT, s.str().c_str(), s.str().c_str(), render);

        while(!check_possible())
        {
            change();
        }
    }

    for(int i = 0; i < GAME_ROW ; i++)
    {
        for(int j = 0; j < GAME_COLUMN; j++)
        {
            if(board[i][j] == 0)
            {
                continue;
            }

            else
            {
                stringstream s, r;
                s << "data/pokemon_icon/" << board[i][j] << "-up.png";
                r << "data/pokemon_icon/" << board[i][j] << "-down.png";

                SDL_Rect icon_rect = {184 + 52 * (j - 1), 140 + 52 * (i - 1), 52, 52};

                icon_button[i][j] = new Button(icon_rect, s.str().c_str(), r.str().c_str(), render);
            }
        }
    }
}

void Game::render(SDL_Renderer* &render)
{
    SDL_RenderCopy(render, level_texture, NULL, NULL);

    if(check_first_move)
    {
        Mix_PlayChannel(-1, sound_first_move, 0);
        check_first_move = false;
    }

    else
    {
        if(check_delete)
        {
            Mix_PlayChannel(-1, sound_delete, 0);
            check_delete = false;
        }

        else
        {
            if(check_no_delete)
            {
                Mix_PlayChannel(-1, sound_no_delete, 0);
                check_no_delete = false;
            }
        }
    }

    new_button->render_up(render);
    pause_button->render_up(render);
    menu_button->render_up(render);

    chance_button->set_alpha(200);
    chance_button->render_up(render);

    in_game_time = (game_time->getTicks() / 1000.f);

    for(int i = 0; i < 420 - in_game_time; i++)
    {
        time_button[i]->render_up(render);
    }

    for(int i = 1; i < GAME_ROW - 1; i++)
    {
        for(int j = 1; j < GAME_COLUMN - 1; j++)
        {
            if(board[i][j] != 0)
            {
                if(i != first_move.x || j != first_move.y)
                {
                    icon_button[i][j]->render_up(render);
                }
            }
        }
    }

    if(first_move.x != -1)
    {
        icon_button[first_move.x][first_move.y]->render_down(render);
    }
}

void Game::render_pause(SDL_Renderer* &render)
{
    pause_menu->render(render);
}

void Game::render_win(SDL_Renderer* &render)
{
    sub_win_menu->render(render);
}

void Game::render_lose(SDL_Renderer* &render)
{
    sub_lose_menu->render(render);
}

bool Game::check_x(int xa, int ya, int xb, int yb)
{
    bool check = false;

    int minxa = xa;
    int maxxa = xa;
    int minxb = xb;
    int maxxb = xb;

    for(int i = xa - 1; i >= 0; i--)
    {
        if(board[i][ya] != 0)
        {
            minxa = i + 1;
            break;
        }

        if(i == 0)
        {
            minxa = i;
        }
    }

    for(int i = xa + 1; i < GAME_ROW; i++)
    {
        if(board[i][ya] != 0)
        {
            maxxa = i - 1;
            break;
        }

        if(i == GAME_ROW - 1)
        {
            maxxa = i;
        }
    }

    for(int i = xb - 1; i >= 0; i--)
    {
        if(board[i][yb] != 0)
        {
            minxb = i + 1;
            break;
        }

        if(i == 0)
        {
            minxb = i;
        }
    }

    for(int i = xb + 1; i < GAME_ROW; i++)
    {
        if(board[i][yb] != 0)
        {
            maxxb = i - 1;
            break;
        }

        if(i == GAME_ROW - 1)
        {
            maxxb = i;
        }
    }

    int minx = max(minxa, minxb);
    int maxx = min(maxxa, maxxb);

    if(minx <= maxx)
    {
        if(ya < yb)
        {
            for(int i = minx; i <= maxx; i++)
            {
                bool check1 = true;

                for(int j = ya + 1; j <= yb - 1; j++)
                {
                    if(board[i][j] != 0)
                    {
                        check1 = false;
                        break;
                    }
                }

                if(check1)
                {
                    check = true;
                    break;
                }
            }
        }

        else if(ya > yb)
        {
            for(int i = minx; i <= maxx; i++)
            {
                bool check1 = true;

                for(int j = yb + 1; j <= ya - 1; j++)
                {
                    if(board[i][j] != 0)
                    {
                        check1 = false;
                        break;
                    }
                }

                if(check1)
                {
                    check = true;
                    break;
                }
            }
        }

        else
        {
            check = true;
        }
    }

    return check;
}

bool Game::check_y(int xa, int ya, int xb, int yb)
{
    bool check = false;

    int minya = ya;
    int maxya = ya;
    int minyb = yb;
    int maxyb = yb;

    for(int i = ya - 1; i >= 0; i--)
    {
        if(board[xa][i] != 0)
        {
            minya = i + 1;
            break;
        }

        if(i == 0)
        {
            minya = i;
        }
    }

    for(int i = ya + 1; i < GAME_COLUMN; i++)
    {
        if(board[xa][i] != 0)
        {
            maxya = i - 1;
            break;
        }

        if(i == GAME_COLUMN - 1)
        {
            maxya = i;
        }
    }

    for(int i = yb - 1; i >= 0; i--)
    {
        if(board[xb][i] != 0)
        {
            minyb = i + 1;
            break;
        }

        if(i == 0)
        {
            minyb = i;
        }
    }

    for(int i = yb + 1; i < GAME_COLUMN; i++)
    {
        if(board[xb][i] != 0)
        {
            maxyb = i - 1;
            break;
        }

        if(i == GAME_COLUMN - 1)
        {
            maxyb = i;
        }
    }

    int miny = max(minya, minyb);
    int maxy = min(maxya, maxyb);

    if(miny <= maxy)
    {
        if(xa < xb)
        {
            for(int i = miny; i <= maxy; i++)
            {
                bool check1 = true;

                for(int j = xa + 1; j <= xb - 1; j++)
                {
                    if(board[j][i] != 0)
                    {
                        check1 = false;
                        break;
                    }
                }

                if(check1)
                {
                    check = true;
                    break;
                }
            }
        }

        else if(xa > xb)
        {
            for(int i = miny; i <= maxy; i++)
            {
                bool check1 = true;

                for(int j = xb + 1; j <= xa - 1; j++)
                {
                    if(board[j][i] != 0)
                    {
                        check1 = false;
                        break;
                    }
                }

                if(check1)
                {
                    check = true;
                    break;
                }
            }
        }

        else
        {
            check = true;
        }
    }

    return check;
}

bool Game::find_way(int xa, int ya, int xb, int yb)
{
    bool success = true;

    if(board[xa][ya] != board[xb][yb])
    {
        success = false;
    }
    else
    {
        if(!check_x(xa, ya, xb, yb) && !check_y(xa, ya, xb, yb))
        {
            success = false;
        }
    }

    return success;
}

bool Game::check_possible()
{
    bool check = false;

    for(int i = 1; i < GAME_ROW - 1; i++)
    {
        for(int j = 1; j < GAME_COLUMN - 1; j++)
        {
            for(int k = 1; k < GAME_ROW - 1; k++)
            {
                for(int l = 1; l < GAME_COLUMN - 1; l++)
                {
                    if(i != k || j != l)
                    {
                        if(board[i][j] == board[k][l] && board[i][j] != 0)
                        {
                            if(find_way(i, j, k, l))
                            {
                                check = true;
                                break;
                            }
                        }
                    }
                }

                if(check)
                {
                    break;
                }
            }

            if(check)
            {
                break;
            }
        }

        if(check)
        {
            break;
        }
    }

    return check;
}

void Game::change()
{
    vector<coordinate> coor;
    vector<int> val;

    for(int i = 1; i < GAME_ROW - 1; i++)
    {
        for(int j = 1; j < GAME_COLUMN - 1; j++)
        {
            if(board[i][j] != 0)
            {
                coor.push_back({i, j});
                val.push_back(board[i][j]);
            }
        }
    }

    random_shuffle(val.begin(), val.end());

    int n = val.size();
    for(int i = 0; i < n; i++)
    {
        board[coor[i].x][coor[i].y] = val[i];
    }
}

bool Game::check_complete()
{
    bool check = true;

    for(int i = 1; i < GAME_ROW - 1;i++)
    {
        for(int j = 1; j < GAME_COLUMN - 1; j++)
        {
            if(board[i][j] != 0)
            {
                check = false;
                break;
            }
        }

        if(!check)
        {
            break;
        }
    }

    return check;
}

int Game::get_count()
{
    return count_change;
}

bool Game::check_new(SDL_Event* e)
{
    return new_button->check_mouse(e);
}

bool Game::check_pause(SDL_Event* e)
{
    return pause_button->check_mouse(e);
}

bool Game::check_menu(SDL_Event* e)
{
    return menu_button->check_mouse(e);
}

bool Game::check_pause_continue(SDL_Event* e)
{
    return pause_menu->play(e);
}

bool Game::check_pause_main_menu(SDL_Event* e)
{
    return pause_menu->main_menu(e);
}

bool Game::check_win_continue(SDL_Event* e)
{
    return sub_win_menu->play(e);
}

bool Game::check_win_main_menu(SDL_Event* e)
{
    return sub_win_menu->main_menu(e);
}

bool Game::check_lose_play(SDL_Event* e)
{
    return sub_lose_menu->play(e);
}

bool Game::check_lose_main_menu(SDL_Event* e)
{
    return sub_lose_menu->main_menu(e);
}

int Game::get_time()
{
    return 420 - in_game_time;
}

void Game::game_time_pause()
{
    game_time->pause();
}

void Game::game_time_unpause()
{
    game_time->unpause();
}
