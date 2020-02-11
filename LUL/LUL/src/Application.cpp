#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "SomeTransform.h"
#include "Camera.h"

const int SCR_WIDTH = 640*2;
const int SCR_HEIGHT = 480*2;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
///

static void ShowMatrix(const glm::mat4& matrix)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			std::cout << matrix[i][j] << ", ";
		}
		std::cout << "\n";
	}
}

static void ShowVector(const glm::vec4& vector)
{
	for (int i = 0; i < 4; i++)
	{
		std::cout << vector[i] << ", ";
	}
	std::cout << "\n";
}

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


int main(void)
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	float positions[] = {
		-0.5, -0.5,    1.0, 0.0, 0.0, 1.0,    0.0, 0.0,// 0
		-0.5,  0.5,    0.0, 1.0, 0.0, 1.0,    0.0, 1.0, // 1
		 0.5, -0.5,    0.0, 0.0, 1.0, 1.0,    1.0, 0.0,// 2
		 0.5,  0.5,    0.4, 0.12, 0.4, 1.0,    1.0, 1.0// 3
	};

	unsigned int indices[36] = {
		0, 1, 2,
		1, 3, 2
	};

	for (int i = 0; i < 36; i++)
	{
		indices[i] = i;
	}

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

	Shader lightShader("res/shaders/Light.shader");
	Shader objectShader("res/shaders/anewshader.shader");
	VertexBuffer vb(vertices, sizeof(float)* 36 * 6);
	
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	
	VertexArray vao;
	IndexBuffer ib = IndexBuffer(indices, 36);
	vao.AddBuffer(vb, vbl);
	Renderer mainRenderer = Renderer();

	vao.Bind();
	
	glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	lightShader.SetUniformMatrix4f("u_Projection", projection);

	objectShader.SetUniform3f("u_Light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	objectShader.SetUniform3f("u_Light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	objectShader.SetUniform3f("u_Light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	/*light source settings*/
	glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, -4.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);
	Transform::Translate(model, lightPosition);
	/*object settings*/
	glm::mat4 objectModel(1.0f);
	//Transform::Translate(objectModel, glm::vec3(10.0f, 10.0f, 10.0f));
	glm::vec3 objectColor = glm::vec3(0.6f, 0.01f, 0.01f);
	
	objectShader.SetUniformMatrix4f("u_Model", objectModel);
	objectShader.SetUniformMatrix4f("u_Projection", projection);
	objectShader.SetUniform3f("u_LightColor", lightColor);
	objectShader.SetUniform3f("u_LightPosition", lightPosition);
	objectShader.SetUniform3f("u_ObjectColor", objectColor);

	objectShader.SetUniform3f("u_Material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	objectShader.SetUniform3f("u_Material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	objectShader.SetUniform3f("u_Material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	objectShader.SetUniform1f("u_Material.shininess", 32.0f);

	/*initialize the renderer*/
	mainRenderer.InitSettings();
	/*render loop*/
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
		model = glm::mat4(1.0f); 
		Transform::Translate(model, lightPosition);
		
		lightShader.SetUniformMatrix4f("u_Model", model);
		lightShader.SetUniformMatrix4f("u_View", camera.GetViewMatrix());
		
		objectShader.SetUniformMatrix4f("u_View", camera.GetViewMatrix());
		
		//////////
		/*draw the light source*/
		mainRenderer.Clear();
		mainRenderer.Draw(vao, ib, lightShader);
		/*draw the object*/
		
		objectModel = glm::mat4(1.0f);
		Transform::Rotate(objectModel, currentFrame * 20.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		objectShader.SetUniformMatrix4f("u_Model", objectModel);
		objectShader.SetUniform3f("u_CameraPosition", camera.Position);
		mainRenderer.Draw(vao, ib, objectShader);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}