// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

//Math Stuff
#include <math.h>
#include <glm.hpp>

//Local Class Includes
#include "square.hpp"
#include "shader.hpp"
#include "xrprogram.hpp"

// Timing Includes
#include <chrono>
#include <thread>



#define PI 3.14159265

class Program
{
private:
	glm::mat4 view_matrix;

	glm::vec3 position;

	glm::mat4 projection_matrix;

	glm::mat4 vp_matrix;

	glm::vec3 right;

	glm::vec3 direction;

	glm::vec3 up;

	Square* sqr;

	int width = 1024;

	int height = 768;

	double interpolation = 0;

	float speed = 0.3;

	float mouseSpeed = 0.005f;

	float horiztonal_angle = 0;

	float verticle_angle = 0;

	int fps = 60;

	GLFWwindow* window;

	XrProgram* xr_program;

public:
	bool init() 
	{
		// Initialise GLFW
		if (!glfwInit())
		{
			fprintf(stderr, "Failed to initialize GLFW\n");
			getchar();
			return false;
		}

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Open a window and create its OpenGL context
		this->window = glfwCreateWindow(width, height, "OpenXR Sample", NULL, NULL);
		if (this->window == NULL) {
			fprintf(stderr, "Failed to open GLFW window.\n");
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(this->window);

		// Initialize GLEW
		glewExperimental = true; // Needed for core profile
		if (glewInit() != GLEW_OK) {
			fprintf(stderr, "Failed to initialize GLEW\n");
			getchar();
			glfwTerminate();
			return false;
		}

		// Ensure we can capture the escape key being pressed below
		glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GL_TRUE);

		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_CULL_FACE);
		// Create and compile our GLSL program from the shaders
		

		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
		this->projection_matrix = Projection;

		glm::mat4 View = glm::lookAt(
			glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		this->view_matrix = View;

		this->position = glm::vec3(4, 3, 3);

		this->direction = glm::vec3(0, 0, 0);

		this->vp_matrix = this->projection_matrix * this->view_matrix;

		Shader* test_shader = new Shader("Shaders\\vert.vsh", "Shaders\\frag.fg", true);
		
		this->sqr = new Square;

		glfwSetCursorPos(this->window, width / 2, height / 2);

		//XrProgram program("OpenXR Sample", this->window);

		this->xr_program = new XrProgram("OpenXR Sample", this->window);
		
		this->xr_program->init();

		this->xr_program->square = this->sqr;

		//program.destroy();

		return 0;
	}

	int main_loop() 
	{
		using clock = std::chrono::steady_clock;
		
		auto next_frame = clock::now();

		do {
			
			//Calculate next frame time
			next_frame += std::chrono::milliseconds(1000/this->fps);

			checkKeys();
			checkMouse();

			drawThings();
			bool result = this->xr_program->XrMainFunction();
			if (!result) 
			{
				break;
			}
			
			//Wait for next frame
			std::this_thread::sleep_until(next_frame);
		} // Check if the ESC key was pressed or the window was closed
		while (glfwGetKey(this->window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(this->window) == 0);

		// Close OpenGL window and terminate GLFW
		glfwTerminate();
		this->xr_program->destroy();
		return true;
	}

	void drawThings()
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sqr->draw(this->vp_matrix);

		// Swap buffers
		glfwSwapBuffers(this->window);
		glfwPollEvents();
	}

	void checkKeys() 
	{
		glm::vec3 temp;
		if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(this->window, GLFW_KEY_DOWN) != GLFW_PRESS)
		{
			temp = this->direction * this->speed;
			this->position += temp;
			calculateViewProjection();
		}
		else if (glfwGetKey(this->window, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(this->window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			temp = this->direction * this->speed;
			this->position -= temp;
			calculateViewProjection();
		}
		if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(this->window, GLFW_KEY_RIGHT) != GLFW_PRESS)
		{
			temp = this->right * this->speed;
			this->position -= temp;
			calculateViewProjection();
		}
		else if (glfwGetKey(this->window, GLFW_KEY_LEFT) != GLFW_PRESS && glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			temp = this->right * this->speed;
			this->position += temp;
			calculateViewProjection();
		}
	}

	void checkMouse() 
	{
		
		if (glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
		{
			double xpos, ypos;
			glfwGetCursorPos(this->window, &xpos, &ypos);
			glfwSetCursorPos(this->window, width/2, height/2);

			this->horiztonal_angle += (float)this->mouseSpeed * (width / 2 - xpos);
			this->verticle_angle += (float)this->mouseSpeed * (height / 2 - ypos);

			this->direction = glm::vec3(cos(this->verticle_angle) * sin(this->horiztonal_angle), sin(this->verticle_angle), cos(this->verticle_angle) * cos(this->horiztonal_angle));
			
			this->right = glm::vec3(sin(this->horiztonal_angle - (PI/2)), 0, cos(this->horiztonal_angle - (PI / 2)));

			this->up = glm::cross(right, direction);

			calculateViewProjection();
		}
	}

	void calculateViewProjection() 
	{
		glm::vec3 final_pos;
		final_pos = position + direction;
		this->view_matrix = glm::lookAt(this->position, final_pos, glm::vec3(0, 1, 0));
		this->vp_matrix = this->projection_matrix * this->view_matrix;
		return;
	}
};

int main(void) 
{
	Program main_program;
	main_program.init();
	main_program.main_loop();
}