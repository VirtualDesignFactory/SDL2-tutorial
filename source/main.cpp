// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 900; // 640
const int SCREEN_HEIGHT = 600; // 480


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

        // Apply the stretched image
        SDL_Rect stretchedRect;
        stretchedRect.x = 0;
        stretchedRect.y = 0;
        stretchedRect.w = SCREEN_WIDTH;
        stretchedRect.h = SCREEN_HEIGHT;
        SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchedRect);

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

    // Initialize PNG loading
    int imgFlags = INT_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    // Get window surface
    gScreenSurface = SDL_GetWindowSurface(gWindow);
    return true;
}


bool loadMedia()
{
    // Load default surface
    gKeyPressSurface[DEFAULT] = loadSurface("images/default.bmp");

    if (gKeyPressSurface[DEFAULT] == NULL)
    {
        printf("Unable to load default image!");
        return false;
    }

    // Load up surface
    gKeyPressSurface[UP] = loadSurface("images/up.bmp");

    if (gKeyPressSurface[UP] == NULL)
    {
        printf("Unable to load up image!");
        return false;
    }

    // Load down surface
    gKeyPressSurface[DOWN] = loadSurface("images/down.bmp");

    if (gKeyPressSurface[DOWN] == NULL)
    {
        printf("Unable to load down image!");
        return false;
    }

    // Load left surface
    gKeyPressSurface[LEFT] = loadSurface("images/left.bmp");

    if (gKeyPressSurface[LEFT] == NULL)
    {
        printf("Unable to load left image!");
        return false;
    }

    // Load right surface
    gKeyPressSurface[RIGHT] = loadSurface("images/right.bmp");

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
    // Initialise the surface to null in case of any errors
    SDL_Surface* optimizedSurface = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_Error: %s/n", path.c_str(), SDL_GetError());
    }
    else
    {
        // Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);

        if (optimizedSurface == NULL)
        {
            printf("Unable to optimse image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of the old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return optimizedSurface;
}
