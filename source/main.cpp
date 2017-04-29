// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH  = 640; // 640
const int SCREEN_HEIGHT = 480; // 480


// Texture wrapper class
class Texture
{
public:
    // Initalize variables
    Texture();

    // Deallocates memory
    ~Texture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Deallocates texture
    void free();

    // Renderes texture at given point
    void render(int x, int y);

    // Gets image dimensions
    int getWidth();
    int getHeight();

private:
    // The actual hardware texture
    SDL_Texture* _texture;

    // Image dimensions
    int _width;
    int _height;
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

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Scene textures
Texture gMeTexture;
Texture gBackgroundTexture;

//// Class methods

Texture::Texture()
{
    // Initialize
    _texture = NULL;
    _width   = 0;
    _height  = 0;
}

Texture::~Texture()
{
    // Deallocate
    free();
}

bool Texture::loadFromFile(std::string path)
{
    // Get rid of the pre-existing texture
    free();

    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load the image at the specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load the image %s! SDL_Image Error: %s/n", path.c_str(), IMG_GetError());
        return false;
    }

    // color key the image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

    if (newTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s/n", path.c_str(), SDL_GetError());
        return false;
    }

    // Get image dimensions
    _width  = loadedSurface->w;
    _height = loadedSurface->h;

    // Get rid of the old loaded surface
    SDL_FreeSurface(loadedSurface);

    _texture = newTexture;

    return true;
}

void Texture::free()
{
    // Free the texture if it exists
    if (_texture != NULL)
    {
        SDL_DestroyTexture(_texture);
        _texture = NULL;
        _width   = 0;
        _height  = 0;
    }
}

void Texture::render(int x, int y)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, _width, _height };
    SDL_RenderCopy(gRenderer, _texture, NULL, &renderQuad);
}

int Texture::getWidth()
{
    return _width;
}

int Texture::getHeight()
{
    return _height;
}

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
        }

        // Clear the screen
        SDL_SetRenderDrawColor(gRenderer, 30, 0, 30, 255);
        SDL_RenderClear(gRenderer);

        // Render the 'Background' texture to the screen
        gBackgroundTexture.render(0, 0);

        // Render the 'me' texture to the screen
        gMeTexture.render(240, 190);

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
    // Load 'me' texture
    if (!gMeTexture.loadFromFile("images/me.png"))
    {
        printf("Failed to load 'Me' texture image!/n");
        return false;
    }

    // Load 'background' texture
    if (!gBackgroundTexture.loadFromFile("images/background.png"))
    {
        printf("Failed to load 'Background' texture image!/n");
        return false;
    }

    return true;
}


void close()
{
    // Free loaded images
    gMeTexture.free();
    gBackgroundTexture.free();

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL Subsystems
    IMG_Quit();
    SDL_Quit();
}
