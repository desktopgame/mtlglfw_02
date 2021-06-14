
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

  const char shaderSrc[] = R"""(
		#include <metal_stdlib>
		using namespace metal;

		// 頂点シェーダー.
		vertex float4 vertFunc(const device packed_float3* vertexArray [[buffer(0)]], unsigned int vID [[vertex_id]])
		{
			return float4(vertexArray[vID], 1.0);
		}

		// フラグメントシェーダー.
		fragment half4 fragFunc()
		{
			return half4(1.0, 0.0, 0.0, 1.0);
		}
	)""";

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

  mtlpp::Library library =
      gpu.NewLibrary(shaderSrc, mtlpp::CompileOptions(), nullptr);

  assert(library);

  mtlpp::Function vertFunc = library.NewFunction("vertFunc");
  assert(vertFunc.GetPtr());

  mtlpp::Function fragFunc = library.NewFunction("fragFunc");
  assert(fragFunc.GetPtr());

  // 頂点バッファの生成.
  const float vertexData[] = {
      0.0f, 1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
  };

  auto vertexBuffer =
      gpu.NewBuffer(vertexData, sizeof(vertexData),
                    mtlpp::ResourceOptions::CpuCacheModeDefaultCache);
  assert(vertexBuffer.GetPtr());

  // レンダーパイプラインの生成.
  mtlpp::RenderPipelineDescriptor desc;
  desc.SetVertexFunction(vertFunc);
  desc.SetFragmentFunction(fragFunc);

  auto attach = desc.GetColorAttachments();
  attach[0].SetPixelFormat(mtlpp::PixelFormat::BGRA8Unorm);

  auto renderPipelineState = gpu.NewRenderPipelineState(desc, nullptr);
  assert(renderPipelineState.GetPtr());

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    mtlpp::MetalDrawable surface = swapchain.NextDrawable();

    mtlpp::CommandBuffer commandBuffer = queue.CommandBuffer();
    assert(commandBuffer.GetPtr());

    mtlpp::RenderPassDescriptor renderPassDesc;
    renderPassDesc.GetColorAttachments()[0].SetClearColor(color);
    renderPassDesc.GetColorAttachments()[0].SetLoadAction(
        mtlpp::LoadAction::Clear);
    renderPassDesc.GetColorAttachments()[0].SetStoreAction(
        mtlpp::StoreAction::Store);
    renderPassDesc.GetColorAttachments()[0].SetTexture(surface.GetTexture());

    if (renderPassDesc) {
      mtlpp::RenderCommandEncoder encoder =
          commandBuffer.RenderCommandEncoder(renderPassDesc);
      assert(encoder.GetPtr());

      encoder.SetRenderPipelineState(renderPipelineState);
      encoder.SetVertexBuffer(vertexBuffer, 0, 0);
      encoder.Draw(mtlpp::PrimitiveType::Triangle, 0, 3);
      encoder.EndEncoding();

      commandBuffer.Present(surface);
    }

    commandBuffer.Commit();
    commandBuffer.WaitUntilCompleted();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  std::cout << "main end" << std::endl;

  return 0;
}
