#include "./raylib/src/raylib.h"
#include "./raylib/src/raymath.h"
#include <cmath>

constexpr int NUM_ROWS = 10;
constexpr int NUM_COLS = 10;
const static int map[][NUM_COLS] = {
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
    { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

constexpr int SCREEN_WIDTH = 1600;
constexpr int SCREEN_HEIGHT = 900;
constexpr int TILE_SIZE = 100;
constexpr float MINIMAP_SCALE = 0.2f;
constexpr int TRIANGLE_SIZE = 25 * MINIMAP_SCALE;

typedef struct MapCoords {
    int x;
    int y;
} MapCoords;

typedef struct Position {
    float x;
    float y;

    MapCoords getMapCoords() const {
        return { static_cast<int>(x / TILE_SIZE), static_cast<int>(y / TILE_SIZE) };
    }
} Position;

constexpr float SPEED = 0.01f;
constexpr float ROTATION_SPEED = 0.001f;
typedef struct Player {
    Position position;
    float angle;

    void move_forwards(float speed) {
        position.x += speed * cosf(angle);
        position.y += speed * sinf(angle);
    }

    void move_backwards(float speed) {
        position.x -= speed * cosf(angle);
        position.y -= speed * sinf(angle);
    }
} Player;

Vector2 RotatePoint(Vector2 point, Position center, float angle) {
    float x = point.x - center.x;
    float y = point.y - center.y;
    float rotated_x = x * cosf(angle) - y * sinf(angle);
    float rotated_y = x * sinf(angle) + y * cosf(angle);
    return { rotated_x + center.x, rotated_y + center.y };
}

int main() {
    constexpr float RIGHT_FACING_ANGLE = 0.f;
    Player player = { Position { TILE_SIZE * (NUM_COLS / 2.f), TILE_SIZE * (NUM_ROWS / 2.f) }, RIGHT_FACING_ANGLE };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raycasting");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Draw minimap
        for (int i = 0; i < NUM_ROWS; i++) {
            for (int j = 0; j < NUM_COLS; j++) {
                if (map[i][j] == 1) {
                    DrawRectangle(
                        j * TILE_SIZE * MINIMAP_SCALE,
                        i * TILE_SIZE * MINIMAP_SCALE,
                        TILE_SIZE * MINIMAP_SCALE,
                        TILE_SIZE * MINIMAP_SCALE,
                        YELLOW);
                }
            }
        }

        // Draw player in minimap
        const Position triangle_position = { player.position.x * MINIMAP_SCALE, player.position.y * MINIMAP_SCALE };
        DrawTriangle(
            RotatePoint(Vector2 { triangle_position.x + TRIANGLE_SIZE, triangle_position.y }, triangle_position, player.angle),
            RotatePoint(Vector2 { triangle_position.x - TRIANGLE_SIZE, triangle_position.y - TRIANGLE_SIZE }, triangle_position, player.angle),
            RotatePoint(Vector2 { triangle_position.x - TRIANGLE_SIZE, triangle_position.y + TRIANGLE_SIZE }, triangle_position, player.angle),
            RED);

        // Handle player movement
        if (IsKeyDown(KEY_UP)) {
            player.move_forwards(SPEED);
        }
        if (IsKeyDown(KEY_DOWN)) {
            player.move_backwards(SPEED);
        }
        if (IsKeyDown(KEY_LEFT)) {
            player.angle -= fmod(ROTATION_SPEED, (2 * PI));
        }
        if (IsKeyDown(KEY_RIGHT)) {
            player.angle += fmod(ROTATION_SPEED, (2 * PI));
        }

        EndDrawing();
    }
}
