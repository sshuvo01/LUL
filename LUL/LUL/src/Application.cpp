#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

static void ShowVector(const glm::vec3& vector)
{
	for (int i = 0; i < 3; i++)
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


int Notmain()
{
	/*
	std::string path = "res/model/nanosuit/nanosuit.obj";

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		
	}
	else
	{
		std::cout << "OKKK" << std::endl;
	}


	for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* mat = scene->mMaterials[i];

		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
		std::cout << str.C_Str() << std::endl;
		mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
		std::cout << str.C_Str() << std::endl;
	}
	

	std::vector<int> ti;

	ti.push_back(420);
	ti.push_back(421);
	*/
	PointLight pl = PointLight(glm::vec3(1.0f), 
		glm::vec3(2.0f), glm::vec3(3.0f), glm::vec3(4.0f),
		1.0f, 2.0f, 3.0f);

	ShowVector(pl.GetSpecular());
	ShowVector(pl.GetDiffuse());
	ShowVector(pl.GetAmbient());

	std::cout << pl.GetAttenuation().quadratic;
	//std::cout  << &ti[0] << std::endl;
	std::cin.get();
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
		// positions          // normals           // texture coords
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


	//GLCALL(glBindVertexArray(1));

	Renderer mainRenderer;
	Shader modelShader("res/shaders/modelLoading.shader");
	Model nanosuitModel("res/model/nanosuit/nanosuit.obj");
	glm::vec3 modelPosition = glm::vec3(0.0f, -1.75f, 0.0f);
	/////////////
	Shader boxShader("res/shaders/anewshader.shader");
	Shader lightShader("res/shaders/Light.shader");
	VertexArray boxVao;
	VertexBuffer boxVb = VertexBuffer(vertices, sizeof(float) * 8 * 36);
	IndexBuffer boxIb = IndexBuffer(indices, 36);
	VertexBufferLayout boxVbl;
	boxVbl.Push<float>(3);
	boxVbl.Push<float>(3);
	boxVbl.Push<float>(2);
	boxVao.AddBuffer(boxVb, boxVbl);

	Texture boxDiffuse = Texture("res/textures/container2.png");
	Texture boxSpecular = Texture("res/textures/container2_specular.png");

	
	boxShader.SetUniform3f("u_Light.ambient", glm::vec3(0.3f));
	boxShader.SetUniform3f("u_Light.diffuse", glm::vec3(0.7f));
	boxShader.SetUniform3f("u_Light.specular", glm::vec3(1.0f));

	boxShader.SetUniform1f("u_Material.shininess", 32.0f);
	/////////////////////////////////
	modelShader.SetUniform3f("u_Light.ambient", glm::vec3(0.3f));
	modelShader.SetUniform3f("u_Light.diffuse", glm::vec3(0.7f));
	modelShader.SetUniform3f("u_Light.specular", glm::vec3(1.0f));

	glm::vec3 lightPos = glm::vec3(5.0f, 3.0f, 0.0f);
	modelShader.SetUniform3f("u_Light.position", lightPos);

	boxShader.SetUniform3f("u_LightPosition", lightPos);

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
		mainRenderer.Clear();


		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		
		modelShader.SetUniformMatrix4f("u_Projection", projection);
		modelShader.SetUniformMatrix4f("u_View", view);

		//ourShader.setMat4("projection", projection);
		//ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, modelPosition); // translate it down so it's at the center of the scene
		model = glm::rotate(model, (float) glm::radians(currentFrame * 15.0), 
			glm::vec3(0.0f, 1.0f, 0.0f));		
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		modelShader.SetUniformMatrix4f("u_Model", model);

		modelShader.SetUniform3f("u_CameraPosition", camera.Position);
		/*render*/
		nanosuitModel.Draw(modelShader, mainRenderer);
		/*render the BOX*/
		boxVao.Bind();
		boxDiffuse.Bind(0);
		boxSpecular.Bind(1);

		boxShader.SetUniform1i("u_Material.diffuse", 0);
		boxShader.SetUniform1i("u_Material.specular", 1);

		float sign = 1.0f;
		float distance = 0.6f;

		for (int i = 0; i < 5; i++)
		{
			sign = sign * -1.0f; // alternate 1, -1

			model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)glm::radians(currentFrame * 120.0),
				glm::vec3(0.0f, sign * 1.0f, 0.0f));
			//model = glm::translate(model, glm::vec3(1.0f, 1.5f, 0.0f));
			//model = glm::translate(model, modelPosition); 
			//model = glm::rotate(model, (float)glm::radians(currentFrame * 15.0), glm::vec3(0.0f, 1.0f, 0.0f));

			model = glm::translate(model, glm::vec3(1.0f, -1.5f + (distance * i), 0.0f));
			model = glm::rotate(model, (float)glm::radians(currentFrame * 180.0),
				glm::vec3(sign*1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.3f));

			boxShader.SetUniformMatrix4f("u_Model", model);
			boxShader.SetUniformMatrix4f("u_View", camera.GetViewMatrix());
			boxShader.SetUniformMatrix4f("u_Projection", projection);
			boxShader.SetUniform3f("u_CameraPosition", camera.Position);

			mainRenderer.Draw(boxVao, boxIb, boxShader);
		}

		/*render the Light, all white*/
		lightShader.SetUniformMatrix4f("u_Projection", projection);
		lightShader.SetUniformMatrix4f("u_View", camera.GetViewMatrix());

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f));
		lightShader.SetUniformMatrix4f("u_Model", model);

		mainRenderer.Draw(boxVao, boxIb, lightShader);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}