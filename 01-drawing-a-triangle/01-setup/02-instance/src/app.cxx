#include "app.hxx"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace std;

namespace app {
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
}

#ifdef DEBUG
auto checkInstancesAvailability(vector<const char*>& requiredExtensions) -> vector<const char*> {
  uint32_t extensionCountVK{};
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountVK, nullptr);
  vector<VkExtensionProperties> extensionsVK(extensionCountVK);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCountVK, extensionsVK.data());

  sort(requiredExtensions.begin(), requiredExtensions.end());
  cout << "Required extensions: " << endl;
  for (const auto& extension : requiredExtensions) {
    cout << "\t" << extension << endl;
  }
  cout << endl;

  sort(extensionsVK.begin(), extensionsVK.end(), [](const auto& lhs, const auto& rhs) {
    return strcmp(lhs.extensionName, rhs.extensionName) < 0;
  });
  cout << "Available extensions: " << endl;
  for (const auto& extension : extensionsVK) {
    cout << "\t" << extension.extensionName << endl;
  }
  cout << endl;

  vector<const char*> missingExtensions{};
  auto itReq{requiredExtensions.begin()};
  auto itVK{extensionsVK.begin()};
  while (itReq != requiredExtensions.end() and itVK != extensionsVK.end()) {
    if (*itReq > itVK->extensionName) {
      ++itVK;
    } else if (*itReq < itVK->extensionName) {
      missingExtensions.push_back(*itReq);
      ++itReq;
    } else {
      ++itReq;
      ++itVK;
    }
  }

  return missingExtensions;
}
#endif // DEBUG

auto HelloTriangleApplication::createInstance() -> void {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pNext = nullptr;
  appInfo.pApplicationName = "Hello Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pNext = nullptr;
  createInfo.flags = 0;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;
  uint32_t extensionCountGLFW{};
  const char** extensionsGLFW{glfwGetRequiredInstanceExtensions(&extensionCountGLFW)};
  vector<const char*> requiredExtensions{extensionsGLFW, extensionsGLFW + extensionCountGLFW};
#ifdef __APPLE__
  requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
  createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif // __APPLE__
  createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
  createInfo.ppEnabledExtensionNames = requiredExtensions.data();

#if DEBUG
  const vector<const char*> missingExtensions{checkInstancesAvailability(requiredExtensions)};
  if (missingExtensions.size() > 0) {
    cout << "Missing the following extensions:" << endl;
    copy(
      missingExtensions.begin(),
      missingExtensions.end(),
      ostream_iterator<const char *>(cout, " ")
    );
  }
#endif // DEBUG

  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error{"Failed to create the Vulkan instance"};
  }
}

auto HelloTriangleApplication::mainLoop() -> void {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

HelloTriangleApplication::~HelloTriangleApplication() {
  vkDestroyInstance(instance, nullptr);
  glfwDestroyWindow(window);
  glfwTerminate();
}
} // namespace app
