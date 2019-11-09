// Init Vulkan
// @author Spector aka. Dustin Specht | created 11-21-17

//#include "vulkan/vulkan.h"
//#include "vulkan/vk_sdk_platform.h"

VkInstance VKInstance;
VkResult Result;
VkDevice device_handle;
VkQueue graphicsQueue;
VkSurfaceKHR surface;
VkPhysicalDevice physicalDevice = NULL; // Final Phyiscal Device to be used
VkQueue presentQueue;

void CreateInstanceInfo() {
    VkApplicationInfo app_info = {};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = NULL;
    app_info.pApplicationName = NULL; //@Check use this of the windows dc field?
    app_info.applicationVersion = 1;
    app_info.pEngineName = "Spector Engine";
    app_info.engineVersion = 1;
    app_info.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instance_info = {};
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pNext = NULL;
    instance_info.flags = 0;
    instance_info.pApplicationInfo = &app_info;
    instance_info.enabledExtensionCount = 2;

    const char *extensionNames[] = {"VK_KHR_surface", "VK_KHR_win32_surface"}; //@Check do I need [1] here?
    instance_info.ppEnabledExtensionNames = extensionNames;
    //API layers
    u32 apiLayerCount = 0;
    vkEnumerateInstanceLayerProperties(&apiLayerCount, NULL);

    if (apiLayerCount == 0)
    { MessageBox(0, "Failed to find any layer for vulkan api", "VULKAN", MB_ICONERROR|MB_OK); }

    VkLayerProperties *apiLayersAvailable = new VkLayerProperties[apiLayerCount];
    vkEnumerateInstanceLayerProperties(&apiLayerCount, apiLayersAvailable);
    bool foundApiValidationLayer = false;

    for(u32 i = 0; i < apiLayerCount; i++) {
        if(strcmp(apiLayersAvailable[i].layerName, "VK_LAYER_LUNARG_standard_validation") == 0)
        { foundApiValidationLayer = true; }
    }

    if (foundApiValidationLayer == false)
    { MessageBox(0, "Failed to find validation of layers", "VULKAN", MB_ICONERROR|MB_OK); }

    const char *apiLayers[] = {"VK_LAYER_LUNARG_standard_validation"}; //TODO make this a das string
    //String vkApiLayers = {"VK_LAYER_LUNARG_standard_validation", 35};

    instance_info.enabledLayerCount = 1;
    instance_info.ppEnabledLayerNames = apiLayers;

    Result = vkCreateInstance(&instance_info, NULL, &VKInstance);

    if (!Result == VK_SUCCESS)
    { MessageBox(0, "Vulkan failed to create a instance", "VULKAN", MB_ICONERROR|MB_OK); }
}

//NOTE: do we really want to pass hwnd and instance here or get them some other way
void CreateWin32WindowSurface(HWND hwnd, HINSTANCE instance) {

    VkWin32SurfaceCreateInfoKHR createInfo;

    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = hwnd;
    createInfo.hinstance = instance;//GetModuleHandle(NULL);

    //auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR) vkGetInstanceProcAddr(VKInstance, "vkCreateWin32SurfaceKHR");
    PFN_vkCreateWin32SurfaceKHR CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR) vkGetInstanceProcAddr(VKInstance, "vkCreateWin32SurfaceKHR");

    if (!CreateWin32SurfaceKHR)
    { MessageBox(0, "Vulkan failed to load CreateWin32SurfaceKHR function pointer", "VULKAN", MB_ICONERROR|MB_OK); }

    Result = CreateWin32SurfaceKHR(VKInstance, &createInfo, NULL, &surface);
    if(Result != VK_SUCCESS)
    { MessageBox(0, "Vulkan failed to create Win32 Surface", "VULKAN", MB_ICONERROR|MB_OK); }
}

void CreatePhysicalDevice() {

    u32 gpuCount = 0;
    Result = vkEnumeratePhysicalDevices(VKInstance, &gpuCount, NULL);
        if (Result != VK_SUCCESS)
        { MessageBox(0, "Vulkan failed to Enumerate Physical Device(s) Count", "VULKAN", MB_ICONERROR|MB_OK); }
    VkPhysicalDevice pDevices[5] = {};
    Result = vkEnumeratePhysicalDevices(VKInstance, &gpuCount, pDevices);
        if (Result != VK_SUCCESS)
        { MessageBox(0, "Vulkan failed to Query Phyiscal Device(s)", "VULKAN", MB_ICONERROR|MB_OK); }

    if (Result == VK_ERROR_INCOMPATIBLE_DRIVER) {
        MessageBox(0, "Failed to find a compatible Vulkan ICD", "VULKAN", MB_ICONERROR|MB_OK);}
    else if (Result) {
        printf("unknown error\n");
        MessageBox(0, "Unknown Error", "VULKAN", MB_ICONERROR|MB_OK); }

    VkPhysicalDeviceProperties PDProperties = {};
    VkPhysicalDeviceFeatures PDFeatures = {};

    for (u8 i = 0; i < gpuCount; i++) {
        vkGetPhysicalDeviceProperties(pDevices[i], &PDProperties);
        vkGetPhysicalDeviceFeatures(pDevices[i], &PDFeatures);

        if(PDProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && PDFeatures.geometryShader) {
//            MessageBox(0, "I GOT TO  THE 1st if loop", "VULKAN", MB_ICONERROR|MB_OK);
            u32 queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &queueFamilyCount, nullptr);

            VkDeviceQueueCreateInfo queueInfo = {};
            queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueCount = 1;
            float queuePriority = 1.0f;
            queueInfo.pQueuePriorities = &queuePriority;

            //@Cleanup Check queueFamilyCount properly / more robustly
            assert(queueFamilyCount != 0);

            VkQueueFamilyProperties *queueFamilies =
                (VkQueueFamilyProperties*) calloc(queueFamilyCount, sizeof(VkQueueFamilyProperties));

            for (u32 j = 0; j < queueFamilyCount; j++) {
                vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &j, queueFamilies);

                VkBool32 supportsPresent;
                vkGetPhysicalDeviceSurfaceSupportKHR(pDevices[i], j, surface, &supportsPresent);

                if(queueFamilies[j].queueCount < 0 && supportsPresent &&
                        queueFamilies->queueFlags & VK_QUEUE_GRAPHICS_BIT) {

                    queueInfo.queueFamilyIndex = j;
                    physicalDevice = pDevices[i];

                    VkPhysicalDeviceFeatures deviceFeatures = {};

                    //NOTE: The Logical Device for vulkan is also made here
                    VkDeviceCreateInfo createInfo = {};
                    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                    createInfo.pQueueCreateInfos = &queueInfo;
                    createInfo.queueCreateInfoCount = 1;
                    createInfo.pEnabledFeatures = &deviceFeatures;
                    createInfo.enabledExtensionCount = 0;

                    Result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device_handle);
                    if (Result != VK_SUCCESS)
                    { MessageBox(0, "Vulkan failed to create Logical Device", "VULKAN", MB_ICONERROR|MB_OK); }
                }
            }
        }
    }
}

void init_vulkan(HWND hwnd, HINSTANCE instance) {
    CreateInstanceInfo();
    CreateWin32WindowSurface(hwnd, instance);
    CreatePhysicalDevice();
}

//TEMP DEBUG TEST
void screendrawtest()
{


}
