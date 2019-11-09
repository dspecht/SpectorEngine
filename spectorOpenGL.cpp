#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
//#define GLEW_STATIC

#define toClipspace(clipSpaceX, clipSpaceY) = \
    (screenSpaceX * (2.0f/ screenWidth) - 1.0f, ScreenSpaceY * (-2.0f / screenHeight) + 1.0f)

globalVar GLdouble pi = 3.1415926535897932384626433832795;
GLuint GLBitmapFontBasePtr = NULL;

vector3 toClipSpace(u32 screenX, u32 screenY, r32 screenZ = 0.0f)
{
    vector3 result = {};
        result.x = screenX * (2.0f / screenWidth) -1.0f;
        result.y = screenY * (-2.0f / screenHeight) + 1.0f;
        result.z = screenZ;
    return result;
}

vector3 toClipSpace(vector2 screenCord, r32 screenZ = 0.0f)
{
    // TODO look at removing these casts if the vector structs are expanded.
    return toClipSpace((u32)screenCord.x, (u32)screenCord.y, screenZ);
}

void gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{// function came from "http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/"
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) *zNear;
    fW = fH * aspect;

    glFrustum((-fW), fW, (-fH), fH, zNear, zFar); // the original just calls this anyway
}

void CreateShaders()
{// TODO(dustin): Look at adding 3 filePaths for the 3 different shaders and have them compile from that file
        //"#version 450 core\n void main(){gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;}";
    char *vertexShaderSource = \
        "#version 450 core\n \
         layout (location = 0) in vec3 aPos \
         void main() { \
         gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0;}";

    char *fragmentShaderSource = \
       "#version 450 core\n void main() {}";

    GLuint shaderProgramID =  glCreateProgram();
    GLuint vertexShaderID =   glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource((GLuint)vertexShaderSource, 1, &vertexShaderSource, 0);
    glCompileShader(vertexShaderID);

    glShaderSource((GLuint)fragmentShaderID, 1, &fragmentShaderSource, 0);
    glCompileShader(fragmentShaderID);

    int success;
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        MessageBox(0, "Vertex Shader Compilation Failure", "Shader Compiler ERROR", MB_ICONERROR|MB_OK);}

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if(!success) {
        MessageBox(0,"Fragment Shader Compilation Failure","Shader Compiler ERROR",MB_ICONERROR|MB_OK);}

    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);

    glLinkProgram(shaderProgramID);

    glGetShaderiv(shaderProgramID, GL_LINK_STATUS, &success);
    if(!success) {
        MessageBox(0,"Shader Failed to Link","Shader Link ERROR",MB_ICONERROR|MB_OK);}

    glValidateProgram(shaderProgramID); //TODO: look at disabling this at some point
    glUseProgram(shaderProgramID);

    // We do not need the objects once we do glUseProgram
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void InitOpenGL()
{
    glClearColor(0.3f,0.3f,0.3f,1.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    CreateShaders();

    glMatrixMode(GL_PROJECTION);
    glViewport(0,0,screenWidth,screenHeight);
    glLoadIdentity();
    gluPerspective(60.0f, (screenWidth/screenHeight), 1.0f, 30.0f);
    glMatrixMode(GL_MODELVIEW);
}

void ResizeWindow(int width, int height = 1)
{
    screenWidth = width;
    screenHeight = height;

    glMatrixMode(GL_PROJECTION);
    glViewport(0,0,screenWidth,screenHeight);
    glLoadIdentity();
    gluPerspective(60.0f, (screenWidth/screenHeight), 1.0f, 30.0f);
    glMatrixMode(GL_MODELVIEW);
}

void RenderRectViaTriangles(vector3 vA, vector3 vB, vector3 vC, vector3 vD, vector3 color)
{ //@NOTE: Where vertB and vertC are the Diagonal
    float Vertices[] = {vA.x, vA.y, vA.z,
                        vB.x, vB.y, vB.z,
                        vC.x, vC.y, vC.z,
                        vD.x, vD.y, vD.z };

    unsigned int indices[] = {0, 1, 3,
                              1, 2, 3};
    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float) , 0);

    glBindVertexArray(VAO);

    glColor4f(color.r, color.g, color.b, 1.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
}

void RenderRect(vector3 tRight, vector3 bRight, vector3 bLeft, vector3 tLeft, vector3 color=Vector3(0,0,0))
{ //@NTODO Fix this to use a quad once we get opengl working more frequently
    float Vertices[] = {tRight.x, tRight.y, tRight.z,
                        bRight.x, bRight.y, bRight.z,
                         bLeft.x,  bLeft.y,  bLeft.z,
                         tLeft.x,  tLeft.y,  tLeft.z };

    unsigned int indices[] = {0, 1, 3,
                              1, 2, 3};
    unsigned int VAO, VBO, EBO;

    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(VAO);

    glColor4f(color.r, color.g, color.b, 1.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
}

void RenderRect(vector2 tRight, vector2 bRight, vector2 bLeft, vector2 tLeft, vector3 color=Vector3(0,0,0))
{//@NOTE Do we need this or should we rename this to a 2d call and just draw it here?
    vector3 tright3 = {tRight.x,tRight.y,1.0f};
    vector3 bright3 = {bRight.x,bRight.y,1.0f};
    vector3 bleft3 = {bLeft.x,bLeft.y,1.0f};
    vector3 tleft3 = {tLeft.x,tLeft.y,1.0f};
    RenderRect(tright3,bright3, bleft3, tleft3, color);
}

void RenderTriangle(vector3 vA, vector3 vB, vector3 vC, vector3 color)
{
    float Vertices[] = {vA.x, vA.y, vA.z,
                        vB.x, vB.y, vB.z,
                        vC.x, vC.y, vC.z };

    unsigned int VAO, VBO;

    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);

    glColor4f(color.r, color.g, color.b, 1.0f); //@TODO Why does  this not supply the color!
    //glColor4f(1.0f,0.0f,0.0f, 1.0f); //@TODO Why does  this not supply the color!
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void testRenderCubeImmeditate()
{
    glBegin( GL_QUADS );
        //Top Face
        glColor3f(   0.0f,  1.0f,  0.0f); //Green
        glVertex3f(  1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f,  1.0f);
        glVertex3f(  1.0f,  1.0f,  1.0f);

        //Bottom Face
        glColor3f(   1.0f,  0.5f,  0.0f); //Orange
        glVertex3f(  1.0f, -1.0f, -1.0f);
        glVertex3f( -1.0f, -1.0f, -1.0f);
        glVertex3f( -1.0f, -1.0f,  1.0f);
        glVertex3f(  1.0f, -1.0f,  1.0f);

        //Front Face
        glColor3f(   1.0f,  0.0f,  0.0f); //Red
        glVertex3f(  1.0f,  1.0f,  1.0f);
        glVertex3f( -1.0f,  1.0f,  1.0f);
        glVertex3f( -1.0f, -1.0f,  1.0f);
        glVertex3f(  1.0f, -1.0f,  1.0f);

        //Back Face
        glColor3f(   1.0f,  1.0f,  0.0f); //Yellow
        glVertex3f(  1.0f, -1.0f, -1.0f);
        glVertex3f( -1.0f, -1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f(  1.0f,  1.0f, -1.0f);

        //Left Face
        glColor3f(   0.0f,  0.0f,  1.0f); //Blue
        glVertex3f( -1.0f,  1.0f,  1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f, -1.0f, -1.0f);
        glVertex3f( -1.0f, -1.0f,  1.0f);

        //Right Face
        glColor3f(   1.0f,  0.0f,  1.0f); //Violet
        glVertex3f(  1.0f,  1.0f,  1.0f);
        glVertex3f(  1.0f,  1.0f, -1.0f);
        glVertex3f(  1.0f, -1.0f, -1.0f);
        glVertex3f(  1.0f, -1.0f,  1.0f);
    glEnd();

}

bool isRendered = false;

void DEBUG_RenderFrame()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // WIREFRAME MODE
    if(!isRendered) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -2.0f);

        isRendered = true;
    }

    vector3 tRight = Vector3( 0.5f,  0.5f, 0.0f);
    vector3 bRight = Vector3( 0.5f, -0.5f, 0.0f);
    vector3 bLeft  = Vector3(-0.5f, -0.5f, 0.0f);
    vector3 tLeft  = Vector3(-0.5f,  0.5f, 0.0f);

    //RenderRectViaTriangles(tRight, bRight, bLeft, tLeft, Vector3(0.0f, 0.0f, 1.0f));
    RenderRect(tRight, bRight, bLeft, tLeft, Vector3(0.0f, 1.0f, 0.0f));

   // RenderTriangle( \
   //        Vector3(-0.5f, -0.5f, 0.0f), Vector3(0.5f, -0.5f, 0.0f), Vector3(0.0f, 0.5f, 0.0f),
   //        Vector3(1.0f, 1.0f, 0.0f) );
}
