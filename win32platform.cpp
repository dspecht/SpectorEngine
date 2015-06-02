#include <windows.h>
#include <stdio.h>
#include "cStringLib.h"
#include "spectorengine.cpp"
#include "win32platform.h"

globalVar HDC g_WindowDC;
globalVar HGLRC g_GLRC;
globalVar bool g_Running = true;

win32_WindowDimension Win32GetWindowDimensions(HWND window)
{
    win32_WindowDimension result = {};

    RECT clientRect;
    GetClientRect(window, &clientRect);
    result.width = (clientRect.right - clientRect.left);
    result.height = (clientRect.bottom - clientRect.top);

    return result;
}

void Win32CleanShutdown(HWND window)
{
    g_Running = false;
    if(GLBitmapFontBasePtr != NULL) {glDeleteLists(GLBitmapFontBasePtr, 96);}
    wglMakeCurrent(g_WindowDC, 0);
    wglDeleteContext(g_GLRC);
    ReleaseDC(window, g_WindowDC);
    PostQuitMessage(0);
}

//TODO: Look at logging actual reasons for failures
LRESULT CALLBACK
Win32MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;
    switch(message)
    {
        case WM_CREATE:
        {
            PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
                PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
                PFD_TYPE_RGBA, 32, 0,0,0,0,0,0,0,0,0,0,0,0,0,24,
                0, 0, 0, 0, 0, 0, 0};

            g_WindowDC = GetDC(window);
            if(!g_WindowDC)
            {
                MessageBox(0, "windows failed to retrieve a device context", "windows Device Context ERROR", MB_ICONERROR|MB_OK);
                Win32CleanShutdown(window);
            }

            int windowsPixelFormat = ChoosePixelFormat(g_WindowDC, &pfd);
            SetPixelFormat(g_WindowDC, windowsPixelFormat, &pfd);

            g_GLRC = wglCreateContext(g_WindowDC);
            if(!g_GLRC)
            {
                MessageBox(0, "OpenGL failed to create a render context", "OpenGL Render Context ERROR", MB_ICONERROR|MB_OK);
                Win32CleanShutdown(window);
            }

            if(!wglMakeCurrent(g_WindowDC, g_GLRC))
            {
                MessageBox(0, "Failed to make the OpenGL Context the Current Context", "OpenGL Render Context ERROR", MB_ICONERROR|MB_OK);
                Win32CleanShutdown(window);
            }

            //TODO(dustin): Check to make sure GLEW is actually working
            glewExperimental=GL_TRUE; // test for glewInit Failing
            GLenum err = glewInit();
            if(GLEW_OK != err) // Assuming this does not fail OPENGL should be good to go
            {
                char *errString = (char*)glewGetErrorString(err);
                MessageBox(0, errString, "OPENGL GLEW ERROR", MB_ICONERROR|MB_OK);
                Win32CleanShutdown(window);
            }

            UpdateWindow(window);
            InitOpenGL(); // just basic settings true init is glewinit()
            SwapBuffers(g_WindowDC);
        } break;
        case WM_DESTROY:
        {
            Win32CleanShutdown(window);
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
        case WM_QUIT:
        {
            Win32CleanShutdown(window);
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
        default:
        {
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }
    return result;
}

void Win32ProcessPendingMessages(HWND window)
{
    MSG message;
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int CALLBACK
WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "SpectorEnginePreviewWindow";

    if(!RegisterClass(&WindowClass))
    {
        MessageBox(0, "Failed to Register WindowClass", "Window Creation Error", MB_ICONERROR|MB_OK);
    }

    HWND Window = CreateWindowEx(0, WindowClass.lpszClassName, "Spector Engine Preview",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,CW_USEDEFAULT,
            1280, 720,  //Window Starting Size
            0, 0, Instance, 0);

    if(!(Window))
    {
        MessageBox(0, "Window Creation Failed", "Window Creation Error", MB_ICONERROR|MB_OK);
        Win32CleanShutdown(Window);
    }

    // Initilize Game Memory Blocks (Permanent|Transient)
    {//TODO(Dustin): Look at doing Win32 State and changing this to be more like HMH memory allocation
        LPVOID baseMemoryAddress = 0;

        Game_Memory GameMemory = {};
        GameMemory.permanentStorageSize = Megabytes(256);
        GameMemory.transientStorageSize = Gigabytes(1);

        uint64 totalMemoryBlockSize = GameMemory.permanentStorageSize + GameMemory.transientStorageSize;

        GameMemory.permanentStorage = VirtualAlloc(baseMemoryAddress, (size_t)totalMemoryBlockSize,
                                                    MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        GameMemory.transientStorage = ((uint8 *)GameMemory.permanentStorage + GameMemory.permanentStorageSize);
    }

    while(g_Running)
    {
        Win32ProcessPendingMessages(Window);
        DEBUG_RenderFrame();
        SwapBuffers(g_WindowDC);
    }
}