#include "BaseCharacter.h"
#include "raylib.h"
#include "raymath.h"

BaseCharacter::BaseCharacter()
{

}
void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{
        getScreenPos().x, getScreenPos().y, width * scale, height * scale};
}

void BaseCharacter::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    // update animation frame
    runningTime += deltaTime;
    // cycle through the sprite sheet
    if (runningTime >= updateTime)
    {
        frame++; // increment frame to move to next frame in the line
        runningTime = 0.f;
        if (frame > maxFrames)
            frame = 0; // once frame is greater than the max frames in sheet, reset to 0 and start loop again
    }
    
    if (Vector2Length(velocity) != 0.0)
    {
        /* makes the map move. Vector2Sub: substracts position of the map with the direction.
         * Vector2Scale: scales direction by the float speed
         *Vector2Normalize: normalized the number, changes from a ~1.4... to a 1
         * set worldPos = worldPos - velocity */
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));

        // set right/left variable. This is the same as an if/else statement
        velocity.x < 0.f ? rightLeft = -1.f : rightLeft = 1.f;
        // if we are moving knight will be running
        texture = run;
    }
    else
    {
        // else the knight is not moving and is idle
        texture = idle;
    }
    velocity = {};

    // draw the character
    // Rectangle source for the source section of DrawTexturePro
    Rectangle source{frame * width, 0.f, rightLeft * width, height};
    // Rectangle Dest for the destination section of DrawTexturePro
    Rectangle dest{getScreenPos().x, getScreenPos().y, scale * width, scale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}