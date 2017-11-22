//TODO: ERROR CHECK VULKAN BETTER ONCE IT IS WORKING
#include <windows.h>
#include <cstdio>
#include "win32platform.h"

// are these two things needed?
//#define VK_USE_PLATFORM_WIN32_KHR
//#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#include "vulkan/vk_sdk_platform.h"

unsigned int screenWidth = 1600;
unsigned int screenHeight = 900;

// Global Vars

globalVar HDC g_WindowDC;

VkInstance vkInstance;
VkResult vkResult;

VkDevice device_handle;

globalVar bool g_Running = true;

struct win32_WindowDimension
{
    int width;
    int height;
};

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
    vkDestroyInstance(vkInstance, NULL);
    vkDestroyDevice(device_handle, NULL);
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
        case WM_KEYUP:
        {
            u32 VKCode = (u32)wParam;
            // Chech the 30th and the 31st bits to see if the key was down
            // and if it is still down
            u8 wasDown = (lParam & (1 << 30)) != 0;
            u8 isDown = (lParam & (1 << 31)) == 0;

            //gameInputHandling(&state,VKCode, wasDown, isDown, lParam);
            result = DefWindowProc(window,message,wParam,lParam);
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
    printf("Hello World");

    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = Instance;
    WindowClass.lpszClassName = "Spector_Engine Vulkan Preview Window";

    if(!RegisterClass(&WindowClass))
    {
        MessageBox(0, "Failed to Register WindowClass", "Window Creation Error", MB_ICONERROR|MB_OK);
    }

    {
        // VULKAN
        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pNext = NULL;
        app_info.pApplicationName = NULL; // use this of the windows dc field?
        app_info.applicationVersion = 1;
        app_info.pEngineName = "Spector";
        app_info.engineVersion = 1;
        app_info.apiVersion = VK_API_VERSION_1_0;

        // initialize the VkInstanceCreateInfo structure
        VkInstanceCreateInfo vkInstance_info = {};
        vkInstance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkInstance_info.pNext = NULL;
        vkInstance_info.flags = 0;
        vkInstance_info.pApplicationInfo = &app_info;
        vkInstance_info.enabledExtensionCount = 0;
        vkInstance_info.ppEnabledExtensionNames = NULL;

        u32 vkApiLayerCount = 0;
        vkEnumerateInstanceLayerProperties(&vkApiLayerCount, NULL);
        if (vkApiLayerCount == 0) {
            MessageBox(0, "Failed to find any layer for vulkan api", "VULKAN", MB_ICONERROR|MB_OK);
        }
        VkLayerProperties *vkApiLayersAvailable = new VkLayerProperties[vkApiLayerCount];
        vkEnumerateInstanceLayerProperties(&vkApiLayerCount, vkApiLayersAvailable);
        bool vkFoundApiValidationLayer = false;

        for(u32 i = 0; i < vkApiLayerCount; i++) {
            if(strcmp(vkApiLayersAvailable[i].layerName, "VK_LAYER_LUNARG_standard_validation") == 0)
            {vkFoundApiValidationLayer = true;}
        }

        if (vkFoundApiValidationLayer == false) {
            MessageBox(0, "Failed to find validation of layers", "VULKAN", MB_ICONERROR|MB_OK);
        }
        const char *vkApiLayers[] = {"VK_LAYER_LUNARG_standard_validation"}; //TODO make this a das string
        //String vkApiLayers = {"VK_LAYER_LUNARG_standard_validation", 35};

        vkInstance_info.enabledLayerCount = 1;
        vkInstance_info.ppEnabledLayerNames = vkApiLayers;

        vkResult = vkCreateInstance(&vkInstance_info, NULL, &vkInstance);

        if (!vkResult == VK_SUCCESS) {
            MessageBox(0, "Vulkan failed to create a instance", "VULKAN", MB_ICONERROR|MB_OK);
        }

        // GET PHYSICAL GPU ENMERATION DETAILS
        u32 vkGpuCount = 0;
        vkResult = vkEnumeratePhysicalDevices(vkInstance, &vkGpuCount, NULL);
            if (vkResult != VK_SUCCESS) {
                MessageBox(0, "Vulkan failed to Enumerate Physical Device(s) Count", "VULKAN", MB_ICONERROR|MB_OK);
            }
        VkPhysicalDevice vkPdevices[4] = {};
        vkResult = vkEnumeratePhysicalDevices(vkInstance, &vkGpuCount, vkPdevices);
            if (vkResult != VK_SUCCESS) {
                MessageBox(0, "Vulkan failed to Query Phyiscal Device(s)", "VULKAN", MB_ICONERROR|MB_OK);
            }
        if (vkResult == VK_ERROR_INCOMPATIBLE_DRIVER) {
            printf("cannot find a compatible Vulkan ICD\n");
            exit(-1);
        } else if (vkResult) {
            printf("unknown error\n");
            exit(-1);
        }

        // Find a Working Physical Device
        VkPhysicalDeviceProperties vkPDProperties = {};
        VkPhysicalDeviceFeatures vkPDFeatures = {};
        VkPhysicalDevice physicalDevice = NULL; // Final Phyiscal Device to be used

        for (u8 i = 0; i < vkGpuCount; i++) {
            vkGetPhysicalDeviceProperties(vkPdevices[i], &vkPDProperties);
            vkGetPhysicalDeviceFeatures(vkPdevices[i], &vkPDFeatures);

            if(vkPDProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && vkPDFeatures.geometryShader) {
                physicalDevice = vkPdevices[i];
            }
        }

        u32 vkQueueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &vkQueueFamilyCount, nullptr);

        VkDeviceQueueCreateInfo queueInfo = {};
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueCount = 1;

        float queuePriority = 1.0f; //NOTE: why does this need to be here ( Because it wants a address)
        queueInfo.pQueuePriorities = &queuePriority;

        if(vkQueueFamilyCount != 0)
        {
            VkQueueFamilyProperties *vkQueueFamilies = (VkQueueFamilyProperties*)calloc(vkQueueFamilyCount, sizeof(VkQueueFamilyProperties));

            for(u32 i = 0; i < vkQueueFamilyCount; i++)
            {
                vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &i, vkQueueFamilies);
                if(vkQueueFamilies[i].queueCount > 0 && vkQueueFamilies->queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    queueInfo.queueFamilyIndex = i;
                }
            }
        }

    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = 0;

    vkResult = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device_handle);
    if (vkResult != VK_SUCCESS) {
        MessageBox(0, "Vulkan failed to create Logical Device", "VULKAN", MB_ICONERROR|MB_OK);
    }


    }


    HWND Window = CreateWindowEx(0, WindowClass.lpszClassName, "Spector Engine Vulkan Preview",
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            CW_USEDEFAULT,CW_USEDEFAULT,
            screenWidth, screenHeight,  //Window Starting Size
            0, 0, Instance, 0);

    if(!(Window))
    {
        MessageBox(0, "Window Creation Failed", "Window Creation Error", MB_ICONERROR|MB_OK);
        Win32CleanShutdown(Window);
    }

    while(g_Running)
    {
        Win32ProcessPendingMessages(Window);
        //DEBUG_RenderFrame();
        //update(&state);
        SwapBuffers(g_WindowDC);
    }
}//https://i.imgur.com/BhcsFVQ.png
