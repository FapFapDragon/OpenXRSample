#pragma once
#ifndef XRSESSION_HPP
#define XRSESSION_HPP

#define XR_USE_GRAPHICS_API_OPENGL
#define XR_USE_PLATFORM_WIN32

#include <windows.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <openxr/openxr.h>
#include <openxr/openxr_platform_defines.h>
#include <openxr/openxr_platform.h>
#include <openxr/xr_linear.h>

#include <string>
#include <fstream>
#include <vector>
#include "square.hpp"

class XrProgram
{
private:
	char application_name[XR_MAX_APPLICATION_NAME_SIZE];

	std::vector<char*> required_extensions{ (char*)XR_KHR_OPENGL_ENABLE_EXTENSION_NAME };
	
	std::vector<char*> optional_extensions{ (char*)XR_KHR_COMPOSITION_LAYER_DEPTH_EXTENSION_NAME };

	std::vector<char*> enabled_extensions;

	GLFWwindow* window = nullptr;

public:

	XrInstance instance;

	Square* square;

	XrSession session;

	XrGraphicsBindingOpenGLWin32KHR graphics_binding;

	XrSpace reference_space;

	XrSystemId system_id;

	XrViewConfigurationType view_type = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

	XrSessionState state = XR_SESSION_STATE_UNKNOWN;

	float near_z;

	float far_z;

	bool xr_shutdown = false;

	// An array of arrays of framebuffers, one per texture/XR image
	std::vector<std::vector<GLuint>> framebuffers;

	//The preffered swapchain format
	int64_t swapchain_format;

	//The preffered swapchain format
	int64_t depth_swapchain_format;

	//An array of arrays of images for each projection swapchain
	std::vector<std::vector<XrSwapchainImageOpenGLKHR>> images;

	//An array of arrays of images for depth info swapchain
	std::vector<std::vector<XrSwapchainImageOpenGLKHR>> depth_images;

	//an array of swapchains for the projection views, 1 swapchain per view
	std::vector<XrSwapchain> swapchains;

	//an array of swapchains for the projectionViews Depth attachment
	std::vector<XrSwapchain> depth_swapchains;

	//An array of projection views, used for rendering to each eye
	std::vector<XrCompositionLayerProjectionView>	projection_views;
	
	//An array of config views, one for each eye
	std::vector<XrViewConfigurationView> xr_config_views;

	struct {
		bool supported = false;
		std::vector<XrCompositionLayerDepthInfoKHR> depth_info;
	} depth;

	bool init();

	void destroy();

	bool createInstance();

	bool createSession();

	bool createReferenceSpace();
	
	bool checkExtensionSupport();

	bool createViews();

	bool checkViewConfigs(XrViewConfigurationType view_type);

	bool createSwapchains(int view_count);

	bool checkSwapchainImageSupport(int64_t format);

	//Populate the images array with the swapchain images;
	bool getSwapchainImages();

	bool genFrameBuffers();

	bool beginSession();

	bool checkXrResult(XrResult);

	bool checkEvents();

	bool XrMainFunction();

	bool renderFrame(int width, int height, XrMatrix4x4f perspective_matrix, XrMatrix4x4f view_matrix, GLuint framebuffer, GLuint depthbuffer, XrSwapchainImageOpenGLKHR image, XrTime predicted_time);

	XrProgram(const char* application_name, GLFWwindow* window);

};

#endif 