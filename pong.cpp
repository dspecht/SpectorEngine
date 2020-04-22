struct Paddle
{
   // r32 width = 20.0f;
   // r32 height = 100.0f;
   // r32 originLocationHeight = (r32)(screenHeight / 2);
   // r32 moveSpeed = 10.0f;

    //vector2 tRight = toClipSpace2( (x/2) + (r32)(screenWidth/2),  (y/2) + (r32)(screenHeight/2));
    //vector2 bLeft  = toClipSpace2(-(x/2) + (r32)(screenWidth/2), -(y/2) + (r32)(screenHeight/2));

    vector2 dim = {10, 100};
    //r32 width = (20.0f / (r32)screenWidth);
    //r32 height = (100.0f / (r32)screenHeight);
    r32 originLocationHeight = (r32)(screenHeight / 2);
    r32 moveSpeed = 10.0f;
};

struct Ball
{
    r32 sideLength = 5.0f;
    vector2 locationOnScreen {};
    r32 moveSpeed = 5.0f;// look at having this changed based on hit and such
};

struct GameState
{
    Paddle playerPaddle;
    Paddle computerPaddle;
    Ball ball = {};
    bool gameInprogress = false;
    vector2 gameScore = {};
};

inline void
DrawPlayerPaddle(Paddle *paddle)
{
    vector3 color = {0.0f,0.0f,1.0f}; // Green
   // RenderRectViaPixels( \
   //         Vector2(startX,(paddle->originLocationHeight - (paddle->height / 2))), \
   //         Vector2(startX + paddle->width,(paddle->originLocationHeight + (paddle->height / 2))), \
   //         color);

    vector2 tRight = toClipSpace2( (paddle->dim.width/2),
                                   (paddle->dim.height/2) + paddle->originLocationHeight);

    vector2 bLeft  = toClipSpace2(-(paddle->dim.width/2),
                                  -(paddle->dim.height/2) + paddle->originLocationHeight);

    RenderRect(tRight, bLeft, color);
}

inline void
DrawComputerPaddle(Paddle *paddle)
{
    vector3 color = {0.0f,1.0f,0.0f}; // Green
   // RenderRectViaPixels( \
   //         Vector2(startX,(paddle->originLocationHeight - (paddle->height / 2))), \
   //         Vector2(startX - paddle->width,(paddle->originLocationHeight + (paddle->height / 2))), \
   //         color);

    vector2 tRight = toClipSpace2( (paddle->dim.width/2) + (r32)(screenWidth),
                                   (paddle->dim.height/2) + paddle->originLocationHeight);

    vector2 bLeft  = toClipSpace2(-(paddle->dim.width/2) + (r32)(screenWidth),
                                  -(paddle->dim.height/2) + paddle->originLocationHeight);
    RenderRect(tRight, bLeft, color);
}

inline void
DrawCenterLine()
{
    r32 lineWidth = 5.0f;
    vector3 color = {1.0f,0.0f,0.0f}; // Whjte

   // RenderRectViaPixels(Vector2(((screenWidth / 2.0f) - 0.5f),0.0f),
   //                     Vector2(((screenWidth / 2.0f) + 0.5f),(r32)screenHeight),
   //                     Vector3(1.0f,1.0f,1.0f));

    vector2 tRight = toClipSpace2( (lineWidth + (r32)screenWidth/2), 0);
    vector2 bLeft  = toClipSpace2(-(lineWidth + (r32)screenWidth/2), (r32)screenHeight);

    RenderRect(tRight, bLeft, color);
}

inline void
DrawBall(Ball *ball)
{
    if(!ball->locationOnScreen) {
        ball->locationOnScreen.x = (r32)(screenWidth / 2.0f);
        ball->locationOnScreen.y = (r32)(screenHeight / 2.0f);
    }

    ball->locationOnScreen = BoundToScreen(ball->locationOnScreen); // Make sure the ball is on the screen

    vector3 color = {1.0f,0.0f,0.0f};

   //RenderRectViaPixels(Vector2((ball->locationOnScreen.x - 4.0f),(ball->locationOnScreen.y - 4.0f)),
   //                    Vector2((ball->locationOnScreen.x + 4.0f),(ball->locationOnScreen.y + 4.0f)),
   //                    Vector3(1,0,0));

    RenderRect(
            toClipSpace2((ball->locationOnScreen.x + 4.0f), (ball->locationOnScreen.y + 4.0f)),
            toClipSpace2((ball->locationOnScreen.x - 4.0f), (ball->locationOnScreen.y - 4.0f)),
            color);
}

inline void
ResetBallPostition(Ball *ball)
{
    ball->locationOnScreen.x = (r32)(screenWidth / 2.0f);
    ball->locationOnScreen.y = (r32)(screenHeight / 2.0f);
}

inline void
gameInputHandling(GameState *state,u32 keyPressed, u8 wasDown, u8 isDown, LPARAM lParam)
{
    if(keyPressed == VK_UP || keyPressed == 'W')
    {
        state->playerPaddle.originLocationHeight = \
            BoundtoScreenY(state->playerPaddle.originLocationHeight + state->playerPaddle.moveSpeed);
        if(state->playerPaddle.originLocationHeight - ((state->playerPaddle.dim.height) /2.0f) <= 0)
            { state->playerPaddle.originLocationHeight = state->playerPaddle.dim.height /2.0f;}
    }
    else if(keyPressed == VK_DOWN || keyPressed == 'S')
    {
        state->playerPaddle.originLocationHeight = \
            BoundtoScreenY(state->playerPaddle.originLocationHeight - state->playerPaddle.moveSpeed);
        if(state->playerPaddle.originLocationHeight + ((state->playerPaddle.dim.height) /2.0f) >= screenHeight)
            { state->playerPaddle.originLocationHeight = (screenHeight - (state->playerPaddle.dim.height) /2.0f);}
    }
    else if(keyPressed == ' ' && !state->gameInprogress)
    {
        state->gameInprogress=true;
    }
}

inline void
update(GameState *state)
{
    glTranslatef(0.0f, 0.0f, -2.0f);
    localPersist vector2 ballMoveVector = {0,0};
    //clearBackgroundToColor(Vector3(0,0,0));
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    DrawCenterLine();
    DrawPlayerPaddle(&state->playerPaddle);
    DrawComputerPaddle(&state->computerPaddle);

    //TODO:Handle Ball Math
    if(state->gameInprogress)
    {
        if(ballMoveVector == Vector2(0,0))
        {
            ballMoveVector.x = -state->ball.moveSpeed;
        }
        if((state->ball.locationOnScreen.x <= 30
            && state->ball.locationOnScreen.x >= 20)
            || (state->ball.locationOnScreen.x >= (screenWidth-30)
            && state->ball.locationOnScreen.x <= (screenWidth-20)))
        {
            r32 playerPaddleTop= \
                state->playerPaddle.originLocationHeight + (state->playerPaddle.dim.height/2.0f);

            r32 playerPaddleBottom= \
                state->playerPaddle.originLocationHeight - (state->playerPaddle.dim.height/2.0f);

            r32 computerPaddleTop= \
                state->computerPaddle.originLocationHeight + (state->computerPaddle.dim.height/2.0f);

            r32 computerPaddleBottom= \
                state->computerPaddle.originLocationHeight - (state->computerPaddle.dim.height/2.0f);

            if((state->ball.locationOnScreen.y <= playerPaddleTop
               && state->ball.locationOnScreen.y >= playerPaddleBottom)
               || (state->ball.locationOnScreen.y <= computerPaddleTop
               && state->ball.locationOnScreen.y >= computerPaddleBottom))
            {
                    ballMoveVector = {-ballMoveVector.x, -ballMoveVector.y};
            }
        }

        state->ball.locationOnScreen += ballMoveVector; // moveball

        if(state->ball.locationOnScreen.x <= 0)
        {
            state->gameScore.y += 1;
            ResetBallPostition(&state->ball);
            if(state->gameScore.y == 10)
            {
                //TODO: Game over section (player Loses)
                state->gameInprogress=false;
                ballMoveVector = Vector2(0,0);
            }
        }
        else if(state->ball.locationOnScreen.y >= screenWidth)
        {
            state->gameScore.x += 1;
            ResetBallPostition(&state->ball);
            if(state->gameScore.x == 10)
            {
                //TODO: Game over section (player wins)
                state->gameInprogress=false;
                ballMoveVector = Vector2(0,0);
            }
        }
    }
    DrawBall(&state->ball);
}
