//
// Created by koksi on 01/07/2026.
//
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <chrono>
#include <ctime>
#include <cmath>

#define TIMEZONEOFFSET (-5)
#define LATITUDE 52.219705
#define LONGITUDE 6.901704
#define WINDOW_WIDTH 256
#define WINDOW_HEIGHT 256

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;
static float clock_scale = 10;
static int offset = 36;
static int night_chunk = 0;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    Calibrate();

    SDL_Surface *surface = NULL;
    char *png_path = NULL;

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("Hello World", 256, 256, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
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

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);

    SDL_DestroySurface(surface);  /* done with this, the texture has a copy of the pixels now. */

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc.) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    int i;

    SDL_FRect dst_rect;
    const Uint64 now = SDL_GetTicks();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);  /* black, full alpha */
    SDL_RenderClear(renderer);

    /* taken from SDL Example 03-lines
    for (i = 0; i < 360; i++) {
        const float size = 65.0f;
        const float x = (WINDOW_WIDTH / 2);
        const float y = (WINDOW_HEIGHT / 2);
        const float r = (float) i * (SDL_PI_F / 180.0f);
        SDL_SetRenderDrawColor(renderer, 73, 104, 216, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, x, y, x + SDL_cosf(r) * size, y + SDL_sinf(r) * size);
    }*/

    SDL_SetRenderDrawColor(renderer, 73, 104, 216, SDL_ALPHA_OPAQUE);
    dst_rect.x = (float) ((WINDOW_WIDTH - 10 * clock_scale) / 2.0f);
    dst_rect.y = (float) ((WINDOW_HEIGHT -12 * clock_scale) / 2.0f);
    dst_rect.w = (float) 10 * clock_scale;
    dst_rect.h = (float) 7 * clock_scale;
    SDL_RenderFillRect(renderer, &dst_rect);

    SDL_Vertex vertices[3];
    for (i= 0; i < night_chunk; i++)
    {
        const float size = 65.0f;
        const float x = (WINDOW_WIDTH /2 );
        const float y = (WINDOW_HEIGHT / 2 );
        const float r = (SDL_PI_F / 180.0f);
        const float step = 7.5f * (i + offset);
        SDL_zeroa(vertices);
        vertices[0].position.x = x;
        vertices[0].position.y = y;
        vertices[0].color.r = 30.0f/255.0f;
        vertices[0].color.g = vertices[0].color.b = 28.0f/255.0f;
        vertices[0].color.a = 1.0f;
        vertices[1].position.x = x + SDL_cosf(r * step) *size;
        vertices[1].position.y = y + SDL_sinf(r * step) *size;
        vertices[1].color = vertices[0].color;
        vertices[2].position.x = x + SDL_cosf(r*(step + 7.5)) *size;
        vertices[2].position.y = y + SDL_sinf(r*(step+7.5)) *size;
        vertices[2].color = vertices[0].color;

        SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);
    }

    dst_rect.x = ((float) (WINDOW_WIDTH - texture_width * clock_scale)) / 2.0f;
    dst_rect.y = ((float) (WINDOW_HEIGHT - texture_height * clock_scale)) / 2.0f;
    dst_rect.w = (float) texture_width * clock_scale;
    dst_rect.h = (float) texture_height * clock_scale;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    SDL_RenderPresent(renderer);  /* put it all on the screen! */

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroyTexture(texture);
}

void Calibrate()
{
    double latitude = LATITUDE;
    double longitude = LONGITUDE;
    int timezoneoffset = TIMEZONEOFFSET;
    int sunrise_hour;
    int sunrise_minute;
    int sunset_hour;
    int sunset_minute;

    constexpr double ZENITH = 90.833;

    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);

    std::tm local{};
#ifdef _WIN32
    localtime_s(&local, &tt);
#else
    local = *std::localtime(&tt);
#endif

    int N = local.tm_yday;

    auto Calculate = [&](bool sunrise)
    {
        double lng_hour = longitude / 15.0;

        double t = sunrise ?
            N + ((6.0 -lng_hour) / 24.0):
            N + ((18.0 - lng_hour) / 24.0);
        // I'm so confused at these numbers they seem arbitary but all the math says yes so idfk
        double M = (0.9856 * t ) - 3.289;

        double L = M
            + 1.916 * SDL_sinf(M * SDL_PI_F /180.0)
            + 0.020 * SDL_sinf(2.0* M * SDL_PI_F /180.0)
            + 282.634;

        //normalise
        while (L < 0) L += 360;
        while (L >= 360) L -= 360;

        double RA = atan(0.91764 * SDL_tanf(L * SDL_PI_F /180.0)) * 180.0 / SDL_PI_F;

        //normalise? I'm still not sure what RA is
        while (RA < 0) RA += 360;
        while (RA >= 360) RA -= 360;

        double Lquadrant = floor(L / 90.0)* 90.0;
        double RAquadrant = floor(RA/90.0) * 90.0;

        RA += (Lquadrant - RAquadrant);
        RA /= 15.0;

        double sinDec = 0.39782 * SDL_sinf(L * SDL_PI_F /180.0);
        double cosDec = SDL_cosf(asin(sinDec));

        double cosH =
            (SDL_cosf(ZENITH * SDL_PI_F /180.0)
            - sinDec * SDL_sinf(latitude * SDL_PI_F /180.0))
            / (cosDec * SDL_cosf(latitude * SDL_PI_F /180.0));

        // dreaded forever sun/night
        if (cosH > 1.0 || cosH < -1.0)
            return -1.0;

        double H;

        if (sunrise)
            H = 360.0 - acos(cosH) * 180.0 / SDL_PI_F;
        else
            H = acos(cosH) * 180.0 / SDL_PI_F;

        H /= 15.0;

        double T = H + RA - (0.6571* t) - 6.622;

        double UT = T - lng_hour;

        while (UT < 0) UT += 24;
        while (UT >= 24) UT -= 24;

        double localTime = UT + timezoneoffset;

        while (localTime < 0) localTime += 24;
        while (localTime >= 24) localTime -=24;

        return localTime;
    };

    double rise = Calculate(true);
    double set = Calculate(false);

    if (rise < 0 || set < 0)
    {
        sunrise_hour = sunrise_minute = -1;
        sunset_hour = sunset_minute = -1;
        return;
    }

    sunrise_hour = static_cast<int>(rise);
    sunrise_minute = static_cast<int>((rise - sunrise_hour) * 60.0 + 0.5);

    if (sunrise_minute == 60)
    {
        sunrise_minute = 0;
        sunrise_hour = (sunrise_hour + 1) % 24;
    }

    sunset_hour = static_cast<int>(set);
    sunset_minute = static_cast<int>((set - sunset_hour) * 60.0 + 0.5);

    if (sunset_minute == 60)
    {
        sunset_minute = 0;
        sunset_hour = (sunset_hour + 1) % 24;
    }

    night_chunk = ((24 - sunset_hour) + (sunrise_hour))*2;

    if (sunrise_minute >= 30)
    {
        night_chunk += 1;
    }

    if (sunset_minute >= 30)
    {
        night_chunk -= 1;
    }
}