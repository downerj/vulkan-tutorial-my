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
  auto isComplete() const -> bool {
    return graphicsFamily.has_value();
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

  auto initWindow() -> void;
  auto initVulkan() -> void;
  auto createInstance() -> void;
  auto pickPhysicalDevice() -> void;
  static auto isDeviceSuitable(const VkPhysicalDevice device) -> bool;
  static auto findQueueFamilies(const VkPhysicalDevice device) -> QueueFamilyIndices;
  static auto getRequiredExtensions() -> std::vector<const char*>;
  static auto checkInstancesAvailability(
    const std::vector<const char*>& requiredExtensions
  ) -> std::vector<const char*>;

#ifdef DEBUG
  VkDebugUtilsMessengerEXT debugMessenger;

  static auto populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& createInfo
  ) -> void;
  auto setupDebugMessenger() -> void;
  static auto getRequiredValidationLayers() -> std::vector<const char*>;
  static auto checkValidationLayersSupport(
    const std::vector<const char*>& requiredLayers
  ) -> std::vector<const char*>;
  static VKAPI_ATTR auto VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
  ) -> VkBool32;
  static auto CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger
  ) -> VkResult;
  static auto DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT debugMessenger,
    const VkAllocationCallbacks* pAllocator
  ) -> void;
#endif // DEBUG
};
} // namespace app


#endif // APP_HXX
