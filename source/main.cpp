// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH  = 640; // 640
const int SCREEN_HEIGHT = 480; // 480


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

// Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Current displayed texture
SDL_Texture* gTexture= NULL;

// The images that correspond to a keypress
SDL_Texture* gKeyPressSurface[TOTAL];


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
    gTexture = gKeyPressSurface[DEFAULT];

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
                    gTexture = gKeyPressSurface[UP];
                    break;

                    case SDLK_DOWN:
                    gTexture = gKeyPressSurface[DOWN];
                    break;

                    case SDLK_LEFT:
                    gTexture = gKeyPressSurface[LEFT];
                    break;

                    case SDLK_RIGHT:
                    gTexture = gKeyPressSurface[RIGHT];
                    break;

                    default:
                    gTexture = gKeyPressSurface[DEFAULT];
                }
            }
        }

        // Clear the screen
        SDL_RenderClear(gRenderer);

        // Render the texture to the screen
        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

        // Update the screen
        SDL_RenderPresent(gRenderer);
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

    // Create renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s/n", SDL_GetError());
        return false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize PNG loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}


bool loadMedia()
{
    // Load default surface
    gKeyPressSurface[DEFAULT] = loadTexture("images/default.png");

    if (gKeyPressSurface[DEFAULT] == NULL)
    {
        printf("Unable to load default image!");
        return false;
    }

    // Load up surface
    gKeyPressSurface[UP] = loadTexture("images/up.png");

    if (gKeyPressSurface[UP] == NULL)
    {
        printf("Unable to load up image!");
        return false;
    }

    // Load down surface
    gKeyPressSurface[DOWN] = loadTexture("images/down.png");

    if (gKeyPressSurface[DOWN] == NULL)
    {
        printf("Unable to load down image!");
        return false;
    }

    // Load left surface
    gKeyPressSurface[LEFT] = loadTexture("images/left.png");

    if (gKeyPressSurface[LEFT] == NULL)
    {
        printf("Unable to load left image!");
        return false;
    }

    // Load right surface
    gKeyPressSurface[RIGHT] = loadTexture("images/right.png");

    if (gKeyPressSurface[RIGHT] == NULL)
    {
        printf("Unable to load right image!");
        return false;
    }

    return true;
}


void close()
{
    // Free loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL Subsystems
    IMG_Quit();
    SDL_Quit();
}


SDL_Texture* loadTexture(std::string path)
{
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load the image at the specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_Image Error: %s/n", path.c_str(), IMG_GetError());
        return newTexture;
    }

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

    if (newTexture == NULL)
    {
        printf("Unable to create tesxture from %s! SDL_Error: %s/n", path.c_str(), SDL_GetError());
    }

    // Get rid of the old loaded surface
    SDL_FreeSurface(loadedSurface);

    return newTexture;
}
