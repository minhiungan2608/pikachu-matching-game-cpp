#include <iostream>
#include "delete.h"
#include "everything.h"

using namespace std;

void Game_Delete::lv1_delete(int arr[][GAME_COLUMN], int xa, int ya, int xb, int yb)
{
    arr[xa][ya] = 0;
    arr[xb][yb] = 0;
}

void Game_Delete::lv2_delete(int arr[][GAME_COLUMN], int xa, int ya, int xb, int yb)
{
    if(ya == yb)
    {
        if(xa > xb)
        {
            for(int i = xb; i >= 1; i--)
            {
                arr[i][yb] = arr[i - 1][yb];
            }

            for(int i = xa; i >= 1; i--)
            {
                arr[i][ya] = arr[i - 1][ya];
            }
        }

        else
        {
            for(int i = xa; i >= 1; i--)
            {
                arr[i][ya] = arr[i - 1][ya];
            }

            for(int i = xb; i >= 1; i--)
            {
                arr[i][yb] = arr[i - 1][yb];
            }
        }
    }

    else
    {
        for(int i = xa; i >= 1; i--)
        {
            arr[i][ya] = arr[i - 1][ya];
        }

        for(int i = xb; i >= 1; i--)
        {
            arr[i][yb] = arr[i - 1][yb];
        }
    }
}

void Game_Delete::lv3_delete(int arr[][GAME_COLUMN], int xa, int ya, int xb, int yb)
{
    if(ya == yb)
    {
        if(xa > xb)
        {
            for(int i = xa; i < GAME_ROW - 1; i++)
            {
                arr[i][ya] = arr[i + 1][ya];
            }

            for(int i = xb; i < GAME_ROW - 1; i++)
            {
                arr[i][yb] = arr[i + 1][yb];
            }
        }

        else
        {
            for(int i = xb; i < GAME_ROW - 1; i++)
            {
                arr[i][yb] = arr[i + 1][yb];
            }

            for(int i = xa; i < GAME_ROW - 1; i++)
            {
                arr[i][ya] = arr[i + 1][ya];
            }
        }
    }

    else
    {
        for(int i = xa; i < GAME_ROW - 1; i++)
        {
            arr[i][ya] = arr[i + 1][ya];
        }

        for(int i = xb; i < GAME_ROW - 1; i++)
        {
            arr[i][yb] =arr[i + 1][yb];
        }
    }
}

void Game_Delete::lv4_delete(int arr[][GAME_COLUMN], int xa, int ya, int xb, int yb)
{
    if(xa == xb)
    {
        if(ya > yb)
        {
            for(int i = yb; i >= 1; i--)
            {
                arr[xb][i] = arr[xb][i - 1];
            }

            for(int i = ya; i >= 1; i--)
            {
                arr[xa][i] = arr[xa][i - 1];
            }
        }

        else
        {
            for(int i = ya; i >= 1; i--)
            {
                arr[xa][i] = arr[xa][i - 1];
            }

            for(int i = yb; i >= 1; i--)
            {
                arr[xb][i] = arr[xb][i - 1];
            }
        }
    }

    else
    {
        for(int i = ya; i >= 1; i--)
        {
            arr[xa][i] = arr[xa][i - 1];
        }

        for(int i = yb; i >= 1; i--)
        {
            arr[xb][i] = arr[xb][i - 1];
        }
    }
}

void Game_Delete::lv5_delete(int arr[][GAME_COLUMN], int xa, int ya, int xb, int yb)
{
    if(xa == xb)
    {
        if(ya > yb)
        {
            for(int i = ya; i < GAME_COLUMN - 1; i++)
            {
                arr[xa][i] = arr[xa][i + 1];
            }

            for(int i = yb; i < GAME_COLUMN - 1; i++)
            {
                arr[xb][i] = arr[xb][i + 1];
            }
        }

        else
        {
            for(int i = yb; i < GAME_COLUMN - 1; i++)
            {
                arr[xb][i] = arr[xb][i + 1];
            }

            for(int i = ya; i < GAME_COLUMN - 1; i++)
            {
                arr[xa][i] = arr[xa][i + 1];
            }
        }
    }

    else
    {
        for(int i = ya; i < GAME_COLUMN - 1; i++)
        {
            arr[xa][i] = arr[xa][i + 1];
        }

        for(int i = yb; i < GAME_COLUMN - 1; i++)
        {
            arr[xb][i] = arr[xb][i + 1];
        }
    }
}

void Game_Delete::lv6_delete(int arr[][GAME_COLUMN], int xa, int ya, int xb, int yb)
{
    if(ya <= 8 && yb <= 8)
    {
        if(xa == xb)
        {
            if(ya > yb)
            {
                for(int i = ya; i <= 7; i++)
                {
                    arr[xa][i] = arr[xa][i + 1];
                }

                arr[xa][8] = 0;

                for(int i = yb; i <= 7; i++)
                {
                    arr[xb][i] = arr[xb][i + 1];
                }

                arr[xb][8] = 0;
            }

            else
            {
                for(int i = yb; i <= 7; i++)
                {
                    arr[xb][i] = arr[xb][i + 1];
                }

                arr[xb][8] = 0;

                for(int i = ya; i <= 7; i++)
                {
                    arr[xa][i] = arr[xa][i + 1];
                }

                arr[xa][8] = 0;
            }
        }

        else
        {
            for(int i = ya; i <= 7; i++)
            {
                arr[xa][i] = arr[xa][i + 1];
            }

            arr[xa][8] = 0;

            for(int i = yb; i <= 7; i++)
            {
                arr[xb][i] = arr[xb][i + 1];
            }

            arr[xb][8] = 0;
        }
    }

    else if(ya >= 9 && yb >= 9)
    {
        if (xa == xb)
        {
            if(ya > yb)
            {
                for(int i = yb; i >= 10; i--)
                {
                    arr[xb][i] = arr[xb][i - 1];
                }

                arr[xb][9] = 0;

                for(int i = ya; i >= 10; i--)
                {
                    arr[xa][i] = arr[xa][i - 1];
                }

                arr[xa][9] = 0;
            }

            else
            {
                for(int i = ya; i >= 10; i--)
                {
                    arr[xa][i] = arr[xa][i - 1];
                }

                arr[xa][9] = 0;

                for(int i = yb; i >= 10; i--)
                {
                    arr[xb][i] = arr[xb][i - 1];
                }

                arr[xb][9] = 0;
            }
        }

        else
        {
            for(int i = ya; i >= 10; i--)
            {
                arr[xa][i] = arr[xa][i - 1];
            }

            arr[xa][9] = 0;

            for(int i = yb; i >= 10; i--)
            {
                arr[xb][i] = arr[xb][i - 1];
            }

            arr[xb][9] = 0;
        }
    }

    else if(ya <= 8 && yb >= 9)
    {
        for(int i = ya; i <= 7; i++)
        {
            arr[xa][i] = arr[xa][i + 1];
        }

        arr[xa][8] = 0;

        for(int i = yb; i >= 10; i--)
        {
            arr[xb][i] = arr[xb][i - 1];
        }

        arr[xb][9] = 0;
    }

    else if(ya >= 9 && yb <= 8)
    {
        for(int i = ya; i >= 10; i--)
        {
            arr[xa][i] = arr[xa][i - 1];
        }

        arr[xa][9] = 0;

        for(int i = yb; i <= 7; i++)
        {
            arr[xb][i] = arr[xb][i + 1];
        }

        arr[xb][8] = 0;
    }
}

void Game_Delete::lv7_delete(int arr[][GAME_COLUMN], int xa, int ya, int xb, int yb)
{
    if(ya <= 8 && yb <= 8)
    {
        if(xa == xb)
        {
            if(ya > yb)
            {
                for(int i = yb; i >= 1; i--)
                {
                    arr[xb][i] = arr[xb][i - 1];
                }

                for(int i = ya; i >= 1; i--)
                {
                    arr[xa][i] = arr[xa][i - 1];
                }
            }

            else
            {
                for(int i = ya; i >= 1; i--)
                {
                    arr[xa][i] = arr[xa][i - 1];
                }

                for(int i = yb; i >= 1; i--)
                {
                    arr[xb][i] = arr[xb][i - 1];
                }
            }
        }

        else
        {
            for(int i = ya; i >= 1; i--)
            {
                arr[xa][i] = arr[xa][i - 1];
            }

            for(int i = yb; i >= 1; i--)
            {
                arr[xb][i] = arr[xb][i - 1];
            }
        }
    }

    else if(ya >= 9 && yb >= 9)
    {
        if (xa == xb)
        {
            if(ya > yb)
            {
                for(int i = ya; i < GAME_COLUMN - 1; i++)
                {
                    arr[xa][i] = arr[xa][i + 1];
                }

                for(int i = yb; i <= GAME_COLUMN - 1; i++)
                {
                    arr[xb][i] = arr[xb][i + 1];
                }
            }

            else
            {
                for(int i = yb; i <= GAME_COLUMN - 1; i++)
                {
                    arr[xb][i] = arr[xb][i + 1];
                }

                for(int i = ya; i <= GAME_COLUMN - 1; i++)
                {
                    arr[xa][i] = arr[xa][i + 1];
                }
            }
        }

        else
        {
            for(int i = ya; i <= GAME_COLUMN - 1; i++)
            {
                arr[xa][i] = arr[xa][i + 1];
            }

            for(int i = yb; i <= GAME_COLUMN - 1; i++)
            {
                arr[xb][i] = arr[xb][i + 1];
            }
        }
    }

    else if(ya <= 8 && yb >= 9)
    {
        for(int i = ya; i >= 1; i--)
        {
            arr[xa][i] = arr[xa][i - 1];
        }

        for(int i = yb; i <= GAME_COLUMN - 1; i++)
        {
            arr[xb][i] = arr[xb][i + 1];
        }
    }

    else if(ya >= 9 && yb <= 8)
    {
        for(int i = ya; i <= GAME_COLUMN - 1; i++)
        {
            arr[xa][i] = arr[xa][i + 1];
        }

        for(int i = yb; i >= 1; i--)
        {
            arr[xb][i] = arr[xb][i - 1];
        }
    }
}
