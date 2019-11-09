#include "spectorengine.h"

r32 BoundtoScreenY(r32 y)
{
    if(y < 0.0f) { y = 0.0f; }
    else if (y > screenHeight) { y = (r32)screenHeight; }
    return y;
}

r32 BoundtoScreenX(r32 x)
{
    if(x < 0.0f) { x = 0.0f; }
    else if (x > screenWidth) { x = (r32)screenWidth; }
    return x;
}

vector2 BoundToScreen(vector2 cords)
{
    cords.x = BoundtoScreenX(cords.x);
    cords.y = BoundtoScreenY(cords.y);
    return cords;
}

