#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <memory>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "SomeTransform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "PointLight.h"
#include "Cubemap.h"
#include "Box.h"
#include "Plane.h"
#include "ScreenRect.h"
#include "DepthMap.h"
#include "DepthCubemap.h"

#include "Framebuffer.h"

#define DBG(x) std::cout << x << std::endl;
const int SCR_WIDTH = 640 * 2;
const int SCR_HEIGHT = 480 * 2;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
///
static void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{

	}
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

static int CreateWindow(GLFWwindow*& window, int width, int height, const std::string& title)
{
	//GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "nooo... error" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	return 0;
}

int main()
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);
	Shader lightShader = Shader("res/shaders/Light.shader");
	Shader tunnelShader = Shader("res/shaders/vert/TunnelShader.vert", "res/shaders/frag/TunnelShader.frag");
	Shader hdrShader = Shader("res/shaders/vert/HDR.vert", "res/shaders/frag/HDR.frag");
	
	Texture woodTexture = Texture("res/textures/wood.png", true);
	
	Framebuffer frame(SCR_WIDTH, SCR_HEIGHT, FramebufferSettings(), 2, true);

	Box box = Box();
	ScreenRect screen = ScreenRect();

	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
	lightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
	lightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
	lightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));

	// Light colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
	lightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
	lightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

	while (!glfwWindowShouldClose(window))
	{
		////////////////////////
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		////////////////////////
		glm::mat4 view = glm::mat4(camera.GetViewMatrix());
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
		lightShader.SetUniformMatrix4f("u_View", view);
		lightShader.SetUniformMatrix4f("u_Projection", projection);
		
		// bind framebuffer
		frame.BindFramebuffer();
		mainRenderer.ChangeViewport(SCR_WIDTH, SCR_HEIGHT, 0, 0);
		mainRenderer.Clear(true, true);

		// Draw tunnel
		/*uniform mat4 u_Model;
		uniform mat4 u_View;
		uniform mat4 u_Projection;
		uniform bool u_ReverseNormal;

		uniform Light u_Lights[20];
		uniform int u_LightN;
		uniform sampler2D u_DiffuseTexture;
		uniform vec3 u_ViewPos;
		*/
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));

		tunnelShader.SetUniformMatrix4f("u_Model", model);
		tunnelShader.SetUniformMatrix4f("u_View", view);
		tunnelShader.SetUniformMatrix4f("u_Projection", projection);
		tunnelShader.SetUniform1i("u_ReverseNormal", 1);

		for (int i = 0; i < lightPositions.size(); i++)
		{
			std::string  name = "u_Lights[" + std::to_string(i) + "]";
			tunnelShader.SetUniform3f(name + ".position", lightPositions[i]);
			tunnelShader.SetUniform3f(name + ".color", lightColors[i]);
		}

		tunnelShader.SetUniform1i("u_LightN", (int)lightPositions.size());
		tunnelShader.SetUniform1i("u_DiffuseTexture", 0);
		woodTexture.Bind(0);
		//tunnelShader.SetUniform3f("u_ViewPos", camera.Position);
		//////////
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);

		// Draw lights
		for (int i = 0; i < lightPositions.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.1f));
			lightShader.SetUniformMatrix4f("u_Model", model);
			lightShader.SetUniform3f("u_LightColor", lightColors[i]);

			mainRenderer.Draw(box.GetVAO(), box.GetIB(), lightShader);
		} // end of for

		frame.UnbindFramebuffer();
		// end of framebuffer
		mainRenderer.ChangeViewport();
		mainRenderer.Clear(true, true);
		
		frame.BindTexture(0, 0);
		hdrShader.SetUniform1i("u_Texture", 0);
		mainRenderer.Draw(screen.GetVAO(), screen.GetIB(), hdrShader);
		/*--------------*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}
