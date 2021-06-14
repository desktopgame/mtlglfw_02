
#define GLFW_INCLUDE_NONE
#define GLFW_EXPOSE_NATIVE_COCOA
#include "mtlpp.hpp"
#include <glfw3.h>
#include <glfw3native.h>
#include <iostream>

static void quit(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

int main(void) {
  std::cout << "main start" << std::endl;
  mtlpp::Device gpu = mtlpp::Device::CreateSystemDefaultDevice();
  mtlpp::CommandQueue queue = gpu.NewCommandQueue();
  mtlpp::MetalLayer swapchain = mtlpp::MetalLayer();
  swapchain.SetDevice(gpu);
  swapchain.SetOpaque(true);

  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window = glfwCreateWindow(640, 480, "GLFW Metal", NULL, NULL);
  ns::Handle windowHandle;
  windowHandle.ptr = static_cast<void *>(glfwGetCocoaWindow(window));
  mtlpp::Window nsWindow = mtlpp::Window(windowHandle);
  nsWindow.SetLayer(swapchain);
  nsWindow.SetWantsLayer(true);

  glfwSetKeyCallback(window, quit);
  mtlpp::ClearColor color = mtlpp::ClearColor(0, 0, 0, 1);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    color.Red = (color.Red > 1.0) ? 0 : color.Red + 0.01;
    mtlpp::MetalDrawable surface = swapchain.NextDrawable();

    mtlpp::RenderPassDescriptor pass;
    pass.GetColorAttachments()[0].SetClearColor(color);
    pass.GetColorAttachments()[0].SetLoadAction(mtlpp::LoadAction::Clear);
    pass.GetColorAttachments()[0].SetStoreAction(mtlpp::StoreAction::Store);
    pass.GetColorAttachments()[0].SetTexture(surface.GetTexture());

    mtlpp::CommandBuffer buffer = queue.CommandBuffer();
    mtlpp::RenderCommandEncoder encoder = buffer.RenderCommandEncoder(pass);
    encoder.EndEncoding();
    buffer.Present(surface);
    buffer.Commit();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  std::cout << "main end" << std::endl;

  return 0;
}
