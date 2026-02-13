#include "Graphics.h"
#include <iostream>
#include <SDL_image.h>

SDL_Window* Graphics::window = NULL;
SDL_Renderer* Graphics::renderer = NULL;
int Graphics::windowWidth = 0;
int Graphics::windowHeight = 0;

int Graphics::Width() {
    return windowWidth;
}

int Graphics::Height() {
    return windowHeight;
}

bool Graphics::OpenWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = display_mode.w;
    windowHeight = display_mode.h;
    window = SDL_CreateWindow(NULL, 0, 0, windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
    if (!window) {
        std::cerr << "Error creating SDL window" << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer" << std::endl;
        return false;
    }

    ImGuiInit(window, renderer);

    return true;
}

void Graphics::ClearScreen(Uint32 color) {
    Uint8 r = (color >> 16) & 0xFF;
    Uint8 g = (color >> 8) & 0xFF;
    Uint8 b = (color) & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);
}

void Graphics::RenderFrame() {
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
}

void Graphics::DrawLine(int x0, int y0, int x1, int y1, Uint32 color) {
    lineColor(renderer, x0, y0, x1, y1, color);
}

void Graphics::DrawThickLine(int x0, int y0, int x1, int y1, Uint32 color, int thickness)
{
    float dx = float(x1 - x0);
    float dy = float(y1 - y0);
    float length = sqrtf(dx * dx + dy * dy);

    if (length == 0) return;
    
    float ux = -dy / length;
    float uy = dx / length;

    int half = thickness / 2;

    for (int i = -half; i <= half; i++)
    {
        int ox = int(ux * i + 0.5f);
        int oy = int(uy * i + 0.5f);
        lineColor(renderer, x0 + ox, y0 + oy, x1 + ox, y1 + oy, color);
    }
}


void Graphics::DrawCircle(int x, int y, int radius, float angle, Uint32 color) {
    circleColor(renderer, x, y, radius, color);
    lineColor(renderer, x, y, x + cos(angle) * radius, y + sin(angle) * radius, color);
}

void Graphics::DrawFillCircle(int x, int y, int radius, Uint32 color) {
    filledCircleColor(renderer, x, y, radius, color);
}

void Graphics::DrawRect(int x, int y, int width, int height, Uint32 color) {
    lineColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y - height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x + width / 2.0, y + height / 2.0, x - width / 2.0, y + height / 2.0, color);
    lineColor(renderer, x - width / 2.0, y + height / 2.0, x - width / 2.0, y - height / 2.0, color);
}

void Graphics::DrawFillRect(int x, int y, int width, int height, Uint32 color) {
    boxColor(renderer, x - width / 2.0, y - height / 2.0, x + width / 2.0, y + height / 2.0, color);
}

void Graphics::DrawPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    for (int i = 0; i < vertices.size(); i++) {
        int currIndex = i;
        int nextIndex = (i + 1) % vertices.size();
        lineColor(renderer, vertices[currIndex].x, vertices[currIndex].y, vertices[nextIndex].x, vertices[nextIndex].y, color);
    }
    filledCircleColor(renderer, x, y, 1, color);
}

void Graphics::DrawFillPolygon(int x, int y, const std::vector<Vec2>& vertices, Uint32 color) {
    std::vector<short> vx;
    std::vector<short> vy;
    for (int i = 0; i < vertices.size(); i++) {
        vx.push_back(static_cast<int>(vertices[i].x));
    }
    for (int i = 0; i < vertices.size(); i++) {
        vy.push_back(static_cast<int>(vertices[i].y));
    }
    filledPolygonColor(renderer, &vx[0], &vy[0], vertices.size(), color);
    filledCircleColor(renderer, x, y, 1, 0xFF000000);
}

void Graphics::DrawTexture(int x, int y, int width, int height, float rotation, SDL_Texture* texture) {
    SDL_Rect dstRect = { x - (width / 2), y - (height / 2), width, height };
    float rotationDeg = rotation * 57.2958;
    SDL_RenderCopyEx(renderer, texture, NULL, &dstRect, rotationDeg, NULL, SDL_FLIP_NONE);
}

void Graphics::ImGuiInit(SDL_Window* window, SDL_Renderer* renderer)
{
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
    ImGui::StyleColorsDark();
}

void Graphics::ImGuiShutdown()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Graphics::ImGuiNewFrame(SDL_Window* window)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void Graphics::CloseWindow(void) {

    ImGuiShutdown();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


SDL_Texture* Graphics::GetTexture(const char* texturePath)
{
    SDL_Texture* texture = nullptr;
    if (SDL_Surface* surface = IMG_Load(texturePath))
    {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }
    
    return texture;
}