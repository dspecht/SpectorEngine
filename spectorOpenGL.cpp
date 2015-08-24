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
}

void
RenderRect(vector3 vertA, vector3 vertB, vector3 vertC, vector3 vertD, vector3 color)
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

    RenderRect(Vector3(1, 1, 0), Vector3(1, -1, 0),
            Vector3(-1, 1, 0), Vector3(-1, -1, -1), Vector3(1, 0, 0));
}
