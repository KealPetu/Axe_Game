//
// Created by Keal on 2/13/2026.
//
#include "raylib.h"
#include "raymath.h"

constexpr int SCREEN_WIDTH { 800 };
constexpr int SCREEN_HEIGHT { 450 };
constexpr float GRAVITY_ACC { 98.1f };
bool gameOver { false };

struct Square
{
    int width           { 0 };
    int height          { 0 };
    Vector2 position    { 0.0f, 0.0f };
    float velocity      { 0.f };
    Color color         { WHITE };
};

struct Circle
{
    float radius        { 0.f };
    Vector2 position    { 0.0f, 0.0f };
    float speed         { 0.f };
    Color color         { WHITE };
};

void updateBodies(Square &axe, Circle &player);
void resetBodies(Square &axe, Circle &player);
void restartGame(Square &axe, Circle &player);
bool haveCollided(const Square &axe, const Circle &player);
void drawBodies(const Square &axe, const Circle &player);

int main(int argc, char **argv)
{
    Square axe(50, 50, Vector2(SCREEN_WIDTH/2.f - 25, 0), 0.f, RED);
    Circle player(25, Vector2(SCREEN_WIDTH/2.f, SCREEN_HEIGHT/2.f), 200, DARKBLUE);

    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, *argv);

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            updateBodies(axe, player);
            if (haveCollided(axe, player)) { gameOver = true; }
        }

        if (IsKeyDown(KEY_R)) { restartGame(axe, player); }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            if (!gameOver) { drawBodies(axe, player); }
            else { DrawText("Game Over!", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 - 10, 20, RED); }
        EndDrawing();
    }

    return 0;
}

// Updates player vertical movement
void updatePlayerPosY(Circle& player, const float deltaTime)
{
    int yAxis { 0 };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { yAxis = -1; }
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { yAxis = 1; }

    player.position.y += deltaTime * player.speed * yAxis;
    player.position.y = Clamp(player.position.y, player.radius, SCREEN_HEIGHT - player.radius);
}

// Updates player horizontal movement
void updatePlayerPosX(Circle& player, const float deltaTime)
{
    int xAxis { 0 };
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) { xAxis = -1; }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { xAxis = 1; }

    player.position.x += player.speed * deltaTime * xAxis;
    player.position.x = Clamp(player.position.x, player.radius, SCREEN_WIDTH - player.radius);
}

// Updates the axe's vertical movement
void updateAxePosY(Square& axe, const float deltaTime)
{
    axe.velocity += GRAVITY_ACC * deltaTime;
    axe.position.y += axe.velocity * deltaTime;

    if (axe.position.y> SCREEN_HEIGHT - 50 || axe.position.y< 0)
    {
        axe.velocity *= -1;
    }
}

// Method in charge of updating the axe and the player's positions
void updateBodies(Square& axe, Circle& player)
{
    const float deltaTime { GetFrameTime() };
    updatePlayerPosY(player, deltaTime);
    updatePlayerPosX(player, deltaTime);
    updateAxePosY(axe, deltaTime);
}

// Checks if the axe and player have collided
bool haveCollided(const Square &axe, const Circle &player)
{
    return CheckCollisionCircleRec(
        player.position,
        player.radius,
        Rectangle{
            axe.position.x,
            axe.position.y,
            static_cast<float>(axe.width),
            static_cast<float>(axe.height)
        });
}

void drawBodies(const Square &axe, const Circle &player)
{
    DrawCircle(player.position.x, player.position.y, player.radius, player.color);
    DrawRectangle(axe.position.x, axe.position.y, axe.width, axe.height, axe.color);
}

void restartGame(Square &axe, Circle &player) {
    resetBodies(axe, player);
    gameOver = false;
}

void resetBodies(Square &axe, Circle &player) {
    axe.position.x = SCREEN_WIDTH/2.f - axe.width/2.f;
    axe.position.y = 0.f;
    axe.velocity = 0.f;

    player.position.x = SCREEN_WIDTH/2.f;
    player.position.y = SCREEN_HEIGHT/2.f;
};
