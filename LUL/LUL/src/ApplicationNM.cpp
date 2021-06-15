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

unsigned int quadVAO = 0;
unsigned int quadVBO;

static float* getVertexData()
{
	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

	//int* a = new int[2]{ 11, 322 };
	//int balda[2] = { 22, 644 };

	float* quadVertices = new float[14 * 6] {
		// positions            // normal         // texcoords  // tangent                          // bitangent
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};

	return quadVertices;
}

int main()
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	float* vertexData = getVertexData();
	Texture diffuseTexture = Texture("res/textures/wood.png");
	Texture displacementMap = Texture("res/textures/toy_box_disp.png");
	Texture normalMap = Texture("res/textures/toy_box_normal.png");

	VertexBuffer vb = VertexBuffer(vertexData, sizeof(float) * 14 * 6);
	VertexArray vao = VertexArray();
	IndexBuffer ib = IndexBuffer(6);
	VertexBufferLayout vbl = VertexBufferLayout();
	vbl.Push<float>(3);//
	vbl.Push<float>(3);//
	vbl.Push<float>(2);//tex coord
	vbl.Push<float>(3);//tangent
	vbl.Push<float>(3);//bitangent
	vao.AddBuffer(vb, vbl);
	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);
	
	Shader parallaxMapping = Shader("res/shaders/vert/parallaxMapping.vert", 
		"res/shaders/frag/parallaxMapping.frag");
	Shader lightShader = Shader("res/shaders/Light.shader");

	glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
	glm::vec3 wallPos = glm::vec3(0.0f, 0.0f, -2.0f);
	float heightScale = 0.1f;

	Box box = Box();
	

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
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(camera.GetViewMatrix());
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		
		//model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), 
			//glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		
		model = glm::translate(model, wallPos);

		parallaxMapping.SetUniformMatrix4f("model", model);
		parallaxMapping.SetUniformMatrix4f("projection", projection);
		parallaxMapping.SetUniformMatrix4f("view", view);

		parallaxMapping.SetUniform3f("lightPos", lightPos);
		parallaxMapping.SetUniform3f("viewPos", camera.Position);

		diffuseTexture.Bind(0);
		displacementMap.Bind(1);
		normalMap.Bind(2);

		parallaxMapping.SetUniform1i("diffuseMap", 0);
		parallaxMapping.SetUniform1i("depthMap", 1);
		parallaxMapping.SetUniform1i("normalMap", 2);
		parallaxMapping.SetUniform1f("heightScale", heightScale);

		// nanosuit shader
		mainRenderer.Clear(true, true);

		// the plane
		mainRenderer.Draw(vao, ib, parallaxMapping);

		// variables for light shader
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		lightShader.SetUniformMatrix4f("u_Model", model);
		lightShader.SetUniformMatrix4f("u_View", view);
		lightShader.SetUniformMatrix4f("u_Projection", projection);

		mainRenderer.Draw(box.GetVAO(), box.GetIB(), lightShader);
		/*--------------*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	delete[] vertexData;

	return 0;
}

int mainNM()
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	float* vertexData = getVertexData();
	unsigned int indices[6] = { 0, 1, 2, 3, 4, 5 };
	VertexBuffer vb = VertexBuffer(vertexData, sizeof(float) * 14 * 6);
	VertexBufferLayout vbl = VertexBufferLayout();
	vbl.Push<float>(3);//
	vbl.Push<float>(3);//
	vbl.Push<float>(2);//tex coord
	vbl.Push<float>(3);//tangent
	vbl.Push<float>(3);//bitangent
	IndexBuffer ib = IndexBuffer(indices, 6);
	VertexArray va = VertexArray();
	va.AddBuffer(vb, vbl);
	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);
	
	// load textures
	Texture diffuseTexture = Texture("res/textures/brickwall.jpg");
	Texture normalMap = Texture("res/textures/brickwall_normal.jpg");
	
	// load shaders
	Shader theShader = Shader("res/shaders/vert/NormalMapping.vert", 
		"res/shaders/frag/NormalMapping.frag");
	Shader lightShader = Shader("res/shaders/Light.shader");

	// scene variables
	glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.3f);
	//
	Box box = Box();

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
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		
		theShader.SetUniformMatrix4f("u_Projection", projection);
		theShader.SetUniformMatrix4f("u_View", view);

		// render normal-mapped quad
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians((float)glfwGetTime() * -20.0f), 
			glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
		
		theShader.SetUniformMatrix4f("u_Model", model);
		theShader.SetUniform3f("u_LightPos", lightPos);
		theShader.SetUniform3f("u_ViewPos", camera.Position);

		theShader.SetUniform1i("u_DiffuseMap", 0);
		theShader.SetUniform1i("u_NormalMap", 1);
		diffuseTexture.Bind(0);
		normalMap.Bind(1);

		mainRenderer.Clear(true, true);
		mainRenderer.Draw(va, ib, theShader);

		// draw the light
		lightShader.SetUniformMatrix4f("u_View", view);
		lightShader.SetUniformMatrix4f("u_Projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		lightShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), lightShader); // light

		/*--------------*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	delete[] vertexData;

	return 0;
}