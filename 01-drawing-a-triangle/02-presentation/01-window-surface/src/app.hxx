#ifndef APP_HXX
#define APP_HXX

#include <cstdint>
#include <optional>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace app {
struct Size {
  uint32_t width;
  uint32_t height;
};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;
  auto isComplete() const -> bool {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

class HelloTriangleApplication {
public:
  HelloTriangleApplication();
  HelloTriangleApplication(const HelloTriangleApplication&) = delete;
  HelloTriangleApplication(HelloTriangleApplication&&) = default;
  auto operator=(const HelloTriangleApplication&) -> HelloTriangleApplication& = delete;
  ~HelloTriangleApplication();
  auto mainLoop() -> void;

private:
  const Size initialWindowSize{800u, 600u};
  const char* const windowTitle{"Vulkan Tutorial"};
  GLFWwindow* window;
  VkInstance instance;
  VkPhysicalDevice physicalDevice{nullptr};
  VkDevice device;
  VkQueue graphicsQueue;
  VkSurfaceKHR surface;
  VkQueue presentQueue;

  auto initWindow() -> void;
  auto initVulkan() -> void;
  auto createInstance() -> void;
  auto pickPhysicalDevice() -> void;
  auto createLogicalDevice() -> void;
  auto createSurface() -> void;
  auto isDeviceSuitable(const VkPhysicalDevice device) -> bool;
  auto findQueueFamilies(const VkPhysicalDevice device) -> QueueFamilyIndices;
  auto getRequiredExtensions() -> std::vector<const char*>;
  auto checkInstancesAvailability(
    const std::vector<const char*>& requiredExtensions
  ) -> std::vector<const char*>;

#ifdef DEBUG
  const std::vector<const char*> validationLayers{getRequiredValidationLayers()};
  VkDebugUtilsMessengerEXT debugMessenger;

  auto populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& createInfo
  ) -> void;
  auto setupDebugMessenger() -> void;
  auto getRequiredValidationLayers() -> std::vector<const char*>;
  auto checkValidationLayersSupport(
    const std::vector<const char*>& requiredLayers
  ) -> std::vector<const char*>;
  static VKAPI_ATTR auto VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
  ) -> VkBool32;
  auto CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
  ) -> VkResult;
  auto DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator
  ) -> void;
#endif // DEBUG
};
} // namespace app


#endif // APP_HXX
