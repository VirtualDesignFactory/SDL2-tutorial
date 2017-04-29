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
    void render(int x, int y, SDL_Rect* clip = NULL);

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
SDL_Rect gSpriteClips[4];
Texture gSpriteSheetTexture;

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
        printf("Unable to load the image %s! SDL_Image Error: %s\n", path.c_str(), IMG_GetError());
        return false;
    }

    // color key the image
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

    // Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

    if (newTexture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
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

void Texture::render(int x, int y, SDL_Rect* clip)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, _width, _height };

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to the screen
    SDL_RenderCopy(gRenderer, _texture, clip, &renderQuad);
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


        // Render the top left sprite
        gSpriteSheetTexture.render(0, 0, &gSpriteClips[0]);

        // Render the top right sprite
        gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);

        // Render the bottom left sprite
        gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);

        // Render the bottom right sprite
        gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);


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
        printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
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
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer for window
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
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
    if (!gSpriteSheetTexture.loadFromFile("images/sprites.png"))
    {
        printf("Failed to load sprite sheet texture image!\n");
        return false;
    }

    // Set sprite clips (x, y, w, h)
    gSpriteClips[0] = { 0,     0, 100, 100 };
    gSpriteClips[1] = { 100,   0, 100, 100 };
    gSpriteClips[2] = { 0,   100, 100, 100 };
    gSpriteClips[3] = { 100, 100, 100, 100 };

    return true;
}


void close()
{
    // Free loaded images
    gSpriteSheetTexture.free();

    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // Quit SDL Subsystems
    IMG_Quit();
    SDL_Quit();
}
