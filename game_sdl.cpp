#include<iostream>

#include<SDL2/SDL.h>

#include "life.hh"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

struct context {

    const int num_cells;
    const int window_size;
    const int step;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect rect;
    bool running;
    bool evolving;
    Game game;

    context() : num_cells(10), window_size(500), step(window_size / num_cells)
                , window(nullptr), renderer(nullptr)
                , running(false), evolving(false)
                , game(num_cells, num_cells) { }

    void translate_mouse(int mouse_x, int mouse_y, int* row, int* col) const
    {
        *row = mouse_y / step;
        *col = mouse_x / step;
    }
};

void cleanup(context ctx)
{
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    SDL_Quit();
}

static inline bool key_up(SDL_Event e, SDL_Keycode key)
{
    return e.type == SDL_KEYUP && e.key.keysym.sym == key;
}

void loop_fn(void *data)
{
    context *ctx = static_cast<context *>(data);
    if (!ctx) {
        std::cerr << "PANIC: Loop context can't be null. Aborting..." << std::endl;
        abort();
    }
    static int i = 0;

    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        if (key_up(e, SDLK_ESCAPE) || e.type == SDL_QUIT) {
            ctx->running = false;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
            cleanup(*ctx);
#endif
            return;
        } else if (key_up(e, SDLK_SPACE)) {
            ctx->evolving = !ctx->evolving;
        } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
            /* std::cout << "BUTTON LEFT RELEASED!" << std::endl; */
            int clicked_row, clicked_col;
            ctx->translate_mouse(e.button.x, e.button.y, &clicked_row, &clicked_col);
            ctx->game.Toggle(clicked_row, clicked_col);
        }
    }

    int mouse_x;
    int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    static int hovered_row;
    static int hovered_col;
    int new_hovered_row;
    int new_hovered_col;

    const int cell_height = ctx->step;
    const int cell_width = ctx->step;

    ctx->translate_mouse(mouse_x, mouse_y, &new_hovered_row, &new_hovered_col);

    if (i > 1) {
        if (new_hovered_row != hovered_row || new_hovered_col != hovered_col) {
            SDL_Rect hovered_rect;
            hovered_rect.x = hovered_col * cell_width;
            hovered_rect.y = hovered_row * cell_height;
            hovered_rect.w = cell_width;
            hovered_rect.h = cell_height;
            bool current = ctx->game.At(hovered_row, hovered_col);
            if (current)
                SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, 255);
            else
                SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(ctx->renderer, &hovered_rect);
        }
    }

    hovered_row = new_hovered_row;
    hovered_col = new_hovered_col;


    /* std::cout << "Mouse at (" << mouse_x << " " << mouse_y << ")" << std::endl; */

    /* SDL_SetRenderDrawColor(ctx->renderer, 0, 255, 255, 100); */
    /* SDL_RenderClear(ctx->renderer); */



    for (int row = 0; row < ctx->game.GetRows(); row++) {
        for (int col = 0; col < ctx->game.GetCols(); col++) {
            /* std::cout << row << " " << col << std::endl; */
            SDL_Rect rect;
            rect.x = col * cell_width;
            rect.y = row * cell_height;
            rect.w = cell_width;
            rect.h = cell_height;

            bool current = ctx->game.At(row, col);
            if (i > 1) {
                bool previous = ctx->game.At(row, col, Previous);

                if (previous == current)
                    continue;
            }

            if (current)
                SDL_SetRenderDrawColor(ctx->renderer, 0, 0, 255, 255);
            else
                SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(ctx->renderer, &rect);
        }
    }

    SDL_Rect hovered_rect;
    hovered_rect.x = hovered_col * cell_width;
    hovered_rect.y = hovered_row * cell_height;
    hovered_rect.w = cell_width;
    hovered_rect.h = cell_height;
    SDL_SetRenderDrawBlendMode(ctx->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ctx->renderer, 255, 0, 0, 100);
    SDL_RenderFillRect(ctx->renderer, &hovered_rect);

    SDL_RenderPresent(ctx->renderer);

    // 0.5 fps
    /* if (ctx->evolving && i % 30 == 0) { */
        ctx->game.Evolve();
    /* } */

    i++;
    /* std::cout << "Rendering frame #" << i << std::endl; */
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    context ctx;

    if (!(ctx.window = SDL_CreateWindow("Game of Life", 100, 100, ctx.window_size, ctx.window_size, 0))) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(ctx.renderer = SDL_CreateRenderer(ctx.window, 1, SDL_RENDERER_ACCELERATED))) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    ctx.game.Toggle(1, 1);
    ctx.game.Toggle(1, 2);
    ctx.game.Toggle(1, 3);

    ctx.running = true;
    ctx.evolving = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(loop_fn, &ctx, 0, 1);
#else
    while (ctx.running) {
        loop_fn(&ctx);
        SDL_Delay(1000/60);
    }

    cleanup(ctx);
#endif

    return 0;
}
