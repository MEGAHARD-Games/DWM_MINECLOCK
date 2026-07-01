//
// Created by koksi on 01/07/2026.
//
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_Surface *surface = NULL;
    char *png_path = NULL;

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 800, 600, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* SDL_Surface is pixel data the CPU can access. SDL_Texture is pixel data the GPU can access.
  Load a .png into a surface, move it to a texture from there. */
    SDL_asprintf(&png_path, "%s/assets/minecraft_clock.png", SDL_GetBasePath());  /* allocate a string of the full file path */
    surface = SDL_LoadPNG(png_path);
    if (!surface) {
        SDL_Log("Couldn't load png: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(png_path);  /* done with this, the file is loaded. */

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc.) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_KEY_DOWN ||
        event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_FRect dst_rect;
    const Uint64 now = SDL_GetTicks();

    /* we'll have some textures move around over a few seconds. */
    const float direction = ((now % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float) (((int) (now % 1000)) - 500) / 500.0f) * direction;

    /* as you can see from this, rendering draws over whatever was drawn before it. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);  /* start with a blank canvas. */

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it. */

    /* top left */
    dst_rect.x = (100.0f * scale);
    dst_rect.y = 0.0f;
    dst_rect.w = (float) texture_width;
    dst_rect.h = (float) texture_height;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    /* center this one. */
    dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) / 2.0f;
    dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height)) / 2.0f;
    dst_rect.w = (float) texture_width;
    dst_rect.h = (float) texture_height;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    /* bottom right. */
    dst_rect.x = ((float) (WINDOW_WIDTH - texture_width)) - (100.0f * scale);
    dst_rect.y = (float) (WINDOW_HEIGHT - texture_height);
    dst_rect.w = (float) texture_width;
    dst_rect.h = (float) texture_height;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
}