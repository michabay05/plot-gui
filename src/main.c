#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

Font font;

const Color BACKGROUND = {25, 25, 25, 255};

Vector2 ticks_per_quadrant;
int ticks_frequency;
const float TICK_THICKNESS = 1.0f;
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

    ticks_per_quadrant.y = (float)((int)(1.0f / ASPECT_RATIO() * ticks_per_quadrant.x));

    // Draw minor gridlines
    const Vector2 QUADRANT_TICK_OFFSETS = {
        (rect.width * 0.5f) / ticks_per_quadrant.x,
        (rect.height * 0.5f) / ticks_per_quadrant.y,
    };

    Vector2 tick_start, tick_end, text_pos;
    float negative_dir, positive_dir;
    const char* text;
    Vector2 text_dim;

    int i;
    // x-axis
    negative_dir = mid.x;
    positive_dir = mid.x;
    for (i = 0; i < ticks_per_quadrant.x; i++) {
        if (i != 0 && i % ticks_frequency == 0) {
            tick_start = (Vector2){negative_dir, rect.y};
            tick_end = (Vector2){negative_dir, rect.y + rect.height};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
            
            text = TextFormat("%d", -i);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){negative_dir - (text_dim.x / 2.0f), mid.y + 5.0f};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);

            tick_start = (Vector2){positive_dir, rect.y};
            tick_end = (Vector2){positive_dir, rect.y + rect.height};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
            
            text = TextFormat("%d", i);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){positive_dir - (text_dim.x / 2.0f), mid.y + 5.0f};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);
        }

        negative_dir -= QUADRANT_TICK_OFFSETS.x;
        positive_dir += QUADRANT_TICK_OFFSETS.x;
    }

    // y-axis
    negative_dir = mid.y;
    positive_dir = mid.y;
    for (i = 0; i < ticks_per_quadrant.y; i++) {
        if (i != 0 && i % ticks_frequency == 0) {
            tick_start = (Vector2){rect.x, negative_dir};
            tick_end = (Vector2){rect.x + rect.width, negative_dir};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
            
            text = TextFormat("%d", i);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){mid.x + 5.0f, negative_dir - (text_dim.y / 2.0f)};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);

            tick_start = (Vector2){rect.x, positive_dir};
            tick_end = (Vector2){rect.x + rect.width, positive_dir};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);
            
            text = TextFormat("%d", -i);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){mid.x + 5.0f, positive_dir - (text_dim.y / 2.0f)};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);
        }

        negative_dir -= QUADRANT_TICK_OFFSETS.y;
        positive_dir += QUADRANT_TICK_OFFSETS.y;
    }

    // Major axes
    DrawLineEx((Vector2){rect.x, mid.y}, (Vector2){rect.x + rect.width, mid.y}, 3.0f,
               RAYWHITE); // x-axis
    DrawLineEx((Vector2){mid.x, rect.y}, (Vector2){mid.x, rect.y + rect.height}, 3.0f,
               RAYWHITE); // y-axis
}

const float POINT_THICKNESS = 3.0f;

void plot_point(const Rectangle rect, float x, float y)
{
    Vector2 mid = {
        rect.x + rect.width / 2.0f,
        rect.y + rect.height / 2.0f,
    };
    
    const Vector2 QUADRANT_TICK_OFFSETS = {
        (rect.width * 0.5f) / ticks_per_quadrant.x,
        (rect.height * 0.5f) / ticks_per_quadrant.y,
    };

    DrawCircleV(Vector2Add(mid, Vector2Multiply(QUADRANT_TICK_OFFSETS, (Vector2){x, -y})), POINT_THICKNESS, RED);
}

void plot_points(const Rectangle rect, Vector2* points, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        plot_point(rect, points[i].x, points[i].y);
    }
}

int main(void)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Plot GUI");

    font = LoadFontEx("resources/CM Serif Roman.ttf", 20, NULL, 0);
    // SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    ticks_per_quadrant =
        (Vector2){MIN_TICKS_PER_QUADRANT, (int)(1.0f / ASPECT_RATIO() * MIN_TICKS_PER_QUADRANT)};
    ticks_frequency = 1;

    Rectangle grid_bounds;
    size_t num_of_points = 500;
    Vector2* points = calloc(num_of_points, sizeof(Vector2));

    float x = -10;
    for (size_t i = 0; i < num_of_points; i++) {
        points[i] = (Vector2){x, 5*x*x + 3*x + 2};
        x += 20.0f / num_of_points;
    }


    while (!WindowShouldClose()) {
        float mouse_wheel = GetMouseWheelMove();
        const Vector2 ZOOM_DIFF_TO_TICKS_DIFF = {2, 2};
        if (mouse_wheel < 0.0f) {
            ticks_per_quadrant = Vector2Add(ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
        } else if (mouse_wheel > 0.0f) {
            ticks_per_quadrant = Vector2Subtract(ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
        }
        
        if (ticks_per_quadrant.x > 35.0f) {
            ticks_frequency = 4;
        } else if (ticks_per_quadrant.x > 28.0f) {
            ticks_frequency = 3;
        } else if (ticks_per_quadrant.x > 15.0f) {
            ticks_frequency = 2;
        } else if (ticks_per_quadrant.x < 15.0f) {
            ticks_frequency = 1;
        }

        Vector2 mouse_delta = GetMouseDelta();
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !Vector2Equals(mouse_delta, Vector2Zero()))
            TraceLog(LOG_INFO, "Mouse delta: (%f, %f)", mouse_delta.x, mouse_delta.y);

        // ================================================================================

        BeginDrawing();
        ClearBackground(BACKGROUND);
        {
            grid_bounds.x = 0;
            grid_bounds.y = 0;
            grid_bounds.width = (float)GetRenderWidth() - grid_bounds.x;
            grid_bounds.height = (float)GetRenderHeight() - grid_bounds.y;
            grid_draw(grid_bounds);

            plot_points(grid_bounds, points, num_of_points);
        }
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
