#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

Vector2 ticks_per_quadrant;
int ticks_frequency;
const float TICK_THICKNESS = 1.5f;
const Color TICK_COLOR = {128, 128, 128, 125};
const float MIN_TICKS_PER_QUADRANT = 10;
const float MAX_TICKS_PER_QUADRANT = 50;

#define ASPECT_RATIO() ((float)GetRenderWidth() / (float)GetRenderHeight())

void grid_draw(const Rectangle rect)
{
    Vector2 mid = {
        rect.x + rect.width / 2.0f,
        rect.y + rect.height / 2.0f,
    };

    // Ensure that the ticks per quadrant is between the minimum and maximum amounts
    ticks_per_quadrant.x = fmin(ticks_per_quadrant.x, MAX_TICKS_PER_QUADRANT);
    ticks_per_quadrant.x = fmax(MIN_TICKS_PER_QUADRANT, ticks_per_quadrant.x);

    ticks_per_quadrant.y = (float)((int)(1.0f / ASPECT_RATIO() * MIN_TICKS_PER_QUADRANT));

    // Draw minor gridlines
    const Vector2 QUADRANT_TICK_OFFSETS = {
        (rect.width * 0.5f) / ticks_per_quadrant.x,
        (rect.height * 0.5f) / ticks_per_quadrant.y,
    };

    Vector2 curr_pos, tick_start, tick_end;

    int i;
    // Negative x axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.x; i++) {
        if (i % ticks_frequency == 0) {
            tick_start = (Vector2){curr_pos.x, rect.y};
            tick_end = (Vector2){curr_pos.x, rect.y + rect.height};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.x -= QUADRANT_TICK_OFFSETS.x;
    }

    // Positive x axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.x; i++) {
        if (i % ticks_frequency == 0) {
            tick_start = (Vector2){curr_pos.x, rect.y};
            tick_end = (Vector2){curr_pos.x, rect.y + rect.height};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.x += QUADRANT_TICK_OFFSETS.x;
    }

    // Negative y axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.y; i++) {
        if (i % ticks_frequency == 0) {
            tick_start = (Vector2){rect.x, curr_pos.y};
            tick_end = (Vector2){rect.x + rect.width, curr_pos.y};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.y -= QUADRANT_TICK_OFFSETS.y;
    }

    // Positive y axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.y; i++) {
        if (i % ticks_frequency == 0) {
            tick_start = (Vector2){rect.x, curr_pos.y};
            tick_end = (Vector2){rect.x + rect.width, curr_pos.y};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.y += QUADRANT_TICK_OFFSETS.y;
    }

    // Major axes
    DrawLineEx((Vector2){rect.x, mid.y}, (Vector2){rect.x + rect.width, mid.y}, 3.0f,
               RAYWHITE); // x-axis
    DrawLineEx((Vector2){mid.x, rect.y}, (Vector2){mid.x, rect.y + rect.height}, 3.0f,
               RAYWHITE); // y-axis
}


int main(void)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Plot GUI");

    const int FONT_SIZE = 20;
    Font font = LoadFontEx("resources/CM Serif Roman.ttf", FONT_SIZE, NULL, 0);

    ticks_per_quadrant =
        (Vector2){MIN_TICKS_PER_QUADRANT, (int)(1.0f / ASPECT_RATIO() * MIN_TICKS_PER_QUADRANT)};
    ticks_frequency = 1;

    while (!WindowShouldClose()) {
        float mouseWheel = GetMouseWheelMove();
        const Vector2 ZOOM_DIFF_TO_TICKS_DIFF = {2, 2};
        if (mouseWheel < 0.0f) {
            ticks_per_quadrant = Vector2Add(ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
        } else if (mouseWheel > 0.0f) {
            ticks_per_quadrant = Vector2Subtract(ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
        }

        if (ticks_per_quadrant.x > 15.0f || ticks_per_quadrant.y > 15.0f) {
            ticks_frequency = 2;
        }

        BeginDrawing();
        ClearBackground((Color){20, 20, 20, 255});
        {
            Rectangle bounds;
            bounds.x = 0;
            bounds.y = 0;
            bounds.width = (float)GetRenderWidth();
            bounds.height = (float)GetRenderHeight();
            grid_draw(bounds);
        }
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
