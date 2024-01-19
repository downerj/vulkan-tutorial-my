#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

auto main(int, char**) -> int {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow* window{glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr)};
  uint32_t extensionCount{};
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::cout << extensionCount << " extensions supported" << std::endl;
  glm::mat4 matrix{};
  glm::vec4 vec{};
  glm::vec4 test{matrix * vec};
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
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
}
