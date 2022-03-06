#define _XOPEN_SOURCE 600

#include <SDL2/SDL.h>
#include <time.h>

#include "render.h"
#include "surface.h"

static SDL_Window *gWindow = NULL;
static SDL_Surface *gSurface = NULL;

static int mouse_x, mouse_y;

static void frambuffer_init()
{
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Gate", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN);
    gSurface = SDL_GetWindowSurface(gWindow);
}

static void frambuffer_close()
{
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

static void motion_get_xy(int *x, int *y)
{
    *x = mouse_x;
    *y = mouse_y;
}

// #define PROFILE

#ifdef PROFILE
#include <gperftools/profiler.h>
#endif

int main()
{
    struct timespec time1 = {0, 0};
    struct timespec time2 = {0, 0};
#ifdef PROFILE
    ProfilerStart("test.prof"); //开启性能分析
    atexit(ProfilerStop);
#endif
    frambuffer_init();
    float fps[4] = {0};
    int fps_index = 0;
    float cur_fps = 0;
#if 0
    surface_t *surface = surface_alloc(W, H);

    while (1)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            }
        }
        clock_gettime(CLOCK_MONOTONIC, &time1);
        sample(surface, cur_fps);
        clock_gettime(CLOCK_MONOTONIC, &time2);
        unsigned long long mtime = (time2.tv_sec - time1.tv_sec) * 1000000 + (time2.tv_nsec - time1.tv_nsec) / 1000;
        fps[(fps_index++) % 4] = 1000000.0f / mtime;
        cur_fps = (fps[0] + fps[1] + fps[2] + fps[3]) / 3;
    }
#else

    surface_t *surface = &(surface_t){0};
    surface_wrap(surface, gSurface->pixels, W, H);

    while (1)
    {
        clock_gettime(CLOCK_MONOTONIC, &time1);

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                exit(EXIT_SUCCESS);
                break;
            case SDL_MOUSEMOTION:
                mouse_x = e.motion.x;
                mouse_y = e.motion.y;
                break;
            }
        }

        sample(surface, cur_fps);
        SDL_UpdateWindowSurface(gWindow);
        clock_gettime(CLOCK_MONOTONIC, &time2);
        unsigned long long mtime = (time2.tv_sec - time1.tv_sec) * 1000000 + (time2.tv_nsec - time1.tv_nsec) / 1000;
        fps[(fps_index++) % 4] = 1000000.0f / mtime;
        cur_fps = (fps[0] + fps[1] + fps[2] + fps[3]) / 4;
    }
    frambuffer_close();
#endif
}