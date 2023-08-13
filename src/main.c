#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

Vector2 ticks_per_quadrant;
Vector2 ticks_frequency;
const float TICK_THICKNESS = 1.5f;
const Color TICK_COLOR = {128, 128, 128, 125};
const Vector2 MAX_TICKS_PER_QUADRANT = {50, 50};
const Vector2 MIN_TICKS_PER_QUADRANT = {5, 5};

void grid_draw(const Vector2 pos, const Vector2 size)
{
    Vector2 mid = {
        pos.x + size.x / 2.0f,
        pos.y + size.y / 2.0f,
    };

    // Ensure that the ticks per quadrant is between the minimum and maximum amounts
    ticks_per_quadrant.x = fmin(ticks_per_quadrant.x, MAX_TICKS_PER_QUADRANT.x);
    ticks_per_quadrant.x = fmax(MIN_TICKS_PER_QUADRANT.x, ticks_per_quadrant.x);

    ticks_per_quadrant.y = fmin(ticks_per_quadrant.y, MAX_TICKS_PER_QUADRANT.y);
    ticks_per_quadrant.y = fmax(MIN_TICKS_PER_QUADRANT.y, ticks_per_quadrant.y);

    // Draw minor gridlines
    const Vector2 QUADRANT_TICK_OFFSETS = Vector2Divide(size, ticks_per_quadrant);
    Vector2 curr_pos, tick_start, tick_end;

    int i;
    // Negative x axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.x; i++) {
        if (i % (int)ticks_frequency.x == 0) {
            tick_start = (Vector2){curr_pos.x, pos.y};
            tick_end = (Vector2){curr_pos.x, pos.y + size.y};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.x -= QUADRANT_TICK_OFFSETS.x;
    }

    // Positive x axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.x; i++) {
        if (i % (int)ticks_frequency.x == 0) {
            tick_start = (Vector2){curr_pos.x, pos.y};
            tick_end = (Vector2){curr_pos.x, pos.y + size.y};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.x += QUADRANT_TICK_OFFSETS.x;
    }

    // Negative y axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.y; i++) {
        if (i % (int)ticks_frequency.y == 0) {
            tick_start = (Vector2){pos.x, curr_pos.y};
            tick_end = (Vector2){pos.x + size.x, curr_pos.y};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.y -= QUADRANT_TICK_OFFSETS.y;
    }

    // Positive y axis
    curr_pos = (Vector2){mid.x, mid.y};
    for (i = 0; i < ticks_per_quadrant.y; i++) {
        if (i % (int)ticks_frequency.y == 0) {
            tick_start = (Vector2){pos.x, curr_pos.y};
            tick_end = (Vector2){pos.x + size.x, curr_pos.y};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
        }

        curr_pos.y += QUADRANT_TICK_OFFSETS.y;
    }

    // Major axes
    DrawLineEx((Vector2){pos.x, mid.y}, (Vector2){pos.x + size.x, mid.y}, 3.0f, RAYWHITE); // x-axis
    DrawLineEx((Vector2){mid.x, pos.y}, (Vector2){mid.x, pos.y + size.y}, 3.0f, RAYWHITE); // y-axis
}

int main(void)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 800;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Plot GUI");

    const int FONT_SIZE = 20;
    Font font = LoadFontEx("resources/CM Serif Roman.ttf", FONT_SIZE, NULL, 0);

    ticks_per_quadrant = (Vector2){5, 5};
    ticks_frequency = Vector2One();

    while (!WindowShouldClose()) {
        float mouseWheel = GetMouseWheelMove();
        const Vector2 ZOOM_DIFF_TO_TICKS_DIFF = {2, 2};
        if (mouseWheel < 0.0f) {
            ticks_per_quadrant = Vector2Add(ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
        } else if (mouseWheel > 0.0f) {
            ticks_per_quadrant = Vector2Subtract(ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
        }

        if (ticks_per_quadrant.x > 15.0f) {
            ticks_frequency.x = 2;
        }
        if (ticks_per_quadrant.y > 15.0f) {
            ticks_frequency.y = 2;
        }

        BeginDrawing();
        ClearBackground((Color){20, 20, 20, 255});
        {
            Vector2 pos, size;
            pos.x = 0;
            pos.y = 0;
            size.x = (float)GetRenderWidth();
            size.y = (float)GetRenderHeight();
            grid_draw(pos, size);
        }
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
