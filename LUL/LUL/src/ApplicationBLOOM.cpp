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

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

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
float exposure = 1.0f;
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

static void rightMouseClick(GLFWwindow* window, int button, int action, int mods)
{
	/*if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		std::cout << "RIGHT CLICK" << std::endl;
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << "Cursor Position at (" << xpos << " : " << ypos << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		std::cout << "LEFT CLICK" << std::endl;
		glfwSetCursorPos(window, SCR_WIDTH, SCR_HEIGHT);
	}*/
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
	glfwSetMouseButtonCallback(window, rightMouseClick);
	// tell GLFW to capture our mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	return 0;
}



static void SetupImgui(GLFWwindow*& window)
{
	const char* glsl_version = "#version 130";
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}


int main()
{
	GLFWwindow* window;
	int code = CreateWindow(window, SCR_WIDTH, SCR_HEIGHT, "SSS");
	
	Renderer mainRenderer = Renderer(SCR_WIDTH, SCR_HEIGHT);

	Shader lightShader = Shader("res/shaders/Light.shader");
	Shader tunnelShader = Shader("res/shaders/vert/TunnelShader.vert", "res/shaders/frag/TunnelShader.frag");
	Shader blurShader = Shader("res/shaders/vert/blur.vert", "res/shaders/frag/blur.frag");
	Shader finalShader = Shader("res/shaders/vert/finalImage.vert", "res/shaders/frag/finalImage.frag");

	Texture woodTexture = Texture("res/textures/wood.png", true);
	Texture containerTexture = Texture("res/textures/container2.png", true);

	Framebuffer frame(SCR_WIDTH, SCR_HEIGHT, FramebufferSettings(), 2, true);
	Framebuffer pingpongFrame[] = { Framebuffer(SCR_WIDTH, SCR_HEIGHT), Framebuffer(SCR_WIDTH, SCR_HEIGHT) };

	Box box = Box();
	ScreenRect screen = ScreenRect();

	std::vector<glm::vec3> lightPositions;
	lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
	lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
	lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
	lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
	// colors
	std::vector<glm::vec3> lightColors;
	lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
	lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
	lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
	lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

	// dear imgui
	SetupImgui(window);
	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
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

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();

		mainRenderer.ChangeViewport(SCR_WIDTH, SCR_HEIGHT, 0, 0);
		mainRenderer.Clear(true, true);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());






		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));

		tunnelShader.SetUniformMatrix4f("u_Model", model);
		tunnelShader.SetUniformMatrix4f("u_View", view);
		tunnelShader.SetUniformMatrix4f("u_Projection", projection);
		tunnelShader.SetUniform1i("u_ReverseNormal", 0);

		for (int i = 0; i < lightPositions.size(); i++)
		{
			std::string  name = "u_Lights[" + std::to_string(i) + "]";
			tunnelShader.SetUniform3f(name + ".position", lightPositions[i]);
			tunnelShader.SetUniform3f(name + ".color", lightColors[i]);
		}

		tunnelShader.SetUniform1i("u_LightN", (int)lightPositions.size());
		//tunnelShader.SetUniform3f("u_ViewPos", camera.Position);
		//////////
		//mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);
		// create one large cube that acts as the floor
		tunnelShader.SetUniform1i("u_DiffuseTexture", 0);
		woodTexture.Bind(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
		tunnelShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);

		// then create multiple cubes as the scenery
		tunnelShader.SetUniform1i("u_DiffuseTexture", 0);
		containerTexture.Bind(0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		tunnelShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		tunnelShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		tunnelShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
		model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.25));
		tunnelShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
		model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		tunnelShader.SetUniformMatrix4f("u_Model", model);
		mainRenderer.Draw(box.GetVAO(), box.GetIB(), tunnelShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		tunnelShader.SetUniformMatrix4f("u_Model", model);
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
		// blur bright fragments with two pass gaussian blur
		blurShader.SetUniform1i("image", 0);
		bool horizontal = true, first_iteration = true;
		unsigned int amount = 10;
		mainRenderer.EnableDepthTest(false);
		for (unsigned int i = 0; i < amount; i++)
		{
			pingpongFrame[horizontal].BindFramebuffer();
			blurShader.SetUniform1i("horizontal", horizontal);
			first_iteration ? frame.BindTexture(0, 1) : pingpongFrame[!horizontal].BindTexture(0);
			mainRenderer.Draw(screen.GetVAO(), screen.GetIB(), blurShader);
			horizontal = !horizontal;
			if (first_iteration) first_iteration = false;
		} // end of for
		frame.UnbindFramebuffer();
		mainRenderer.EnableDepthTest();
		// end of framebuffer
		//uniform sampler2D scene;
		//uniform sampler2D bloomBlur;
		//uniform bool bloom;
		//uniform float exposure;
		mainRenderer.ChangeViewport();
		mainRenderer.Clear(true, true);
		finalShader.SetUniform1i("bloom", true);
		finalShader.SetUniform1f("exposure", exposure);
		finalShader.SetUniform1i("scene", 0);
		finalShader.SetUniform1i("bloomBlur", 1);
		frame.BindTexture(0, 0);
		pingpongFrame[!horizontal].BindTexture(1);
		mainRenderer.Draw(screen.GetVAO(), screen.GetIB(), finalShader);

		
		/*--------------*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
