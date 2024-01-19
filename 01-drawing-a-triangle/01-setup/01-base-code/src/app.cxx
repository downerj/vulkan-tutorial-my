#include "app.hxx"

namespace app {
HelloTriangleApplication::HelloTriangleApplication() {
  initWindow();
  initVulkan();
}

auto HelloTriangleApplication::initWindow() -> void {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  const Size& size{initialWindowSize};
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

auto HelloTriangleApplication::initVulkan() -> void {}

auto HelloTriangleApplication::mainLoop() -> void {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

HelloTriangleApplication::~HelloTriangleApplication() {
  glfwDestroyWindow(window);
  glfwTerminate();
}
} // namespace app
