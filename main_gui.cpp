#include "LibreSpriteCore.h"
#include <SDL2/SDL.h>
#include <iostream>

const int SPRITE_W = 32;
const int SPRITE_H = 32;
const int ZOOM = 16;
const int WIN_W = SPRITE_W * ZOOM;
const int WIN_H = SPRITE_H * ZOOM;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "LibreSprite Core - Native C++ Replica",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_W, WIN_H,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize our reverse-engineered C++ Sprite Document
    app::SpriteDoc doc(SPRITE_W, SPRITE_H);
    app::Color currentColor{255, 0, 0, 255}; // Default Red
    bool isDrawing = false;
    char currentTool = 'p'; // 'p' = pencil, 'e' = eraser

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                } else if (e.key.keysym.sym == SDLK_p) {
                    currentTool = 'p';
                    std::cout << "Tool: Pencil\n";
                } else if (e.key.keysym.sym == SDLK_e) {
                    currentTool = 'e';
                    std::cout << "Tool: Eraser\n";
                } else if (e.key.keysym.sym == SDLK_1) {
                    currentColor = {255, 0, 0, 255}; // Red
                    std::cout << "Color: Red\n";
                } else if (e.key.keysym.sym == SDLK_2) {
                    currentColor = {0, 255, 0, 255}; // Green
                    std::cout << "Color: Green\n";
                } else if (e.key.keysym.sym == SDLK_3) {
                    currentColor = {0, 0, 255, 255}; // Blue
                    std::cout << "Color: Blue\n";
                }
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                isDrawing = true;
                int x = e.button.x / ZOOM;
                int y = e.button.y / ZOOM;
                if (currentTool == 'p') {
                    doc.drawPencil(x, y, currentColor);
                } else {
                    doc.drawEraser(x, y);
                }
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                isDrawing = false;
            } else if (e.type == SDL_MOUSEMOTION && isDrawing) {
                int x = e.motion.x / ZOOM;
                int y = e.motion.y / ZOOM;
                if (currentTool == 'p') {
                    doc.drawPencil(x, y, currentColor);
                } else {
                    doc.drawEraser(x, y);
                }
            }
        }

        // Render background (dark gray checkered pattern)
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Draw sprite pixels
        const auto& img = doc.activeImage();
        for (int y = 0; y < SPRITE_H; ++y) {
            for (int x = 0; x < SPRITE_W; ++x) {
                app::Color c = img.getPixel(x, y);
                if (c.a > 0) {
                    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
                    SDL_Rect rect{x * ZOOM, y * ZOOM, ZOOM, ZOOM};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }

        // Draw grid lines
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        for (int x = 0; x <= SPRITE_W; ++x) {
            SDL_RenderDrawLine(renderer, x * ZOOM, 0, x * ZOOM, WIN_H);
        }
        for (int y = 0; y <= SPRITE_H; ++y) {
            SDL_RenderDrawLine(renderer, 0, y * ZOOM, WIN_W, y * ZOOM);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
