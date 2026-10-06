#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "timer.h"

using namespace std;

Timer::Timer()
{
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

Timer::~Timer()
{}

void Timer::start()
{
    mStarted = true;

    mPaused = false;

    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::stop()
{
    mStarted = false;

    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void Timer::pause()
{
    if( mStarted && !mPaused )
    {
        mPaused = true;

        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void Timer::unpause()
{
    if( mStarted && mPaused )
    {
        mPaused = false;

        mStartTicks = SDL_GetTicks() - mPausedTicks;

        mPausedTicks = 0;
    }
}

int Timer::getTicks()
{
    int time = 0;

    if( mStarted )
    {
        if( mPaused )
        {
            time = mPausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool Timer::isStarted()
{
    return mStarted;
}

bool Timer::isPaused()
{
    return mPaused && mStarted;
}
