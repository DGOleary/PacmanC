#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "drawfunctions.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "defaultfunctions.h"

#define WINDOW_WIDTH (825)
#define WINDOW_HEIGHT (400)
#define SPEED (300)

//function used to check if boundaries are the same, assumes boundaries don't start at the same origin
int compareBoundary(void *bound1, void *bound2){
    //casts the voids to boundaries
    Boundary bounds1=*(Boundary*)bound1;
    Boundary bounds2=*(Boundary*)bound2;
    if(bounds1.x-bounds2.x==0){
        return bounds1.y-bounds2.y;
    }
    return bounds1.x-bounds2.x;
}

//function to hash boundaries
int boundaryHash(HashSet *set, void *add){
    //casts to a boundary
    Boundary bound=*(Boundary*)add;
    //array of primes for each value in the Boundary struct
    int prime[] = {17, 619, 1861, 79};
    //initial prime to seed function 
    int seed = 67;
    //combines all the values into one hash mod the size 
    int hash = (seed * (prime[0] + bound.x) * (prime[1] + bound.y) * (prime[2] + bound.h) * (prime[3] + bound.w)) % set->size;
    return hash;
}


int main(int argc, char *argv[])
{
    // area of a single tile from the texture atlas
    SDL_Rect area;
    area.x = 0;
    area.y = 0;
    area.w = 25;
    area.h = 25;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

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

    // enables alpha blending
    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);

    // explicitly enables pngs
    IMG_Init(IMG_INIT_PNG);

    // loads texture atlas
    SDL_Surface *surface = IMG_Load("atlas.png");
    //load the background image
    SDL_Surface *background = IMG_Load("background.png");
    //checks both for errors
    if (!surface||!background)
    {
        printf("error creating surface: %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
    SDL_Texture *back_tex = SDL_CreateTextureFromSurface(rend, background);

    if (!tex||!back_tex)
    {
        printf("error creating texture: %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //frees background memory
    SDL_FreeSurface(background);

    // loads window icon
    surface = IMG_Load("icon.png");
    if (!surface)
    {
        printf("error creating surface: %s", SDL_GetError());
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);

    // creates rectangle to export pacman texture to
    SDL_Rect pac;
    SDL_QueryTexture(tex, NULL, NULL, &pac.w, &pac.h);
    pac.w=25;
    pac.h=25;

    SDL_Rect *ghosts = (SDL_Rect*)malloc(4*sizeof(SDL_Rect));
    
    //creates rectangle for the background
    SDL_Rect bg;
    SDL_QueryTexture(back_tex, NULL, NULL, &bg.w, &bg.h);
    bg.w=825;
    bg.h=400;

    Sprite_Values *back;

    pac.x = (WINDOW_WIDTH - pac.w) / 2;

    float x_pos = (WINDOW_WIDTH - pac.w) / 2;
    float y_pos = (WINDOW_WIDTH - pac.h) / 2;
    float x_vel = 0;
    float y_vel = 0;

    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    // holds the link to the renderer and the texture atlas
    SDL_Objs obj;
    obj.rend = rend;
    obj.tex = tex;

    Sprite_Values pac_val = *createSpriteValues(&pac, 5, 2, 25, 25, 0, SDL_FLIP_NONE);
    // manually making the array of frame values
    pac_val.frame_offsets[0] = (int[]){0, 0};
    pac_val.frame_offsets[1] = (int[]){25, 0};

    bool close_requested = false;

    //create boundaries for the maze
        Boundary bottom;
        bottom.x=0;
        bottom.y=375;
        bottom.w=25;
        bottom.h=25;

        HashSet *boundaries = createHashSet();

        HashSetAdd(boundaries, &bottom, boundaryHash);
        // set the positions in the struct
        pac.y = (int)y_pos;
        pac.x = (int)x_pos;

    while (!close_requested)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                close_requested = true;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 1;
                    down = left = right = 0;
                    pac_val.flip = SDL_FLIP_NONE;
                    pac_val.dir = 90;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 1;
                    up = left = right = 0;
                    pac_val.flip = SDL_FLIP_NONE;
                    pac_val.dir = 270;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    down = up = right = 0;
                    pac_val.flip = SDL_FLIP_NONE;
                    pac_val.dir = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    down = left = up = 0;
                    pac_val.flip = SDL_FLIP_HORIZONTAL;
                    pac_val.dir = 0;
                    break;
                }
                break;
                //TODO remove this code and edit so he continues in the same direction and can only move in available directions
            case SDL_KEYUP:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                }
                break;
            }
        }

        x_vel = y_vel = 0;
        if (up && !down)
        {
            y_vel = -SPEED;
        }
        if (!up && down)
        {
            y_vel = SPEED;
        }
        if (left && !right)
        {
            x_vel = -SPEED;
        }
        if (!left && right)
        {
            x_vel = SPEED;
        }

        // update positions
        x_pos += x_vel / 60;
        y_pos += y_vel / 60;
        
        // if (x_pos <= 0)
        //     x_pos = 0;
        // if (y_pos <= 0)
        //     y_pos = 0;
        // if (x_pos >= WINDOW_WIDTH - pac.w)
        //     x_pos = WINDOW_WIDTH - pac.w;
        // if (y_pos >= WINDOW_HEIGHT - pac.h)
        //     y_pos = WINDOW_HEIGHT - pac.h;

        Boundary *pos;
        pos->x=x_pos;
        pos->y=y_pos;
        pos->h=25;
        pos->y=25;
        if(HashSetContains(boundaries, pos, boundaryHash, compareBoundary)){
            printf("bounds check");
        }

        // clear the window
        SDL_RenderClear(rend);

        //draw background
        SDL_RenderCopy(rend, back_tex, NULL, NULL);

        // draw the pacman to the window
        animateLoop(&obj, &pac_val);
        SDL_RenderPresent(rend);

        // wait 1/60th of a second
        SDL_Delay(1000 / 60);
    }

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(window);
    SDL_Quit();
}





//TODO function that will make background
// void drawLevel(SDL_Renderer *rend, SDL_Texture *tex, Sprite_Values **vals){
//     *vals=(Sprite_Values*)malloc(10*sizeof(Sprite_Values));
//     for(int i=0;i<10;i++){
//         //rects for the location on screen and 
//         SDL_Rect *temploc=(SDL_Rect*)malloc(sizeof(SDL_Rect));
//         SDL_Rect *tempat=(SDL_Rect*)malloc(sizeof(SDL_Rect));
//         vals[i]=createBGTile(temp, 0, 0, 25, 25, SDL_FLIP_NONE);
//     }
//     SDL_RenderCopy(rend, tex, )
// }