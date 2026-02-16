//
// Created by Keal on 2/13/2026.
//
#include "raylib.h"
#include "raymath.h"

constexpr int SCREEN_WIDTH { 800 };
constexpr int SCREEN_HEIGHT { 450 };
constexpr float GRAVITY_ACC { 98.1f };

struct Square
{
    int width       { 0 };
    int height      { 0 };
    int posX        { 0 };
    int posY        { 0 };
    float velocity  { 0 };
    Color color     { WHITE };
};

struct Circle
{
    float radius    { 0.f };
    int posX        { 0 };
    int posY        { 0 };
    int speed       { 0 };
    Color color     { WHITE };
};

void drawFrame(const Square &axe, const Circle &player);
void updateBodies(Square &axe, Circle &player);
bool haveCollided(Square axe, Circle player);

int main(int argc, char **argv)
{
    Square axe(50, 50, (SCREEN_WIDTH/2) - 25, 0, 0.f, RED);
    Circle player(25, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 200, DARKBLUE);
    bool gameOver { false };

    SetConfigFlags(FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, *argv);

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            updateBodies(axe, player);
            if (haveCollided(axe, player))
            {
                gameOver = true;
            }
        }

        BeginDrawing();
        if (!gameOver)
        {
            drawFrame(axe, player);
        }
        else
        {
            ClearBackground(RAYWHITE);
            DrawText("Game Over!", SCREEN_WIDTH/2 - 50, SCREEN_HEIGHT/2 - 10, 20, RED);
        }
        EndDrawing();
    }

    return 0;
}

void updatePlayerPosY(Circle& player, const float deltaTime)
{
    // Circle vertical movement Logic
    int yAxis { 0 };
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { yAxis = -1; }
    else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { yAxis = 1; }

    player.posY += deltaTime * player.speed * yAxis;
    player.posY = Clamp(player.posY, player.radius, SCREEN_HEIGHT - player.radius);
}

void updatePlayerPosX(Circle& player, const float deltaTime)
{
    //Circle horizontal movement logic
    int xAxis { 0 };
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) { xAxis = -1; }
    else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { xAxis = 1; }

    player.posX += player.speed * deltaTime * xAxis;
    player.posX = Clamp(player.posX, player.radius, SCREEN_WIDTH - player.radius);
}

void updateAxePosY(Square& axe, const float deltaTime)
{
    //  Axe movement Logic
    axe.velocity += GRAVITY_ACC * deltaTime;
    axe.posY += axe.velocity * deltaTime;

    if (axe.posY > SCREEN_HEIGHT - 50 || axe.posY < 0)
    {
        axe.velocity *= -1;
    }
}

bool haveCollided(Square axe, Circle player)
{
    const int bAxeY {axe.posY + axe.height};
    const int uAxeY {axe.posY};
    const int lAxeX {axe.posX};
    const int rAxeX {axe.posX + axe.width};

    const int uPlayerY {static_cast<int>(player.posY - player.radius)};
    const int bPlayerY {static_cast<int>(player.posY + player.radius)};
    const int rPlayerX {static_cast<int>(player.posX + player.radius)};
    const int lPlayerX {static_cast<int>(player.posX - player.radius)};

    const bool collision {bAxeY >= uPlayerY && uAxeY <= bPlayerY && lAxeX <= rPlayerX && rAxeX >= lPlayerX};

    return collision;
}

void updateBodies(Square& axe, Circle& player)
{
    const float deltaTime { GetFrameTime() };
    updatePlayerPosY(player, deltaTime);
    updatePlayerPosX(player, deltaTime);
    updateAxePosY(axe, deltaTime);
}

void drawFrame(const Square &axe, const Circle &player)
{
    ClearBackground(RAYWHITE);
    DrawCircle(player.posX, player.posY, player.radius, player.color);
    DrawRectangle(axe.posX, axe.posY, axe.width, axe.height, axe.color);
}
