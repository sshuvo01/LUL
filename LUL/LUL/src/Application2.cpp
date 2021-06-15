#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

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
#include "DepthMap.h"
#include "DepthCubemap.h"

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

void RenderScene(const Renderer& renderer, const Shader& shader, const Box& box)
{
	// set the model matrices here***
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(5.0f));
	shader.SetUniformMatrix4f("u_Model", model);
	glDisable(GL_CULL_FACE);
	shader.SetUniform1i("u_ReverseNormal", 1);

	renderer.Draw(box.GetVAO(), box.GetIB(), shader);

	// next cube
	shader.SetUniform1i("u_ReverseNormal", 0);
	glEnable(GL_CULL_FACE);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);

	// next cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);

	// another cubeeeee
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);

	// yet anotherrrrrrrrrrr cubeee
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);

	// one last cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.75f));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);
}

int main()
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);
	const unsigned int SHADOW_WIDTH = 1024;
	const unsigned int SHADOW_HEIGHT = 1024;

	Shader mainShader = Shader("res/shaders/vert/pointShadow.vert", 
		"res/shaders/frag/pointShadow.frag");
	Shader cubemapShader = Shader("res/shaders/vert/pointShadowDepth.vert", 
		"res/shaders/frag/pointShadowDepth.frag", "res/shaders/geom/pointShadowDepth.geom");
	Shader lightShader = Shader("res/shaders/Light.shader");

	Texture woodTexture = Texture("res/textures/wood.png");
	Box box = Box();
	DepthCubemap theCubemap = DepthCubemap();

	

	/* light variables */
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

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
		lightPos.z = sin(glfwGetTime()*0.5f)*3.0f;
		
		////glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float nearPlane = 1.0f;
		float farPlane = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f),
			(float)theCubemap.GetWidth()/(float)theCubemap.GetHeight(), nearPlane, farPlane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, 
			lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		mainRenderer.ChangeViewport(theCubemap.GetWidth(), theCubemap.GetHeight());
		//glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		
		theCubemap.BindFramebuffer();
		mainRenderer.Clear(false, true);
		//glClear(GL_DEPTH_BUFFER_BIT);

		std::string naam = "";
		for (unsigned int i = 0; i < 6; i++)
		{
			//u_LightSpace[6]
			naam = "u_LightSpace[" + std::to_string(i) + "]";

			cubemapShader.SetUniformMatrix4f(naam.c_str(), shadowTransforms[i]);
		} // end of for
		cubemapShader.SetUniform1f("u_FarPlane", farPlane);
		cubemapShader.SetUniform3f("u_LightPos", lightPos);

		RenderScene(mainRenderer, cubemapShader, box);
		theCubemap.UnbindFramebuffer();
		// render the original scene
		mainRenderer.ChangeViewport();
		//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mainRenderer.Clear(true, true);


		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		// render the light, single color
		lightShader.SetUniformMatrix4f("u_Projection", projection);
		lightShader.SetUniformMatrix4f("u_View", view);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		lightShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), lightShader);
		///////////////////////////
		mainShader.SetUniformMatrix4f("u_Projection", projection);
		mainShader.SetUniformMatrix4f("u_View", view);
		
		mainShader.SetUniform1i("u_DiffuseTexture", 0);
		mainShader.SetUniform1i("u_DepthCubemap", 1);

		mainShader.SetUniform3f("u_LightPos", lightPos);
		mainShader.SetUniform3f("u_ViewPos", camera.Position);
		mainShader.SetUniform1i("u_EnableShadow", 1);
		mainShader.SetUniform1f("u_FarPlane", farPlane);

		woodTexture.Bind(0);
		theCubemap.BindCubemap(1);
		mainShader.Bind();
		RenderScene(mainRenderer, mainShader, box);
		/*--------------*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}


	return 0;
}