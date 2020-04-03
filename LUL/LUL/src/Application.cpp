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


int notmain()
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
	/*
	PointLight pl = PointLight(glm::vec3(1.0f), 
		glm::vec3(2.0f), glm::vec3(3.0f), glm::vec3(4.0f),
		1.0f, 2.0f, 3.0f);

	
	std::map<float, std::string> ball;

	std::map<float, std::string>::iterator iBegin = ball.begin();
	std::map<float, std::string>::iterator iEnd = ball.end();
	

	ball[211.0f] = "abal";
	ball[23.0f] = "asdfafs";
	ball[3.0f] = "kick";
	ball[2313.1f] = "a23rsdaf";

	for (auto it = ball.begin(); it != ball.end(); it++)
	{
		 std::cout <<  ball[it->first] << std::endl;
	}

	std::cout << ball[2.0f] << std::endl;
	
	

	*/
	GLbitfield mask = 0;

	if ((mask| GL_DEPTH_BUFFER_BIT) == GL_COLOR_BUFFER_BIT)
	{
		std::cout << "bokachoda" << std::endl;
	}

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

	float planeVertices[] = {
		// positions          
							/* texture Coords 
							(note we set these higher than 1 
							(together with GL_REPEAT as texture wrapping mode). 
							this will cause the floor texture to repeat) */
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	
	unsigned int planeIndices[6] = {
		0, 1, 2, 3, 4, 5
	};

	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	float cubeVertices[] = {
		// Back face
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
   // Front face
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
   // Left face
   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
   // Right face
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
   // Bottom face
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
   // Top face
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
	};
	
	float quadVertices[] = { 
		// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	std::vector<glm::vec3> windows;
	windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
	
	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT, true);
	
	/*box data*/
	//Shader boxShader("res/shaders/Box.shader");
	Shader boxShader("res/shaders/vert/Box.vert", "res/shaders/frag/Box.frag");
	Shader outlineShader("res/shaders/Outliner.vert");
	
	Texture boxTexture("res/textures/marble.jpg");
	
	/*frame buffer*/
	Shader framebuffer("res/shaders/vert/Framebuffer.vert", "res/shaders/frag/Framebuffer.frag");
	VertexArray frameVao = VertexArray();
	VertexBuffer frameVb = VertexBuffer(quadVertices, sizeof(float) * 4 * 6);
	VertexBufferLayout frameVbl = VertexBufferLayout();
	frameVbl.Push<float>(2);
	frameVbl.Push<float>(2);
	frameVao.AddBuffer(frameVb, frameVbl);
	IndexBuffer frameIb = IndexBuffer(planeIndices, 6);
	/*-----------------*/
	/*grass data*/
	Shader grassShader("res/shaders/grass.vert");
	Texture grassTexture("res/textures/blending_transparent_window.png", GL_CLAMP_TO_EDGE);
	VertexArray grassVao = VertexArray();
	VertexBuffer grassVb = VertexBuffer(transparentVertices, sizeof(float) * 5 * 6);
	IndexBuffer grassIb = IndexBuffer(planeIndices, 6);

	VertexArray boxVao = VertexArray();
	VertexBuffer boxVb = VertexBuffer(cubeVertices, sizeof(float) * 5 * 36);
	IndexBuffer boxIb = IndexBuffer(indices, 36);

	VertexBufferLayout boxVbl;
	boxVbl.Push<float>(3);
	boxVbl.Push<float>(2);
	boxVao.AddBuffer(boxVb, boxVbl);

	/* floor data */
	Shader floorShader("res/shaders/Floor.shader");
	Texture floorTexture("res/textures/metal.png");
	
	VertexArray floorVao = VertexArray();
	VertexBuffer floorVb = VertexBuffer(planeVertices, sizeof(float) * 6 * 5);
	IndexBuffer floorIb = IndexBuffer(planeIndices, 6);

	VertexBufferLayout floorVbl;
	floorVbl.Push<float>(3);
	floorVbl.Push<float>(2);
	floorVao.AddBuffer(floorVb, boxVbl);
	
	/*same layout for grass*/ 
	grassVao.AddBuffer(grassVb, boxVbl);

	/*blending settings initialized*/
	mainRenderer.InitBlending();
	/*render loop*/
	std::map<float, glm::vec3> windowsDistance;

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
		

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		
		/*frame buffer*/
		mainRenderer.BindFramebuffer();
		//glEnable(GL_DEPTH_TEST);
		mainRenderer.EnableDepthTest();
		mainRenderer.Clear(true, true);
		/*render the floor*/
		floorShader.SetUniformMatrix4f("u_Model", glm::mat4(1.0f));
		floorShader.SetUniformMatrix4f("u_View", camera.GetViewMatrix());
		floorShader.SetUniformMatrix4f("u_Projection", projection);

		floorShader.SetUniform1i("u_FloorTexture", 0);
		floorTexture.Bind(0);
		mainRenderer.Draw(floorVao, floorIb, floorShader);

		/*render the boxes */
		//GLCALL(glEnable(GL_CULL_FACE));
		//glFrontFace(GL_CCW);
		//glCullFace(GL_BACK);

		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		
		boxShader.SetUniformMatrix4f("u_Model", model);
		boxShader.SetUniformMatrix4f("u_View", view);
		boxShader.SetUniformMatrix4f("u_Projection", projection);

		boxShader.SetUniform1i("u_BoxTexture", 0);
		boxTexture.Bind(0);
		mainRenderer.Draw(boxVao, boxIb, boxShader);

		// another box
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		boxShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(boxVao, boxIb, boxShader);
		
		//GLCALL(glDisable(GL_CULL_FACE));
		 
		/*draw windows, (actually windows)*/
		grassShader.SetUniformMatrix4f("u_Projection", projection);
		grassShader.SetUniformMatrix4f("u_View", view);
		grassTexture.Bind(0);
		grassShader.SetUniform1i("u_GrassTexture", 0);

		/*sort the windows-- distance*/
		windowsDistance.clear();
		for (int i = 0; i < windows.size(); i++)
		{
			float dist = glm::length(camera.Position - windows[i]);
			windowsDistance[dist] = windows[i];
		}

		for (auto iter = windowsDistance.rbegin(); iter != windowsDistance.rend(); iter++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, iter->second);

			grassShader.SetUniformMatrix4f("u_Model", model);
			mainRenderer.Draw(grassVao, grassIb, grassShader);

		} // end of for
		/*frame buffer ends*/
		mainRenderer.UnbindFramebuffer();
		//glDisable(GL_DEPTH_TEST);
		mainRenderer.EnableDepthTest(false);
		framebuffer.SetUniform1i("u_ScreenTexture", 0);
		mainRenderer.BindTexture(0);
		mainRenderer.Draw(frameVao, frameIb, framebuffer);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}