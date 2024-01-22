#include "app.hxx"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include "debug.hxx"

using namespace std;

namespace app {
#ifdef DEBUG
auto HelloTriangleApplication::populateDebugMessengerCreateInfo(
  VkDebugUtilsMessengerCreateInfoEXT& createInfo
) -> void {
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
    | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;
}

auto HelloTriangleApplication::setupDebugMessenger() -> void {
  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  populateDebugMessengerCreateInfo(createInfo);

  if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
    throw runtime_error{"Failed to setup the debug messenger!"};
  }
}

auto HelloTriangleApplication::getRequiredValidationLayers() -> vector<const char*> {
  vector<const char*> layers{
    "VK_LAYER_KHRONOS_validation",
  };
  sort(layers.begin(), layers.end());
  return layers;
}

auto HelloTriangleApplication::checkValidationLayersSupport(
  const vector<const char*>& requiredLayers
) -> vector<const char*> {
  uint32_t layerCount{};
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  DEBUG_LINE(debug::fg::cyan << "Required validation layers: ");
  for (const auto& layer : requiredLayers) {
    DEBUG_LINE("\t" << layer);
  }
  DEBUG_LINE(debug::reset);

  sort(availableLayers.begin(), availableLayers.end(), [](const auto& lhs, const auto& rhs) {
    return strcmp(lhs.layerName, rhs.layerName) < 0;
  });
  DEBUG_LINE(debug::fg::green << "Available validation extensions: ");
  for (const auto& layer : availableLayers) {
    DEBUG_LINE("\t" << layer.layerName);
  }
  DEBUG_LINE(debug::reset);

  vector<const char*> missingLayers{};
  auto itReq{requiredLayers.begin()};
  auto itAvail{availableLayers.begin()};
  while (itReq != requiredLayers.end() and itAvail != availableLayers.end()) {
    if (strcmp(*itReq, itAvail->layerName) > 0) {
      ++itAvail;
    } else if (strcmp(*itReq, itAvail->layerName) < 0) {
      missingLayers.push_back(*itReq);
      ++itReq;
    } else {
      ++itReq;
      ++itAvail;
    }
  }

  return missingLayers;
}

auto HelloTriangleApplication::debugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT, // messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* // pUserData
) -> VkBool32 {
  if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      DEBUG_LOG(debug::fg::yellow);
    } else {
      DEBUG_LOG(debug::fg::red);
    }
    DEBUG_LOG("Validation layer: " << pCallbackData->pMessage);
    DEBUG_LINE(debug::reset);
  }

  return VK_FALSE;
}

auto HelloTriangleApplication::CreateDebugUtilsMessengerEXT(
  VkInstance instance,
  const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
  const VkAllocationCallbacks* pAllocator,
  VkDebugUtilsMessengerEXT* pDebugMessenger
) -> VkResult {
  auto vkCreateDebugUtilsMessengerEXT =
    (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
      instance,
      "vkCreateDebugUtilsMessengerEXT"
    );
  if (vkCreateDebugUtilsMessengerEXT == nullptr) {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
  return vkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

auto HelloTriangleApplication::DestroyDebugUtilsMessengerEXT(
  VkInstance instance,
  VkDebugUtilsMessengerEXT debugMessenger,
  const VkAllocationCallbacks* pAllocator
) -> void {
  auto vkDestroyDebugUtilsMessengerEXT{
    reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(
      instance,
      "vkDestroyDebugUtilsMessengerEXT"
    ))
  };
  if (vkDestroyDebugUtilsMessengerEXT != nullptr) {
    vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, pAllocator);
  }
}
#endif // DEBUG

auto HelloTriangleApplication::getRequiredExtensions() -> vector<const char*> {
  uint32_t extensionCountGLFW{};
  const char** extensionsGLFW{glfwGetRequiredInstanceExtensions(&extensionCountGLFW)};
  vector<const char*> extensions{extensionsGLFW, extensionsGLFW + extensionCountGLFW};
#ifdef DEBUG
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // DEBUG

  sort(extensions.begin(), extensions.end());
#ifdef DEBUG
  DEBUG_LINE(debug::fg::cyan << "Required extensions: ");
  for (const auto& extension : extensions) {
    DEBUG_LINE("\t" << extension);
  }
  DEBUG_LINE(debug::reset);
#endif // DEBUG

  return extensions;
}

auto HelloTriangleApplication::checkInstancesAvailability(
  const vector<const char*>& requiredExtensions
) -> vector<const char*> {
  uint32_t extensionCountVK{};
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountVK, nullptr);
  vector<VkExtensionProperties> availableExtensions(extensionCountVK);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountVK, availableExtensions.data());

  sort(availableExtensions.begin(), availableExtensions.end(), [](const auto& lhs, const auto& rhs) {
    return strcmp(lhs.extensionName, rhs.extensionName) < 0;
  });
#ifdef DEBUG
  DEBUG_LINE(debug::fg::green << "Available extensions: ");
  for (const auto& extension : availableExtensions) {
    DEBUG_LINE("\t" << extension.extensionName);
  }
  DEBUG_LINE(debug::reset);
#endif // DEBUG

  vector<const char*> missingExtensions{};
  auto itReq{requiredExtensions.begin()};
  auto itAvail{availableExtensions.begin()};
  while (itReq != requiredExtensions.end() and itAvail != availableExtensions.end()) {
    if (strcmp(*itReq, itAvail->extensionName) > 0) {
      ++itAvail;
    } else if (strcmp(*itReq, itAvail->extensionName) > 0) {
      missingExtensions.push_back(*itReq);
      ++itReq;
    } else {
      ++itReq;
      ++itAvail;
    }
  }

  return missingExtensions;
}

HelloTriangleApplication::HelloTriangleApplication() {
  initWindow();
  initVulkan();
}

auto HelloTriangleApplication::initWindow() -> void {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  const app::Size& size{initialWindowSize};
  window = glfwCreateWindow(size.width, size.height, windowTitle, nullptr, nullptr);
  glfwSetKeyCallback(
    window,
    [](GLFWwindow* window, int key, int /* scancode */, int action, int mods) {
      const bool isCtrlQ{key == GLFW_KEY_Q and mods == GLFW_MOD_CONTROL};
      const bool isCtrlW{key == GLFW_KEY_W and mods == GLFW_MOD_CONTROL};
      const bool isAltF4{key == GLFW_KEY_F4 and mods == GLFW_MOD_ALT};
      const bool isPressed{action == GLFW_PRESS};
      if ((isCtrlQ or isCtrlW or isAltF4) and isPressed) {
        glfwSetWindowShouldClose(window, GL_TRUE);
      }
    }
  );
}

auto HelloTriangleApplication::initVulkan() -> void {
  createInstance();
#ifdef DEBUG
  setupDebugMessenger();
#endif // DEBUG
}

auto HelloTriangleApplication::createInstance() -> void {
#ifdef DEBUG
  // Check for validation layers, only for debug builds.
  const vector<const char*> validationLayers{getRequiredValidationLayers()};
  const vector<const char*> missingLayers{checkValidationLayersSupport(validationLayers)};
  if (missingLayers.size() > 0u) {
    DEBUG_LINE(debug::fg::red << "Missing the following extensions:");
    for (const auto& layer : missingLayers) {
      DEBUG_LINE("\t" << layer);
    }
    DEBUG_LINE(debug::reset);
    throw runtime_error{"Validation layers requested, but not available!"};
  }
#endif // DEBUG

  // Check for extension availability.
  const vector<const char*> requiredExtensions{getRequiredExtensions()};
  const vector<const char*> missingExtensions{checkInstancesAvailability(requiredExtensions)};
  if (missingExtensions.size() > 0u) {
#ifdef DEBUG
    DEBUG_LINE(debug::fg::red << "Missing the following extensions:");
    for (const auto& extension : missingExtensions) {
      DEBUG_LINE("\t" << extension);
    }
    DEBUG_LINE(debug::reset);
#endif // DEBUG
    throw runtime_error{"Extensions requested, but not available!"};
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

#ifdef DEBUG
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  populateDebugMessengerCreateInfo(debugCreateInfo);
#endif // DEBUG

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.flags = 0;
  createInfo.pApplicationInfo = &appInfo;
#ifdef DEBUG
  createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
  createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
  createInfo.ppEnabledLayerNames = validationLayers.data();
#else
  createInfo.pNext = nullptr;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
#endif // DEBUG
#ifdef __APPLE__
  requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif // __APPLE__
  createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw runtime_error{"Failed to create the Vulkan instance"};
  }
}

auto HelloTriangleApplication::mainLoop() -> void {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

HelloTriangleApplication::~HelloTriangleApplication() {
#ifdef DEBUG
  DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
#endif // DEBUG
  vkDestroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}
} // namespace app
