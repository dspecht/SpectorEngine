struct Paddle
{
    r32 width = 20.0f;
    r32 height = 100.0f;
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
    r32 startX = 10.0f;
    vector3 color = {0,1,0}; // Green
    RenderRectViaPixels( \
            Vector2(startX,(paddle->originLocationHeight - (paddle->height / 2))), \
            Vector2(startX + paddle->width,(paddle->originLocationHeight + (paddle->height / 2))), \
            color);
}

inline void
DrawComputerPaddle(Paddle *paddle)
{
    r32 startX = screenWidth - 10.0f;
    vector3 color = {0,1,0}; // Green
    RenderRectViaPixels( \
            Vector2(startX,(paddle->originLocationHeight - (paddle->height / 2))), \
            Vector2(startX - paddle->width,(paddle->originLocationHeight + (paddle->height / 2))), \
            color);
}

inline void
DrawCenterLine()
{
    RenderRectViaPixels(Vector2(((screenWidth / 2.0f) - 0.5f),0.0f),
                        Vector2(((screenWidth / 2.0f) + 0.5f),(r32)screenHeight),
                        Vector3(1.0f,1.0f,1.0f));
}

inline void
DrawBall(Ball *ball)
{
    if(!ball->locationOnScreen) {
        ball->locationOnScreen.x = (r32)(screenWidth / 2.0f);
        ball->locationOnScreen.y = (r32)(screenHeight / 2.0f);
    }
    ball->locationOnScreen = BoundToScreen(ball->locationOnScreen); // Make sure the ball is on the screen

    RenderRectViaPixels(Vector2((ball->locationOnScreen.x - 4.0f),(ball->locationOnScreen.y - 4.0f)),
                        Vector2((ball->locationOnScreen.x + 4.0f),(ball->locationOnScreen.y + 4.0f)),
                        Vector3(1,0,0));
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
            BoundtoScreenY(state->playerPaddle.originLocationHeight - state->playerPaddle.moveSpeed);
        if(state->playerPaddle.originLocationHeight - ((state->playerPaddle.height) /2.0f) <= 0)
            { state->playerPaddle.originLocationHeight = state->playerPaddle.height /2.0f;}
    }
    else if(keyPressed == VK_DOWN || keyPressed == 'S')
    {
        state->playerPaddle.originLocationHeight = \
            BoundtoScreenY(state->playerPaddle.originLocationHeight + state->playerPaddle.moveSpeed);
        if(state->playerPaddle.originLocationHeight + ((state->playerPaddle.height) /2.0f) >= screenHeight)
            { state->playerPaddle.originLocationHeight = (screenHeight - (state->playerPaddle.height) /2.0f);}
    }
    else if(keyPressed == ' ' && !state->gameInprogress)
    {
        state->gameInprogress=true;
    }
}

inline void
update(GameState *state)
{
    localPersist vector2 ballMoveVector = {0,0};
    clearBackgroundToColor(Vector3(0,0,0));
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
                state->playerPaddle.originLocationHeight + (state->playerPaddle.height/2.0f);
            r32 playerPaddleBottom= \
                state->playerPaddle.originLocationHeight - (state->playerPaddle.height/2.0f);
            r32 computerPaddleTop= \
                state->computerPaddle.originLocationHeight + (state->computerPaddle.height/2.0f);
            r32 computerPaddleBottom= \
                state->computerPaddle.originLocationHeight - (state->computerPaddle.height/2.0f);
            if((state->ball.locationOnScreen.y <= playerPaddleTop
               && state->ball.locationOnScreen.y >= playerPaddleBottom)
               || (state->ball.locationOnScreen.y <= computerPaddleTop
               && state->ball.locationOnScreen.y >= computerPaddleBottom))
            {
                    ballMoveVector = -ballMoveVector;
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
