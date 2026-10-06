#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    int getTicks();

    bool isStarted();
    bool isPaused();

private:
    int mStartTicks;

    int mPausedTicks;

    bool mPaused;
    bool mStarted;
};
#endif // TIMER_H_INCLUDED
