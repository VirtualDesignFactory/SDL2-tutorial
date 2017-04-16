// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//// Function declarations

// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();



//// Global variables -> Yes this is bad...

// The window we will be rendering to
SDL_Window*  gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;



//// Start of Main function

int main(int argc, char* args[])
{
    // Start SDL and create a window
    if (!init())
    {
        printf("Failed to initalize!\n");
        close();
        return -1;
    }

    // Load media
    if (!loadMedia())
    {
        printf("Failed to load media!\n");
        close();
        return -1;
    }


    // Apply the image
    SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);

    // Update the surface
    SDL_UpdateWindowSurface(gWindow);

    // Wait two seconds
    SDL_Delay(2000);


    // Free resources and close SDL
    close();

    return 0;
}


//// Function implementations

bool init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized! SDL_Error: %s/n", SDL_GetError());
        return false;
    }

    // Create a window
    gWindow = SDL_CreateWindow(
        "SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (gWindow == NULL)
    {
        printf("Window could not be created! SDL_Error: %s/n", SDL_GetError());
        return false;
    }

    // Get window surface
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return true;
}


bool loadMedia()
{
    // Load splash image
    gHelloWorld = SDL_LoadBMP("images/hello_world.bmp");

    if (gHelloWorld == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s/n", "images/hello_world.bmp", SDL_GetError());
        return false;
    }

    return true;
}


void close()
{
    // Deallocate surface
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL Subsystems
    SDL_Quit();
}