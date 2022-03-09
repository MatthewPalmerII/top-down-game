/* AUTHOR: MATTHEW PALMER
 * DATE: 03/05/2022
*/

#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
// create window width and height
const int windowWidth = 384;
const int windowHeight = 384;
// initialize the window
InitWindow(windowWidth, windowHeight, "Matt's Top Down Game!");

// load the world map
Texture2D map = LoadTexture("nature_tileset/WorldMap.png");
Vector2 mapPos{0.0, 0.0};
const float mapScale{4.0f};

// create instance of character class named knight
Character knight{windowWidth, windowHeight};

Prop props[2]{
    Prop{Vector2{550.f, 300.f}, LoadTexture("nature_tileset/Rock.png")},
    Prop{Vector2{400.f, 500.f}, LoadTexture("nature_tileset/Log.png")}
};

// create instance of a goblin
Enemy goblin{
    Vector2{800.f, 300.f}, 
    LoadTexture("characters/goblin_idle_spritesheet.png"), 
    LoadTexture("characters/goblin_run_spritesheet.png")
};

Enemy slime{
    Vector2{500.f, 700.f},
    LoadTexture("characters/slime_idle_spritesheet.png"),
    LoadTexture("characters/slime_run_spritesheet.png")
};

Enemy* enemies[]{
    &goblin,
    &slime
};

for(auto enemy : enemies)
{
    enemy->setTarget(&knight);
}

SetTargetFPS(60);
while(!WindowShouldClose())
{
    BeginDrawing();
    ClearBackground(WHITE);

    /* get the characters worldPos vector and flipping it. 
     * This will move mapPos in the opposite direction 
     * so the character will stay in the middle of the screen 
     * and the map will move. */
    mapPos = Vector2Scale(knight.getWorldPos(), -1.f);
    
    // draw the world map
    DrawTextureEx(map, mapPos, 0.0, mapScale, WHITE);
    
    // draw the props
    for (auto prop : props)
    {
        prop.Render(knight.getWorldPos());
    }

    if (!knight.getAlive()) // Character is not alive
    {
        DrawText("Game Over!", 55.f, 45.f, 40, RED);
        EndDrawing();
        continue;
    }
    else // Character is alive
    {
        std::string knightsHealth = "Health: ";
        knightsHealth.append(std::to_string(knight.getHealth()), 0, 5);
        DrawText(knightsHealth.c_str(), 55.f, 45.f, 40, RED);
    }

    // call the tick() function and pass in getFrameTime()
    knight.tick(GetFrameTime());
    

    // check the map boundaries
    if (knight.getWorldPos().x < 0.f ||
        knight.getWorldPos().y < 0.f ||
        knight.getWorldPos().x + windowWidth > map.width * mapScale ||
        knight.getWorldPos().y + windowHeight > map.height * mapScale)
    {
        knight.undoMovement();
    }

    /* loop through prop array to check collisions
     * use CheckCollisionRecs() and call getCollisionRec() for the prop and knight
     * then call undoMovement() if they collide */
    for(auto prop : props)
    {
        if (CheckCollisionRecs(prop.getCollisionRec(knight.getWorldPos()), knight.getCollisionRec()))
        {
            knight.undoMovement();
        }
    }

    // load enemies into world
    for (auto enemy : enemies)
    {
        enemy->tick(GetFrameTime());
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        for (auto enemy : enemies)
        {
           if (CheckCollisionRecs(enemy->getCollisionRec(), knight.getWeaponCollisionRec()))
            {
            enemy->setAlive(false);
            } 
        }
    }
    EndDrawing();
}


UnloadTexture(map);
CloseWindow();
}