#ifndef APP_HXX
#define APP_HXX

#include <cstdint>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace app {
struct Size {
  uint32_t width;
  uint32_t height;
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
  auto initWindow() -> void;
  auto initVulkan() -> void;
};
} // namespace app


#endif // APP_HXX
