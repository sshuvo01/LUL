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
bool enableBlinn = true;
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
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	{
		enableBlinn = !enableBlinn;
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


int maintest()
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

	////////////////////////
	int aa = 7;
	const int * ptr;
	ptr = &aa;

	int value2 = 420;
	int *const klang = &value2;
	*klang = 212;

	std::cout << *klang << std::endl;


	std::string face[6] = {
		"top",
		"bottom",
		"right",
		"left",
		"back",
		"front"
	};

	const std::string* ptrstr = face;

	std::cout << ptrstr[0];

	std::cin.get();
	return 0;
}

void RenderScene(const Renderer& renderer, const Shader& shader,
	const Plane& plane, const Box& box)
{
	// set the model matrix in this function
	// draw the plane first
	glm::mat4 model = glm::mat4(1.0f);
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(plane.GetVAO(), plane.GetIB(), shader);
	// first box
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);
	// second box
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);
	// third box
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.SetUniformMatrix4f("u_Model", model);
	renderer.Draw(box.GetVAO(), box.GetIB(), shader);
}

int mainShadow(void)
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);
	DepthMap depthMap = DepthMap();

	Box box = Box();
	Plane plane = Plane();
	Shader test2 = Shader("res/shaders/vert/test2.vert", "res/shaders/frag/test2.frag");
	Shader depthMapShader = Shader("res/shaders/vert/shadowmap.vert",
		"res/shaders/frag/shadowmap.frag");
	Texture woodTexture = Texture("res/textures/wood.png");
	Shader shadowShader = Shader("res/shaders/vert/shadow.vert",
		"res/shaders/frag/shadow.frag");
	/*lightspace transform*/
	float nearPlane = 1.0f, farPlane = 7.5f;
	glm::mat4 lightProjection =
		glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

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
		mainRenderer.EnableDepthTest(true);
		/*------*/
		/*render the depth map*/
		depthMap.BindFramebuffer();
		depthMapShader.SetUniformMatrix4f("u_LightSpaceMatrix", lightSpaceMatrix);
		mainRenderer.Clear(false, true);
		mainRenderer.ChangeViewport(depthMap.GetWidht(), depthMap.GetHeight());
		RenderScene(mainRenderer, depthMapShader, plane, box);
		depthMap.UnbindFramebuffer();
		/*render the original scene*/
		mainRenderer.Clear(true, true);
		mainRenderer.ChangeViewport();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		//test2.SetUniformMatrix4f("u_View", view);
		//test2.SetUniformMatrix4f("u_Projection", projection);
		//woodTexture.Bind(0);
		//test2.SetUniform1i("u_Texture", 0);

		shadowShader.SetUniformMatrix4f("u_View", view);
		shadowShader.SetUniformMatrix4f("u_Projection", projection);
		shadowShader.SetUniformMatrix4f("u_LightSpaceMatrix", lightSpaceMatrix);
		shadowShader.SetUniform3f("u_LightPos", lightPos);
		shadowShader.SetUniform3f("u_CameraPos", camera.Position);

		woodTexture.Bind(0);
		shadowShader.SetUniform1i("u_DiffuseTexture", 0);
		depthMap.BindTexture(1);
		shadowShader.SetUniform1i("u_DepthMap", 1);

		RenderScene(mainRenderer, shadowShader, plane, box);
		/*--------------*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}


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

	float* quadVertices = new float[14 * 6]{
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

int main(void)
{
	
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	float planeVertices[] = {
		// positions            // normals         // texcoords
		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

		 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
		-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
		 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
	};

	unsigned int indices[6] = { 0, 1, 2, 3, 4, 5 };

	Plane aPlane;

	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);
	VertexArray vao = VertexArray();
	VertexBuffer vb = VertexBuffer(planeVertices, sizeof(float) * 8 * 6);
	VertexBufferLayout vbl = VertexBufferLayout();
	vbl.Push<float>(3);
	vbl.Push<float>(3);
	vbl.Push<float>(2);
	vao.AddBuffer(vb, vbl);

	IndexBuffer ib = IndexBuffer(indices, 6);
	Shader shd = Shader("res/shaders/vert/blinnphong.vert",
		"res/shaders/frag/blinnphong.frag");
	Shader shd2 = Shader("res/shaders/vert/Cloth.vert", 
		"res/shaders/frag/Cloth.frag");
	Texture planeTexture = Texture("res/textures/fabric_pattern_07_col_1_2k.png");
	Texture planeRoughness = Texture("res/textures/fabric_pattern_07_rough_2k.png");
	Texture planeNormal = Texture("res/textures/fabric_pattern_07_nor_2k.png");
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);

	// plane with tangent/bitangent
	float* vertexData = getVertexData();
	VertexBuffer vb2 = VertexBuffer(vertexData, sizeof(float) * 14 * 6);
	VertexArray vao2 = VertexArray();
	IndexBuffer ib2 = IndexBuffer(6);
	VertexBufferLayout vbl2 = VertexBufferLayout();
	vbl2.Push<float>(3);//
	vbl2.Push<float>(3);//
	vbl2.Push<float>(2);//tex coord
	vbl2.Push<float>(3);//tangent
	vbl2.Push<float>(3);//bitangent
	vao2.AddBuffer(vb2, vbl2);


	// lights
	// ------
	int numOfLights = 4;
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, 20.0f, 10.0f),
		glm::vec3(10.0f, 20.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};

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
		mainRenderer.EnableDepthTest(true);
		mainRenderer.Clear(true, true);
		/*------*/
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(40.0f), glm::vec3(1.0, 1.0, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, -5.0f, -10.0f));

		shd.SetUniformMatrix4f("u_Projection", projection);
		shd.SetUniformMatrix4f("u_View", view);
		shd.SetUniformMatrix4f("u_Model", model);
		shd.SetUniform1i("u_PlaneTex", 0);
		shd.SetUniform3f("u_LightPos", lightPos);
		shd.SetUniform3f("u_CameraPos", camera.Position);
		shd.SetUniform1i("u_Blinn", (int)enableBlinn);

		//
		/*
		uniform mat4 projection;
		uniform mat4 view;
		uniform mat4 model;

		// material parameters
		uniform vec3 albedo;
		uniform float alpha;

		// lights
		uniform vec3 lightPositions[4];
		uniform vec3 lightColors[4];

		uniform vec3 camPos;
		*/
		shd2.SetUniformMatrix4f("projection", projection);
		shd2.SetUniformMatrix4f("view", view);
		shd2.SetUniformMatrix4f("model", model);
		
		shd2.SetUniform3f("albedo", glm::vec3(0.5f, 0.0f, 0.5f));
		//shd2.SetUniform1f("ao", 1.0f);
		//shd2.SetUniform1f("metallic", 0.01f);
		//shd2.SetUniform1f("u_alpha", 0.81f);
		
		planeTexture.Bind(0);
		shd2.SetUniform1i("albedoMap", 0);
		planeRoughness.Bind(1);
		shd2.SetUniform1i("roughnessMap", 1);
		planeNormal.Bind(2);
		shd2.SetUniform1i("normalMap", 2);

		// lights setup
		for (int i = 0; i < numOfLights; i++)
		{
			std::string name = "lightPositions[" + std::to_string(i) + "]";
			shd2.SetUniform3f(name, lightPositions[i]);
			name = "lightColors[" + std::to_string(i) + "]";
			shd2.SetUniform3f(name, lightColors[i]);
		}

		//mainRenderer.Draw(vao, ib, shd);
		mainRenderer.Draw(vao2, ib2, shd2);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}

int maasina(void)
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);
	Model nanosuitModel = Model("res/model/nanosuit/nanosuit.obj");
	Shader nanosuitShader = Shader("res/shaders/vert/nanosuit.vert",
		"res/shaders/frag/nanosuit.frag", "res/shaders/geom/nanosuit.geom");

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
		mainRenderer.EnableDepthTest(true);
		mainRenderer.Clear(true, true);
		/*------*/
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		glm::translate(model, glm::vec3(0.0f, -5.0f, -10.0f));

		nanosuitShader.SetUniformMatrix4f("u_Projection", projection);
		nanosuitShader.SetUniformMatrix4f("u_View", view);
		nanosuitShader.SetUniformMatrix4f("u_Model", model);
		nanosuitShader.SetUniform1f("time", glfwGetTime());

		nanosuitModel.Draw(nanosuitShader, mainRenderer);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}

int mainInstancingModel(void)
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	float quadVertices[] = {
		// positions     // colors
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};

	unsigned int indices[6];
	for (int i = 0; i < 6; i++)
	{
		indices[i] = i;
	}

	VertexArray pointsVao = VertexArray();
	VertexBuffer pointsVb = VertexBuffer(quadVertices, 30 * sizeof(float));
	VertexBufferLayout pointsVbl = VertexBufferLayout();
	pointsVbl.Push<float>(2);
	pointsVbl.Push<float>(3);
	pointsVao.AddBuffer(pointsVb, pointsVbl);
	IndexBuffer pointsIndices = IndexBuffer(indices, 6);
	Shader instancingShader = Shader("res/shaders/vert/Instancing.vert", "res/shaders/frag/Instancing.frag");
	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);

	Shader planetShader = Shader("res/shaders/vert/planet.vert",
		"res/shaders/frag/planet.frag");


	Shader rockShader = Shader("res/shaders/vert/rock.vert", "res/shaders/frag/rock.frag");


	//Model nanosuitModel("res/model/nanosuit/nanosuit.obj");

	glm::vec2 translations[100];
	int index = 0;
	float offsets = 0.1f;

	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offsets;
			translation.y = (float)y / 10.0f + offsets;
			translations[index++] = translation;
		}
	}

	//pointsVao.Bind();
	VertexBuffer instanceBuffer = VertexBuffer(translations, sizeof(glm::vec2) * 100);
	pointsVao.AddInstanceBuffer(instanceBuffer, DataType::VEC2, 2, 1);

	//unsigned int instancedBuffer;
	//GLCALL(glCreateBuffers(1, &instancedBuffer));
	//GLCALL(glBindBuffer(GL_ARRAY_BUFFER, instancedBuffer));
	//GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, translations, GL_STATIC_DRAW));
	//GLCALL(glEnableVertexAttribArray(2));
	//GLCALL(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
	//GLCALL(glVertexAttribDivisor(2, 1));

	////////////////////////
	unsigned int amount = 10000;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 50.0;
	float offset = 6.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
	/////////////////////////
	VertexBuffer matrixBuffer = VertexBuffer(modelMatrices, amount * sizeof(glm::mat4));
	Model planetModel("res/model/planet/planet.obj");
	Model rockModel("res/model/rock/rock.obj");

	matrixBuffer.Bind();
	for (int i = 0; i < rockModel.m_Meshes.size(); i++)
	{
		rockModel.m_Meshes[i].m_VA.AddInstanceBuffer(matrixBuffer, DataType::MAT4, 3, 1);
		/*
		rockModel.m_Meshes[i].m_VA.Bind();
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
		*/
	}


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
		mainRenderer.EnableDepthTest(true);
		mainRenderer.Clear(true, true);
		/*
		for (int i = 0; i < 100; i++)
		{
			std::string uniformName = "u_Offsets[" + std::to_string(i) + "]";
			instancingShader.SetUniform2f(uniformName, translations[i]);
		}
		*/
		//instancingShader.Bind();
		//pointsVao.Bind();
		//pointsIndices.Bind();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
			(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		planetShader.SetUniformMatrix4f("u_Model", model);
		planetShader.SetUniformMatrix4f("u_View", view);
		planetShader.SetUniformMatrix4f("u_Projection", projection);

		planetModel.Draw(planetShader, mainRenderer);
		//rockModel.Draw(planetShader, mainRenderer);
		/*the rocks*/
		/*
		for (int i = 0; i < amount; i++)
		{
			planetShader.SetUniformMatrix4f("u_Model", modelMatrices[i]);
			rockModel.Draw(planetShader, mainRenderer);
		}
		*/


		//GLCALL(glDrawElementsInstanced(GL_TRIANGLES, pointsIndices.GetCount(), 
			//GL_UNSIGNED_INT, nullptr, 100));
		rockShader.Bind();
		rockShader.SetUniform1i("texture_diffuse1", 0);
		rockModel.m_Textures[0].Bind(0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id);

		rockShader.SetUniformMatrix4f("u_View", view);
		rockShader.SetUniformMatrix4f("u_Projection", projection);

		for (int i = 0; i < rockModel.m_Meshes.size(); i++)
		{
			//rockModel.m_Meshes[i].Draw(rockShader, mainRenderer);
			//glDrawElementsInstanced(GL_TRIANGLES, pointsIndices.GetCount(),
				//GL_UNSIGNED_INT, nullptr, 100);
			rockModel.m_Meshes[i].m_VA.Bind();
			rockModel.m_Meshes[i].m_IB.Bind();

			glDrawElementsInstanced(GL_TRIANGLES, rockModel.m_Meshes[i].m_IB.GetCount(),
				GL_UNSIGNED_INT, nullptr, amount);
		} // e/nd of for
		//mainRenderer.Draw(pointsVao, pointsIndices, instancingShader, PrimitiveShape::TRIANGLE);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}

int mainGeometryShader(void)
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");

	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};

	unsigned int indices[4];
	for (int i = 0; i < 4; i++)
	{
		indices[i] = i;
	}

	VertexArray pointsVao = VertexArray();
	VertexBuffer pointsVb = VertexBuffer(points, 20 * sizeof(float));
	VertexBufferLayout pointsVbl = VertexBufferLayout();
	pointsVbl.Push<float>(2);
	pointsVbl.Push<float>(3);
	pointsVao.AddBuffer(pointsVb, pointsVbl);
	IndexBuffer pointsIndices = IndexBuffer(indices, 4);
	Shader testShader = Shader("res/shaders/vert/test.vert", "res/shaders/frag/test.frag",
		"res/shaders/geom/test.geom");
	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);


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
		mainRenderer.EnableDepthTest(false);
		mainRenderer.Clear(true, false);
		mainRenderer.Draw(pointsVao, pointsIndices, testShader, PrimitiveShape::POINT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}


int main2(void)
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
		// positions          // normals    // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	/*skybox*/
	VertexArray skyVao = VertexArray();
	VertexBuffer skyVb = VertexBuffer(skyboxVertices, sizeof(float) * 36 * 3);
	VertexBufferLayout skyVbl = VertexBufferLayout();
	skyVbl.Push<float>(3);
	skyVao.AddBuffer(skyVb, skyVbl);
	IndexBuffer skyIb = IndexBuffer(indices, 36);

	Shader skyboxShader = Shader("res/shaders/vert/Skybox.vert", "res/shaders/frag/Skybox.frag");
	Shader kriss = Shader("res/shaders/vert/test.vert", "res/shaders/frag/test.frag",
		"res/shaders/geom/test.geom");

	std::string faces[6] = {
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	Cubemap skyboxCubemap = Cubemap("res/Cubemaps/skybox", faces);
	/*skybox*/


	std::vector<glm::vec3> windows;
	//windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	//windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
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
	VertexBuffer boxVb = VertexBuffer(cubeVertices, sizeof(float) * 8 * 36);
	IndexBuffer boxIb = IndexBuffer(indices, 36);

	VertexBufferLayout boxVbl;
	boxVbl.Push<float>(3);
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
	floorVao.AddBuffer(floorVb, floorVbl);

	/*same layout for grass*/
	grassVao.AddBuffer(grassVb, floorVbl);

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
		mainRenderer.Clear(true, true);

		//glEnable(GL_DEPTH_TEST);
		mainRenderer.EnableDepthTest();

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

		boxShader.SetUniform3f("u_CameraPos", camera.Position);
		boxShader.SetUniformMatrix4f("u_Model", model);
		boxShader.SetUniformMatrix4f("u_View", view);
		boxShader.SetUniformMatrix4f("u_Projection", projection);

		boxShader.SetUniform1i("u_BoxTexture", 0);
		boxTexture.Bind(0);
		boxShader.SetUniform1i("u_SkyboxTexture", 1);
		skyboxCubemap.Bind(1); // another texture;;;
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

		/*skybox*/

		mainRenderer.EnableDepthWriting(false);//disable depth writing
		skyboxShader.SetUniformMatrix4f("u_Projection", projection);

		glm::mat4 skyView = glm::mat4(glm::mat3(view));
		skyboxShader.SetUniformMatrix4f("u_View", skyView);
		skyboxShader.SetUniformMatrix4f("u_Model", model);

		skyboxShader.SetUniform1i("u_SkyboxTexture", 0);
		skyboxCubemap.Bind(0);

		mainRenderer.Draw(skyVao, skyIb, skyboxShader);

		mainRenderer.EnableDepthWriting(true); //restore depth writing

		/*end of skybox*/

		mainRenderer.UnbindFramebuffer();
		/*end of frame buffer*/
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