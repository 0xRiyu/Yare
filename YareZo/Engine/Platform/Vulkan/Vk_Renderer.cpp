#include "Platform/Vulkan/Vk_Renderer.h"
#include "Platform/Vulkan/Vk_Devices.h"
#include "Core/Yzh.h"
#include "Core/Glfw.h"

namespace Yarezo {
	namespace Graphics {

		YzVkRenderer::YzVkRenderer()
			:m_ImageAvailableSemaphores(MAX_FRAMES_IN_FLIGHT),
			 m_RenderFinishedSemaphores(MAX_FRAMES_IN_FLIGHT) {
		}

		YzVkRenderer::~YzVkRenderer() {
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				m_ImageAvailableSemaphores[i].cleanUp();
				m_RenderFinishedSemaphores[i].cleanUp();
			}

			m_YzSwapchain.reset();
		}

		void YzVkRenderer::init() {
			m_YzInstance = YzVkInstance::getYzVkInstance();
			// Create a swapchain, a swapchain is responsible for maintaining the images
			// that will be presented to the user. 
			m_YzSwapchain = std::make_shared<YzVkSwapchain>();
			m_YzSwapchain->init();

			createSemaphores();
		}

		void YzVkRenderer::createSemaphores() {
			for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
				m_ImageAvailableSemaphores[i].init();
				m_RenderFinishedSemaphores[i].init();
			}
		}

		void YzVkRenderer::recreateSwapchain() {
			int width = 0;
			int height = 0;
			GLFWwindow* window = static_cast<GLFWwindow*>(Application::getAppInstance()->getWindow()->getNativeWindow());

			glfwGetFramebufferSize(window, &width, &height);

			if (width == 0 || height == 0) {
				YZ_INFO("Application was minimized.");
				while (width == 0 || height == 0) {
					glfwGetFramebufferSize(window, &width, &height);
					glfwWaitEvents();
				}
				YZ_INFO("Application is no longer minimized.");
			}
			YZ_INFO("The application window has been re-sized, the new dimensions [W,H]  are: " + std::to_string(width) + ", " + std::to_string(height));

			YzVkDevice::instance()->waitIdle();
			m_YzSwapchain.reset(new YzVkSwapchain);
			m_YzSwapchain->init();
		}

		bool YzVkRenderer::begin() {
			VkResult result = m_YzSwapchain->acquireNextImage(m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore());

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				recreateSwapchain();
				return false;
			}
			else if (result != VK_SUCCESS) {
				YZ_ERROR("Vulkan failed to aquire a swapchain image.");
			}
			return true;
		}

		bool YzVkRenderer::present(YzVkCommandBuffer* cmdBuffer) {

			cmdBuffer->submitGfxQueue(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
									  m_ImageAvailableSemaphores[m_CurrentFrame].getSemaphore(),
									  m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore(),
									  true);

			VkResult result = m_YzSwapchain->present(m_RenderFinishedSemaphores[m_CurrentFrame].getSemaphore());

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				recreateSwapchain();
				return false;
			}
			else if (result != VK_SUCCESS) {
				YZ_ERROR("Vulkan failed to present a swapchain image.");
			}

			m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			return true;
		}
	}
}
