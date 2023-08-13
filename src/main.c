#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

Font font;

const Color BACKGROUND = {25, 25, 25, 255};

const float TICK_THICKNESS = 1.0f;
const Color TICK_COLOR = {128, 128, 128, 125};
const float MIN_TICKS_PER_QUADRANT = 10;
const float MAX_TICKS_PER_QUADRANT = 50;

#define ASPECT_RATIO() ((float)GetRenderWidth() / (float)GetRenderHeight())

typedef struct
{
    Vector2 ticks_per_quadrant;
    Vector2 total_ticks;
    int tick_freq;

    double min_x;
    double max_x;
    double min_y;
    double max_y;

    Vector2 origin;
} CoordPlane;

void grid_draw(const Rectangle rect, const CoordPlane cp)
{
    const Vector2 TICK_OFFSETS = {
        rect.width / cp.total_ticks.x,
        rect.height / cp.total_ticks.y,
    };

    Vector2 tick_start, tick_end, text_pos;
    float negative_dir, positive_dir, negative_num, positive_num;
    const char* text;
    Vector2 text_dim;

    int i;
    negative_dir = cp.origin.x;
    negative_num = 0;
    positive_dir = cp.origin.x;
    positive_num = 0;
    for (i = 0; i < cp.ticks_per_quadrant.x; i++) {
        if (i != 0 && i % cp.tick_freq == 0) {
            tick_start = (Vector2){negative_dir, rect.y};
            tick_end = (Vector2){negative_dir, rect.y + rect.height};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);

            text = TextFormat("%d", (int)negative_num);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){negative_dir - (text_dim.x / 2.0f), cp.origin.y + 5.0f};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);

            tick_start = (Vector2){positive_dir, rect.y};
            tick_end = (Vector2){positive_dir, rect.y + rect.height};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);

            text = TextFormat("%d", (int)positive_num);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){positive_dir - (text_dim.x / 2.0f), cp.origin.y + 5.0f};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);
        }

        negative_dir -= TICK_OFFSETS.x;
        positive_dir += TICK_OFFSETS.x;
        if (negative_num > cp.min_x)
            negative_num--;
        if (positive_num < cp.max_x)
            positive_num++;
    }

    negative_dir = cp.origin.y;
    negative_num = 0;
    positive_dir = cp.origin.y;
    positive_num = 0;
    for (i = 0; i < cp.ticks_per_quadrant.y; i++) {
        if (i != 0 && i % cp.tick_freq == 0) {
            tick_start = (Vector2){rect.x, negative_dir};
            tick_end = (Vector2){rect.x + rect.width, negative_dir};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);

            text = TextFormat("%d", (int)negative_num);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){cp.origin.x + 5.0f, negative_dir - (text_dim.x / 2.0f)};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);

            tick_start = (Vector2){rect.x, positive_dir};
            tick_end = (Vector2){rect.x + rect.width, positive_dir};
            DrawLineEx(tick_start, tick_end, TICK_THICKNESS, TICK_COLOR);

            text = TextFormat("%d", (int)positive_num);
            text_dim = MeasureTextEx(font, text, font.baseSize, 0);
            text_pos = (Vector2){cp.origin.x + 5.0f, positive_dir - (text_dim.x / 2.0f)};
            DrawRectangleV(text_pos, text_dim, BACKGROUND);
            DrawTextEx(font, text, text_pos, font.baseSize, 0, WHITE);
        }

        negative_dir -= TICK_OFFSETS.y;
        positive_dir += TICK_OFFSETS.y;
        if (negative_num > cp.min_y)
            negative_num--;
        if (positive_num < cp.max_y)
            positive_num++;
    }

    DrawLineEx((Vector2){rect.x, cp.origin.y}, (Vector2){rect.x + rect.width, cp.origin.y}, 3.0f,
               RAYWHITE); // x-axis
    DrawLineEx((Vector2){cp.origin.x, rect.y}, (Vector2){cp.origin.x, rect.y + rect.height}, 3.0f,
               RAYWHITE); // y-axis
}

const float POINT_THICKNESS = 3.0f;

void plot_point(const Rectangle rect, const CoordPlane cp, float x, float y)
{
    const Vector2 TICK_OFFSETS = {
        rect.width / cp.total_ticks.x,
        rect.height / cp.total_ticks.y,
    };

    DrawCircleV(Vector2Add(cp.origin, Vector2Multiply(TICK_OFFSETS, (Vector2){x, -y})),
                POINT_THICKNESS, RED);
}

void plot_points(const Rectangle rect, const CoordPlane cp, const Vector2* const points,
                 size_t size)
{
#if 1
    const Vector2 TICK_OFFSETS = {
        rect.width / cp.total_ticks.x,
        rect.height / cp.total_ticks.y,
    };
    
    for (size_t i = 1; i < size; i++) {
        DrawLineEx(
            Vector2Add(cp.origin, Vector2Multiply(TICK_OFFSETS, (Vector2){points[i-1].x, -points[i-1].y})),
            Vector2Add(cp.origin, Vector2Multiply(TICK_OFFSETS, (Vector2){points[i].x, -points[i].y})),
            2.25f,
            RED
        );
    }
#else
    for (size_t i = 0; i < size; i++) {
        plot_point(rect, cp, points[i].x, points[i].y);
    }
#endif
}

void update(CoordPlane* cp)
{
    float mouse_wheel = GetMouseWheelMove();
    const Vector2 ZOOM_DIFF_TO_TICKS_DIFF = {1, 1};
    if (mouse_wheel < 0.0f) {
        cp->ticks_per_quadrant = Vector2Add(cp->ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
    } else if (mouse_wheel > 0.0f) {
        cp->ticks_per_quadrant = Vector2Subtract(cp->ticks_per_quadrant, ZOOM_DIFF_TO_TICKS_DIFF);
    }
    cp->min_x = -cp->ticks_per_quadrant.x;
    cp->max_x = cp->ticks_per_quadrant.x;
    cp->min_y = -cp->ticks_per_quadrant.y;
    cp->max_y = cp->ticks_per_quadrant.y;

    if (cp->ticks_per_quadrant.x > 35.0f) {
        cp->tick_freq = 4;
    } else if (cp->ticks_per_quadrant.x > 28.0f) {
        cp->tick_freq = 3;
    } else if (cp->ticks_per_quadrant.x > 15.0f) {
        cp->tick_freq = 2;
    } else if (cp->ticks_per_quadrant.x < 15.0f) {
        cp->tick_freq = 1;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        cp->origin = Vector2Add(cp->origin, GetMouseDelta());
    }

    // Ensure that the ticks per quadrant is between the minimum and maximum amounts
    cp->ticks_per_quadrant.x = fmin(cp->ticks_per_quadrant.x, MAX_TICKS_PER_QUADRANT);
    cp->ticks_per_quadrant.x = fmax(MIN_TICKS_PER_QUADRANT, cp->ticks_per_quadrant.x);

    cp->ticks_per_quadrant.y = (float)((int)(1.0f / ASPECT_RATIO() * cp->ticks_per_quadrant.x));

    cp->total_ticks = Vector2Scale(cp->ticks_per_quadrant, 2.0f);
}

int main(void)
{
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Plot GUI");

    font = LoadFontEx("resources/CM Serif Roman.ttf", 20, NULL, 0);
    // SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(font.texture, TEXTURE_FILTER_POINT);

    Vector2 ticks =
        (Vector2){MIN_TICKS_PER_QUADRANT, (int)(1.0f / ASPECT_RATIO() * MIN_TICKS_PER_QUADRANT)};
    Vector2 total_ticks = Vector2Scale(ticks, 2);
    CoordPlane cp = {
        .ticks_per_quadrant = ticks,
        .total_ticks = total_ticks,
        .tick_freq = 1,
        .min_x = -ticks.x,
        .max_x = ticks.x,
        .min_y = -ticks.y,
        .max_y = ticks.y,
        .origin = {GetRenderWidth() / 2.0f, GetRenderHeight() / 2.0f},
    };

    Rectangle grid_bounds;
    size_t num_of_points = 500;
    Vector2* points = calloc(num_of_points, sizeof(Vector2));

    float x = -10;
    for (size_t i = 0; i < num_of_points; i++) {
        points[i] = (Vector2){x, x * x + 3};
        x += 20.0f / num_of_points;
    }

    while (!WindowShouldClose()) {
        update(&cp);

        BeginDrawing();
        ClearBackground(BACKGROUND);
        {
            grid_bounds.x = 0;
            grid_bounds.y = 0;
            grid_bounds.width = (float)GetRenderWidth() - grid_bounds.x;
            grid_bounds.height = (float)GetRenderHeight() - grid_bounds.y;

            grid_draw(grid_bounds, cp);
            plot_points(grid_bounds, cp, points, num_of_points);
        }
        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}
