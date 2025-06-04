#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Init SDL systems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 ||
        IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG ||
        TTF_Init() == -1 ||
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Initialization error: " << SDL_GetError() << "\n";
        return 1;
    }

    // Create window + renderer
    SDL_Window* window = SDL_CreateWindow("SDL2 Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load image texture
    SDL_Texture* image = IMG_LoadTexture(renderer, "spaceship.png");  // PNG file in same folder
    if (!image) {
        std::cerr << "Image load error: " << IMG_GetError() << "\n";
    }

    // Load font and create a texture from text
    TTF_Font* font = TTF_OpenFont("OpenSans-Regular.ttf", 28); // Include a .ttf font
    SDL_Color white = {255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Hello SDL2", white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {50, 50, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);

    // Load sound
    Mix_Chunk* sound = Mix_LoadWAV("Movement2.wav"); // Include a short sound effect

    bool quit = false;
    SDL_Event event;

    Uint32 startTime = SDL_GetTicks(); // Timing

    while (!quit) {
        // Event loop
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    Mix_PlayChannel(-1, sound, 0); // Play sound on spacebar
                }
            }
        }

        // Time tracking
        Uint32 elapsedTime = SDL_GetTicks() - startTime;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(renderer);

        // Draw filled rectangle
        SDL_Rect rect = {300, 250, 200, 100};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderFillRect(renderer, &rect);

        // Draw image
        if (image) {
            SDL_Rect imgRect = {550, 400, 200, 150};
            SDL_RenderCopy(renderer, image, NULL, &imgRect);
        }

        // Draw text
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        // Present the frame
        SDL_RenderPresent(renderer);

        // Cap FPS (rudimentary)
        SDL_Delay(16); // ~60 FPS
    }

    // Cleanup
    Mix_FreeChunk(sound);
    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(image);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
