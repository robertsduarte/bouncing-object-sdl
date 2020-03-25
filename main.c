#include <stdio.h>
#include "SDL.h"
#include "SDL_timer.h"
#include "SDL_image.h"

#define SCREEN_WIDTH (640)
#define SCREEN_HEIGHT (480)
#define SPEED (300)

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
            "Bouncing Object",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            640, 480, 0);

    if (!window)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer)
    {
        printf("%s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = IMG_Load("img/DVD_logo_green.png");
    if (!surface)
    {
        printf("%s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        printf("%s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect image_rect;
    SDL_QueryTexture(texture, NULL, NULL, &image_rect.w, &image_rect.h);

    float image_pos_x = (SCREEN_WIDTH - image_rect.w) / 2;
    float image_pos_y = (SCREEN_HEIGHT - image_rect.h) / 2;
    float image_vel_x = SPEED;
    float image_vel_y = SPEED;

    int quit = 0;

    while (!quit)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                {
                    quit = 1;
                    break;
                }
                case SDL_KEYDOWN:
                {
                    if (e.key.keysym.sym == SDLK_q)
                    {
                        quit = 1;
                    }
                    break;
                }
            }
        }
        if (image_pos_x <= 0)
        {
            image_pos_x = 0;
            image_vel_x = -image_vel_x;
            surface = IMG_Load("img/DVD_logo_blue.png");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        if (image_pos_y <= 0)
        {
            image_pos_y = 0;
            image_vel_y = -image_vel_y;
            surface = IMG_Load("img/DVD_logo_green.png");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        if (image_pos_x >= SCREEN_WIDTH - image_rect.w)
        {
            image_pos_x = SCREEN_WIDTH - image_rect.w;
            image_vel_x = -image_vel_x;
            surface = IMG_Load("img/DVD_logo_red.png");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        if (image_pos_y >= SCREEN_HEIGHT - image_rect.h)
        {
            image_pos_y = SCREEN_HEIGHT - image_rect.h;
            image_vel_y = -image_vel_y;
            surface = IMG_Load("img/DVD_logo_yellow.png");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        image_pos_x += image_vel_x / 60;
        image_pos_y += image_vel_y / 60;
        
        image_rect.x = (int) image_pos_x;
        image_rect.y = (int) image_pos_y;

        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, NULL, &image_rect);
        SDL_RenderPresent(renderer);

        SDL_Delay(1000/60);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
