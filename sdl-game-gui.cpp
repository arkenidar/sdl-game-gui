// https://claude.ai/chat/c9101bf5-9807-4f12-be2b-b2d186596edc

// SDL2 Game GUI with buttons and text rendering

// Requirements : compilation requires SDL2 and SDL2_ttf development libraries .

// On Windows with MSYS2 + MinGW64: pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf --needed
// On Debian/Ubuntu: sudo apt install libsdl2-dev libsdl2-ttf-dev
// On Fedora: sudo dnf install SDL2-devel SDL2_ttf-devel
// On Arch Linux: sudo pacman -S sdl2 sdl2_ttf
// On macOS: brew install sdl2 sdl2_ttf

// This C++ code compiles with GNU g++ and requires the SDL2 and SDL2_ttf libraries :
// g++ -o sdl-game-gui sdl-game-gui.cpp $(pkg-config --cflags --libs SDL2_ttf)

// Tested on GNU g++ :
// g++ compiles in Debian 12 GNU / Linux .
// g++ compiles in Windows 11 using MSYS2 + MinGW64 .
// Not tested yet on Ubuntu, Fedora, Arch Linux, macOS, but should work quite similarly .

// The code is a simple SDL2 game GUI with buttons and text rendering.
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <vector>

class Button
{
public:
    SDL_Rect rect;
    std::string text;
    bool isHovered;

    Button(int x, int y, int w, int h, const std::string &t)
        : rect{x, y, w, h}, text(t), isHovered(false) {}
};

class GameGUI
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    std::vector<Button> buttons;
    bool running;
    int score;

public:
    GameGUI() : window(nullptr), renderer(nullptr), font(nullptr), running(true), score(0) {}

    bool init()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
            return false;
        }

        if (TTF_Init() < 0)
        {
            std::cerr << "TTF initialization failed: " << TTF_GetError() << std::endl;
            return false;
        }

        window = SDL_CreateWindow(
            "Game GUI",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_SHOWN);

        if (!window)
        {
            std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer)
        {
            std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
            return false;
        }

        font = TTF_OpenFont("arial.ttf", 24);
        if (!font)
        {
            std::cerr << "Font loading failed: " << TTF_GetError() << std::endl;
            return false;
        }

        // Create buttons
        buttons.emplace_back(50, 50, 200, 50, "Start Game");
        buttons.emplace_back(50, 120, 200, 50, "Options");
        buttons.emplace_back(50, 190, 200, 50, "Quit");

        return true;
    }

    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEMOTION:
            {
                int mouseX = event.motion.x;
                int mouseY = event.motion.y;

                for (auto &button : buttons)
                {
                    button.isHovered = (mouseX >= button.rect.x &&
                                        mouseX <= button.rect.x + button.rect.w &&
                                        mouseY >= button.rect.y &&
                                        mouseY <= button.rect.y + button.rect.h);
                }
                break;
            }

            case SDL_MOUSEBUTTONDOWN:
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    for (size_t i = 0; i < buttons.size(); ++i)
                    {
                        if (mouseX >= buttons[i].rect.x &&
                            mouseX <= buttons[i].rect.x + buttons[i].rect.w &&
                            mouseY >= buttons[i].rect.y &&
                            mouseY <= buttons[i].rect.y + buttons[i].rect.h)
                        {

                            handleButtonClick(i);
                        }
                    }
                }
                break;
            }
            }
        }
    }

    void handleButtonClick(size_t buttonIndex)
    {
        switch (buttonIndex)
        {
        case 0: // Start Game
            score += 10;
            break;
        case 1: // Options
            // Add options logic here
            break;
        case 2: // Quit
            running = false;
            break;
        }
    }

    void render()
    {
        SDL_SetRenderDrawColor(renderer, 45, 45, 45, 255);
        SDL_RenderClear(renderer);

        // Render buttons
        for (const auto &button : buttons)
        {
            SDL_SetRenderDrawColor(renderer,
                                   button.isHovered ? 100 : 70,
                                   button.isHovered ? 100 : 70,
                                   button.isHovered ? 100 : 70,
                                   255);

            SDL_RenderFillRect(renderer, &button.rect);

            // Render button text
            SDL_Color textColor = {255, 255, 255, 255};
            // Use TTF_RenderText_Blended for anti-aliased text
            SDL_Surface *textSurface = TTF_RenderText_Blended(font, button.text.c_str(), textColor);
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_Rect textRect = {
                button.rect.x + (button.rect.w - textWidth) / 2,
                button.rect.y + (button.rect.h - textHeight) / 2,
                textWidth,
                textHeight};

            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        // Render score
        SDL_Color scoreColor = {255, 255, 255, 255};
        std::string scoreText = "Score: " + std::to_string(score);
        // Use TTF_RenderText_Blended for anti-aliased score text
        SDL_Surface *scoreSurface = TTF_RenderText_Blended(font, scoreText.c_str(), scoreColor);
        SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

        SDL_Rect scoreRect = {600, 50, scoreSurface->w, scoreSurface->h};
        SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        SDL_RenderPresent(renderer);
    }

    void run()
    {
        while (running)
        {
            handleEvents();
            render();
            SDL_Delay(16); // Cap at ~60 FPS
        }
    }

    ~GameGUI()
    {
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }
};

int main(int argc, char *argv[])
{
    GameGUI game;

    if (!game.init())
    {
        return 1;
    }

    game.run();
    return 0;
}