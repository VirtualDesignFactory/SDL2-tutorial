// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


// Key press surface constants
enum KeyPressSurface
{
    DEFAULT,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    TOTAL
};

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

// Loads individual image
SDL_Surface* loadSurface(std::string path);

// The images that correspond to a keypress
SDL_Surface* gKeyPressSurface[TOTAL];

// Current displayed image
SDL_Surface* gCurrentSurface = NULL;



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

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Set the default current surface
    gCurrentSurface = gKeyPressSurface[DEFAULT];

    // While the application is running
    while (!quit)
    {
        // Handle events on the queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            if (e.type == SDL_KEYDOWN)
            {
                // Select surfaces based on the key press
                switch (e.key.keysym.sym)
                {
                    case SDLK_UP:
                    gCurrentSurface = gKeyPressSurface[UP];
                    break;

                    case SDLK_DOWN:
                    gCurrentSurface = gKeyPressSurface[DOWN];
                    break;

                    case SDLK_LEFT:
                    gCurrentSurface = gKeyPressSurface[LEFT];
                    break;

                    case SDLK_RIGHT:
                    gCurrentSurface = gKeyPressSurface[RIGHT];
                    break;

                    default:
                    gCurrentSurface = gKeyPressSurface[DEFAULT];
                }
            }
        }

        // Apply the image
        SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

        // Update the surface
        SDL_UpdateWindowSurface(gWindow);
    }

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
    // Load default surface
    gKeyPressSurface[DEFAULT] = SDL_LoadBMP("images/default.bmp");

    if (gKeyPressSurface[DEFAULT] == NULL)
    {
        printf("Unable to load default image!");
        return false;
    }

    // Load up surface
    gKeyPressSurface[UP] = SDL_LoadBMP("images/up.bmp");

    if (gKeyPressSurface[UP] == NULL)
    {
        printf("Unable to load up image!");
        return false;
    }

    // Load down surface
    gKeyPressSurface[DOWN] = SDL_LoadBMP("images/down.bmp");

    if (gKeyPressSurface[DOWN] == NULL)
    {
        printf("Unable to load down image!");
        return false;
    }

    // Load left surface
    gKeyPressSurface[LEFT] = SDL_LoadBMP("images/left.bmp");

    if (gKeyPressSurface[LEFT] == NULL)
    {
        printf("Unable to load left image!");
        return false;
    }

    // Load right surface
    gKeyPressSurface[RIGHT] = SDL_LoadBMP("images/right.bmp");

    if (gKeyPressSurface[RIGHT] == NULL)
    {
        printf("Unable to load right image!");
        return false;
    }

    return true;
}


void close()
{
    // Deallocate surface
    SDL_FreeSurface(gCurrentSurface);
    gCurrentSurface = NULL;

    // Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL Subsystems
    SDL_Quit();
}

SDL_Surface* loadSurface(std::string path)
{
    // Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s/n", path.c_str(), SDL_GetError());
    }

    return loadedSurface;
}