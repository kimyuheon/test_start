// VulkanRenderer.h
#pragma once
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <vector>
#include <windows.h>

struct Vertex {
    glm::vec3 pos;
};

enum class DrawState {
    Idle,
    DrawingRect,
    Extruding
};

class VulkanRenderer {
public:
    void Init(HWND hwnd);
    void Cleanup();
    void UpdateVertices();
    void UpdateUniform(const glm::mat4& mvp);
    void Render();

    // CAD-like mouse interaction
    void OnMouseDown(int x, int y);
    void OnMouseMove(int x, int y);
    void OnMouseUp();

private:
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;
    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline pipeline = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
    VkFramebuffer framebuffer = VK_NULL_HANDLE;
    VkExtent2D swapchainExtent{};

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexMemory = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexMemory = VK_NULL_HANDLE;
    VkBuffer uniformBuffer = VK_NULL_HANDLE;
    VkDeviceMemory uniformMemory = VK_NULL_HANDLE;

    DrawState drawState = DrawState::Idle;
    POINT rectStart = {0, 0};
    POINT rectEnd = {0, 0};
    float extrudeDepth = 0.0f;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    void CreateInstance();
    void CreateDevice(HWND hwnd);
    void CreateSwapchain();
    void CreateRenderPass();
    void CreatePipeline();
    void CreateBuffers();
    void CreateCommandBuffer();
    void RecordCommandBuffer();
    void CreateFramebuffer();
    void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                      VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void BuildCubeGeometry();
};
