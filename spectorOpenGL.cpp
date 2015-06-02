#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#define GLEW_STATIC

#include "MiddleMan/glew/glew.h"
#include "MiddleMan/glew/wglew.h"
#include <GL/gl.h>
#include "MiddleMan/glew/glext.h"
#include <Wingdi.h>

globalVar GLdouble pi = 3.1415926535897932384626433832795;
globalVar GLuint GLBitmapFontBasePtr = NULL;

void gluPerspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{// function came from "http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/"
    GLdouble fW, fH;
    fH = tan(fovY / 360 * pi) *zNear;
    fW = fH * aspect;

    glFrustum((-fW), fW, (-fH), fH, zNear, zFar); // the original just calls this anyway
}

void CreateShaders()
{// TODO(dustin): Look at adding 3 filePaths for the 3 different shaders and have them compile from that file

    char *vertexShaderSource =
        "#version 450 core\n void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;}";

    char *fragmentShaderSource =
        "#version 450 core\n void main() { gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);}";

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
    glUseProgram(shaderProgramID);
}

void InitOpenGL()
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

//TODO replace with something better as t his does not do what we want really
int glPrint(char *msg, vector2 screenLocation, vector3 textColor=Vector3(1.0f, 1.0f, 1.0f), ...)
{
    char text[256];
    va_list ap;

    if(msg == NULL) {return 0;}

    glLoadIdentity();
    //glTranslatef(0.0f, 0.0f, 0.0f); // move 1 into the screen
    glColor3f(textColor.r, textColor.g, textColor.b);
    glRasterPos2f(screenLocation.x, screenLocation.y); // (0,0f, 0.0f) is screen center +- 0.5 is max

    va_start(ap, msg);
        vsprintf_s(text, msg, ap);
    va_end(ap);

    glPushAttrib(GL_LIST_BIT);
    glListBase(GLBitmapFontBasePtr - 32);
    glCallLists(getStringLength(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();

    return 0;
}

void ResizeWindow(int width, int height)
{
    if(!height) {height = 1;}

    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (width/height), 0.2f, 255.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void DEBUG_RenderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    vector3 Vertices[3] = {Vector3(-1.0f, -1.0f, 0.0f), Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 1.0f, -1.0f)};

    GLuint VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glColor3f(0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
    glDisableVertexAttribArray(0);
}
