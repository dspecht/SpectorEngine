#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#define GLEW_STATIC

#define toClipspace(clipSpaceX, clipSpaceY) = \
    (screenSpaceX * (2.0f/ screenWidth) - 1.0f, ScreenSpaceY * (-2.0f / screenHeight) + 1.0f)

globalVar GLdouble pi = 3.1415926535897932384626433832795;
globalVar GLuint GLBitmapFontBasePtr = NULL;

vector3
toClipSpace(u32 screenX, u32 screenY, r32 screenZ = 1.0f)
{
    vector3 result = {};
        result.x = screenX * (2.0f / screenWidth) -1.0f;
        result.y = screenY * (-2.0f / screenHeight) + 1.0f;
        result.z = screenZ;
    return result;
}

vector3
toClipSpace(vector2 screenCord, r32 screenZ = 1.0f)
{
    // TODO look at removing these casts if the vector structs are expanded.
    return toClipSpace((u32)screenCord.x, (u32)screenCord.y, screenZ);
}

void
gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{// function came from "http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/"
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) *zNear;
    fW = fH * aspect;

    glFrustum((-fW), fW, (-fH), fH, zNear, zFar); // the original just calls this anyway
}

void
CreateShaders()
{// TODO(dustin): Look at adding 3 filePaths for the 3 different shaders and have them compile from that file
    char *vertexShaderSource =
        "#version 450 core\n void main() {gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;}";

    char *fragmentShaderSource =
       "#version 450 core\n void main() {}";

    GLuint shaderProgramID = glCreateProgram();
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource((GLuint)vertexShaderSource, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShaderID);

    glShaderSource((GLuint)fragmentShaderID, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShaderID);

    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);

    glLinkProgram(shaderProgramID);
    glValidateProgram(shaderProgramID); //TODO: look at disabling this at some point
    glUseProgram(shaderProgramID);
}

void
InitOpenGL()
{
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    CreateShaders();
}

void
ResizeWindow(int width, int height)
{
    if(!height) {height = 1;}

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (width/height), 0.2f, 255.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    screenWidth = width;
    screenHeight = height;
}

void
RenderRectViaTriangles(vector3 vertA, vector3 vertB, vector3 vertC, vector3 vertD, vector3 color)
{ // Where vertB and vertC are the Diagonal
    vector3 Vertices[6] = { vertA, vertB, vertC, vertB, vertC, vertD };
    GLuint VBO = 0;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glColor3f(color.r, color.g, color.b);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}

void
RenderRect(vector3 bLeft, vector3 tLeft, vector3 tRight, vector3 bRight, vector3 color=Vector3(0,0,0))
{
    vector3 Vertices[4] = {bLeft, tLeft, tRight, bRight};
    GLuint VBO = 0;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glColor3f(color.r, color.g, color.b);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableVertexAttribArray(0);
}

void
RenderRect(vector2 bLeft, vector2 tLeft, vector2 tRight, vector2 bRight, vector3 color=Vector3(0,0,0))
{
    vector3 bleft3 = {bLeft.x,bLeft.y,1.0f};
    vector3 tleft3 = {tLeft.x,tLeft.y,1.0f};
    vector3 tright3 = {tRight.x,tRight.y,1.0f};
    vector3 bright3 = {bRight.x,bRight.y,1.0f};
    RenderRect(bleft3,tleft3,tright3,bright3,color);
}

void
RenderRectViaPixels(vector2 bLeft, vector2 tLeft, vector2 tRight, vector2 bRight, vector3 color=Vector3(0,0,0))
{
    RenderRect(toClipSpace(bLeft),toClipSpace(tLeft),toClipSpace(tRight),toClipSpace(bRight),color);
}

void
RenderRectViaPixels(vector2 bottomLeft, vector2 topRight, vector3 color=Vector3(0,0,0))
{
    vector2 topLeft = {bottomLeft.x,topRight.y};
    vector2 bottomRight = {topRight.x,bottomLeft.y};
    RenderRectViaPixels(bottomLeft,topLeft,topRight,bottomRight,color);
}

void
RenderTriangle(vector3 vertA, vector3 vertB, vector3 vertC, vector3 color=Vector3(0, 1, 0))
{
    vector3 Vertices[3] = { vertA, vertB, vertC };
    GLuint VBO = 0;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glColor3f(color.r, color.g, color.b);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void
DEBUG_RenderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    vector3 topLeft =       toClipSpace(0, screenHeight);
    vector3 topRight =      toClipSpace(screenWidth, screenHeight);
    vector3 bottomLeft =    toClipSpace(0,0);
    vector3 bottomRight =   toClipSpace(screenWidth, 0);

    RenderRect(bottomLeft,topLeft,topRight,bottomRight,  Vector3(0,255,0));
    //RenderRectViaPixels(Vector2(((screenWidth / 2.0f) - 0.5f),0.0f),
    //                    Vector2(((screenWidth / 2.0f) + 0.5f),(r32)screenHeight),
    //                    Vector3(1.0f,1.0f,1.0f));

    //RenderRectViaPixels(Vector2(10, 50), Vector2(30, 150), Vector3(0,1,0));
    //RenderRectViaPixels(Vector2((r32)(screenWidth-10),(r32)(screenHeight-50)), \
    //        Vector2((r32)(screenWidth-30), (r32)(screenHeight-150)), Vector3(0,1,0));
}
