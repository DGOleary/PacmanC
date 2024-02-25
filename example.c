#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)
#define SPEED (300)
#define SCROLL_SPEED (300)

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Hello!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    if (!window)
    {
        printf("window creation error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *rend = SDL_CreateRenderer(window, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = IMG_Load("mugi.jpg");
    if (!surface)
    {
        printf("error creating surface: %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    if (!tex)
    {
        printf("error creating texture: %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect dest;

    SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);
    dest.w /= 4;
    dest.h /= 4;

    dest.x = (WINDOW_WIDTH - dest.w) / 2;

    float x_pos = (WINDOW_WIDTH - dest.w) / 2;
    float y_pos = (WINDOW_WIDTH - dest.h) / 2;
    float x_vel = 0;
    float y_vel = 0;

    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    bool close_requested = false;

    while (!close_requested)
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                close_requested = true;
            }
        }
        // while (SDL_PollEvent(&event))
        // {
        //     switch (event.type)
        //     {
        //     case SDL_QUIT:
        //         close_requested = true;
        //         break;
            // case SDL_KEYDOWN:
            //     switch (event.key.keysym.scancode)
            //     {
            //     case SDL_SCANCODE_W:
            //     case SDL_SCANCODE_UP:
            //         up = 1;
            //         break;
            //     case SDL_SCANCODE_S:
            //     case SDL_SCANCODE_DOWN:
            //         down = 1;
            //         break;
            //     case SDL_SCANCODE_A:
            //     case SDL_SCANCODE_LEFT:
            //         left = 1;
            //         break;
            //     case SDL_SCANCODE_D:
            //     case SDL_SCANCODE_RIGHT:
            //         right = 1;
            //         break;
            //     }
            //     break;
            // case SDL_KEYUP:
            //     switch (event.key.keysym.scancode)
            //     {
            //     case SDL_SCANCODE_W:
            //     case SDL_SCANCODE_UP:
            //         up = 0;
            //         break;
            //     case SDL_SCANCODE_S:
            //     case SDL_SCANCODE_DOWN:
            //         down = 0;
            //         break;
            //     case SDL_SCANCODE_A:
            //     case SDL_SCANCODE_LEFT:
            //         left = 0;
            //         break;
            //     case SDL_SCANCODE_D:
            //     case SDL_SCANCODE_RIGHT:
            //         right = 0;
            //         break;
            //     }
            //     break;
           // }
       // }

        int mouse_x,mouse_y;
        int buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

        int target_x = mouse_x - dest.w / 2;
        int target_y = mouse_y - dest.h / 2;
        float delta_x = target_x - x_pos;
        float delta_y = target_y - y_pos;
        float distance = sqrt(delta_x * delta_x + delta_y * delta_y);
        // x_vel = y_vel = 0;
        // if (up && !down)
        // {
        //     y_vel = -SPEED;
        // }
        // if (!up && down)
        // {
        //     y_vel = SPEED;
        // }
        // if (left && !right)
        // {
        //     x_vel = -SPEED;
        // }
        // if (!left && right)
        // {
        //     x_vel = SPEED;
        // }

        if(distance < 5){
            x_vel = y_vel = 0;
        }
        else
        {
            x_vel = delta_x * SPEED / distance;
            y_vel = delta_y * SPEED / distance;
        }

        if(buttons & SDL_BUTTON(SDL_BUTTON_LEFT)){
            x_vel = -x_vel;
            y_vel = -y_vel;
        }

        // update positions
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;

        // if (x_pos <= 0)
        //     x_pos = 0;
        // if (y_pos <= 0)
        //     y_pos = 0;
        // if (x_pos >= WINDOW_WIDTH - dest.w)
        //     x_pos = WINDOW_WIDTH - dest.w;
        // if (y_pos >= WINDOW_HEIGHT - dest.h)
        //     y_pos = WINDOW_HEIGHT - dest.h;

        if (x_pos <= 0) x_pos = 0;
        if (y_pos <= 0) y_pos = 0;
        if (x_pos >= WINDOW_WIDTH - dest.w) x_pos = WINDOW_WIDTH - dest.w;
        if (y_pos >= WINDOW_HEIGHT - dest.h) y_pos = WINDOW_HEIGHT - dest.h;

        // set the positions in the struct
        dest.y = (int) y_pos;
        dest.x = (int) x_pos;

        // clear the window
        SDL_RenderClear(rend);

        // draw the image to the window
        SDL_RenderCopy(rend, tex, NULL, &dest);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000 / 60);
    }

    // while(dest.y >= -dest.h){
    //     SDL_RenderClear(rend);

    //     //puts into integers for pixel locations
    //     dest.y = (int) y_pos;

    //     //draws image to window, null area can be used to pick a sprite from a texture map
    //     SDL_RenderCopy(rend, tex, NULL, &dest);
    //     SDL_RenderPresent(rend);

    //     //updates sprite position once per frame
    //     y_pos -= (float) SCROLL_SPEED/60;

    //     //1/60th of a second
    //     SDL_Delay(1000/60);
    // }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
}