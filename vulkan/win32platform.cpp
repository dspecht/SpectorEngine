//TODO: ERROR CHECK VULKAN BETTER ONCE IT IS WORKING
#include <windows.h>
#include <cstdio>
#include "win32platform.h"

// are these two things needed?
#define VK_USE_PLATFORM_WIN32_KHR
#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#include "vulkan/vk_sdk_platform.h"
//#include "initvulkan.cpp"

u32 screenWidth = 1600;
u32 screenHeight = 900;

// Global Vars
globalVar HDC g_WindowDC;
globalVar bool g_Running = true;

struct win32_WindowDimension
{
    u32 width;
    u32 height;
};

void Assert(bool flag, String msg) {
    if(!flag) {
        OutputDebugStringA( "ASSERT: " );
        OutputDebugStringA( msg );
        OutputDebugStringA( "\n" );
        int *base = 0;
        *base = 1;
        messagebox(0, msg.string, "Vulkan", mb_iconerror|mb_ok);

    }
}

void CheckVulkanResult( VkResult &result, char *msg ) {
    Assert( result == VK_SUCCESS, msg );
}

win32_WindowDimension Win32GetWindowDimensions(HWND window) {
    win32_WindowDimension result = {};

    RECT clientRect;
    GetClientRect(window, &clientRect);
    result.width = (clientRect.right - clientRect.left);
    result.height = (clientRect.bottom - clientRect.top);

    return result;
}

void Win32CleanShutdown(HWND window) {
    g_Running = false;
    vkDestroySurfaceKHR(VKInstance, surface, NULL);
    vkDestroyInstance(VKInstance, NULL);
    vkDestroyDevice(device_handle, NULL);
    ReleaseDC(window, g_WindowDC);
    PostQuitMessage(0);
}

//TODO: Look at logging actual reasons for failures
LRESULT CALLBACK
Win32MainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch(message) {
        case WM_CREATE: {
            PIXELFORMATDESCRIPTOR pfd = {sizeof(PIXELFORMATDESCRIPTOR), 1,
                PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
                PFD_TYPE_RGBA, 32, 0,0,0,0,0,0,0,0,0,0,0,0,0,24,
                0, 0, 0, 0, 0, 0, 0};

            g_WindowDC = GetDC(window);
            if(!g_WindowDC) {
                MessageBox(0, "windows failed to retrieve a device context", "windows Device Context ERROR", MB_ICONERROR|MB_OK);
                Win32CleanShutdown(window);
            }

            int windowsPixelFormat = ChoosePixelFormat(g_WindowDC, &pfd);
            SetPixelFormat(g_WindowDC, windowsPixelFormat, &pfd);


            UpdateWindow(window);
            SwapBuffers(g_WindowDC);
        } break;
        //case WM_PAINT:
        //{
        //    win32_WindowDimension dimensions = Win32GetWindowDimensions(window);
        //    ResizeWindow(dimensions.width, dimensions.height);
        //    DEBUG_RenderFrame();
        //    DefWindowProc(window, message, wParam, lParam);
        //} break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP: {
            u32 VKCode = (u32)wParam;
            // Chech the 30th and the 31st bits to see if the key was down
            // and if it is still down
            u8 wasDown = (lParam & (1 << 30)) != 0;
            u8 isDown = (lParam & (1 << 31)) == 0;

            //gameInputHandling(&state,VKCode, wasDown, isDown, lParam);
            result = DefWindowProc(window,message,wParam,lParam);
        } break;
        case WM_DESTROY: {
            Win32CleanShutdown(window);
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
        case WM_QUIT: {
            Win32CleanShutdown(window);
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
        default: {
            result = DefWindowProc(window, message, wParam, lParam);
        } break;
    }
    return result;
}

void Win32ProcessPendingMessages(HWND window) {
    MSG message;
    while(PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int CALLBACK
WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode) {
    printf("Hello World");

    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "Spector_Engine Vulkan Preview Window";

    if(!RegisterClass(&WindowClass))
    {MessageBox(0, "Failed to Register WindowClass", "Window Creation Error", MB_ICONERROR|MB_OK);}

    HWND Window = CreateWindowEx(0, WindowClass.lpszClassName, "Spector Engine Vulkan Preview",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,CW_USEDEFAULT,
            screenWidth, screenHeight,  //Window Starting Size
            0, 0, Instance, 0);

    if(!(Window)) {
        MessageBox(0, "Window Creation Failed", "Window Creation Error", MB_ICONERROR|MB_OK);
        Win32CleanShutdown(Window);
    }

//    init_vulkan(Window, Instance);

    while(g_Running) {
        Win32ProcessPendingMessages(Window);
        //DEBUG_RenderFrame();
        //update(&state);
        SwapBuffers(g_WindowDC);
    }
}//https://i.imgur.com/BhcsFVQ.png
