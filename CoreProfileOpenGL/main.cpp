#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <soil\SOIL.h>
#include <time.h>

#include "shader.hpp"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mpKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mpMouseCallback(GLFWwindow* window, double xpos, double ypos);
void mpScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mpHandleInput();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  gCamera(glm::vec3(0.0f, 0.0f, 3.0f));

// Deltatime
GLfloat gDeltaTime = 0.0f;	// Time between current frame and last frame
GLfloat gLastFrame = 0.0f;  	// Time of last frame

int main()
{
	srand(time(NULL));

	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* lWindow = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

	if (lWindow == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(lWindow);

	// Set the required callback functions
	glfwSetKeyCallback(lWindow, mpKeyCallback);
	glfwSetCursorPosCallback(lWindow, mpMouseCallback);
	glfwSetScrollCallback(lWindow, mpScrollCallback);

	// GLFW Options
	glfwSetInputMode(lWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Build and compile our shader program
	GLuint lLightingProgramID = LoadShaders("lighting.vs", "lighting.fs");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat lVerticesData[] = {
		// Positions          // Normals           // Texture Coords
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

	// First, set the container's VAO (and VBO)
	//Create VAO
	GLuint lVAO;
	glGenVertexArrays(1, &lVAO);
	glBindVertexArray(lVAO);

	//Create VBO
	GLuint lVBO;
	glGenBuffers(1, &lVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lVerticesData), lVerticesData, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	glUseProgram(lLightingProgramID);
	// Set material properties
	glUniform3f(glGetUniformLocation(lLightingProgramID, "material.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(lLightingProgramID, "material.specular"), 0.3f, 0.3f, 0.3f);
	glUniform1f(glGetUniformLocation(lLightingProgramID, "material.shininess"), 32.0f);

	glm::vec3 lSpotlightColor;
	lSpotlightColor.x = (float)rand() / (float)RAND_MAX + 0.5f;
	lSpotlightColor.y = (float)rand() / (float)RAND_MAX + 0.5f;
	lSpotlightColor.z = (float)rand() / (float)RAND_MAX + 0.5f;

	float lRotationAngle = 0.0f;
	GLfloat lCurrentFrame = 0.0f;

	// Game loop
	while (!glfwWindowShouldClose(lWindow))
	{
		// Calculate deltatime of current frame
		lCurrentFrame = glfwGetTime();
		gDeltaTime = lCurrentFrame - gLastFrame;
		gLastFrame = lCurrentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		mpHandleInput();

		// Clear the colorbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use cooresponding shader when setting uniforms/drawing objects
		glUseProgram(lLightingProgramID);

		GLint lViewPosLoc = glGetUniformLocation(lLightingProgramID, "viewPos");
		glUniform3f(lViewPosLoc, gCamera.Position.x, gCamera.Position.y, gCamera.Position.z);


		//Get PROPERTIES for SPOTLIGHT A
		GLint lSpotlightPosLoc = glGetUniformLocation(lLightingProgramID, "lightA.position");
		GLint lSpotlightDirLoc = glGetUniformLocation(lLightingProgramID, "lightA.direction");

		glm::vec3 lSpotlightPositionA(2.0, 2.0, 2.0);
		glUniform3f(lSpotlightPosLoc, lSpotlightPositionA.x, lSpotlightPositionA.y, lSpotlightPositionA.z);
		glUniform3f(lSpotlightDirLoc, -lSpotlightPositionA.x, -lSpotlightPositionA.y, -lSpotlightPositionA.z);

		GLint lSpotlightCutOffLoc = glGetUniformLocation(lLightingProgramID, "lightA.cutOff");
		GLint lSpotlightOuterCutOffLoc = glGetUniformLocation(lLightingProgramID, "lightA.outerCutOff");
		glUniform1f(lSpotlightCutOffLoc, glm::cos(glm::radians(8.0f)));
		glUniform1f(lSpotlightOuterCutOffLoc, glm::cos(glm::radians(10.0f)));

		glm::vec3 lSpotlightDiffuse = lSpotlightColor   * glm::vec3(0.8f); // Decrease the influence
		glm::vec3 lSpotlightAmbient = lSpotlightDiffuse * glm::vec3(0.2f); // Low influence

		glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.ambient"), lSpotlightAmbient.x, lSpotlightAmbient.y, lSpotlightAmbient.z);
		glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.diffuse"), lSpotlightDiffuse.x, lSpotlightDiffuse.y, lSpotlightDiffuse.z);
		glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.linear"), 0.09);
		glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.quadratic"), 0.032);

		//Get PROPERTIES for SPOTLIGHT B

		lSpotlightPosLoc = glGetUniformLocation(lLightingProgramID, "lightB.position");
		lSpotlightDirLoc = glGetUniformLocation(lLightingProgramID, "lightB.direction");

		glm::vec3 lSpotlightPositionB(-2.0, 2.0, 2.0);
		glUniform3f(lSpotlightPosLoc, lSpotlightPositionB.x, lSpotlightPositionB.y, lSpotlightPositionB.z);
		glUniform3f(lSpotlightDirLoc, -lSpotlightPositionB.x, -lSpotlightPositionB.y, -lSpotlightPositionB.z);

		lSpotlightCutOffLoc = glGetUniformLocation(lLightingProgramID, "lightB.cutOff");
		lSpotlightOuterCutOffLoc = glGetUniformLocation(lLightingProgramID, "lightB.outerCutOff");
		glUniform1f(lSpotlightCutOffLoc, glm::cos(glm::radians(8.0f)));
		glUniform1f(lSpotlightOuterCutOffLoc, glm::cos(glm::radians(10.0f)));

		lSpotlightDiffuse = lSpotlightColor   * glm::vec3(0.5f); // Decrease the influence
		lSpotlightAmbient = lSpotlightDiffuse * glm::vec3(0.2f); // Low influence

		glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.ambient"), lSpotlightAmbient.x, lSpotlightAmbient.y, lSpotlightAmbient.z);
		glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.diffuse"), lSpotlightDiffuse.x, lSpotlightDiffuse.y, lSpotlightDiffuse.z);
		glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.specular"), 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.linear"), 0.09);
		glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.quadratic"), 0.032);

		// Create camera transformations
		glm::mat4 lViewMatrix = gCamera.GetViewMatrix();
		glm::mat4 lProjectionMatrix = glm::perspective(gCamera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		//Create model transformations
		lRotationAngle += 40.0 * gDeltaTime;

		glm::mat4 lModelMatrix;
		lModelMatrix = glm::rotate(lModelMatrix, glm::radians(lRotationAngle), glm::vec3(1, 1, 1));

		// Get the uniform locations
		GLint lModelMatrixLoc = glGetUniformLocation(lLightingProgramID, "model");
		GLint lViewMatrixLoc = glGetUniformLocation(lLightingProgramID, "view");
		GLint lProjectionMatrixLoc = glGetUniformLocation(lLightingProgramID, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(lViewMatrixLoc, 1, GL_FALSE, glm::value_ptr(lViewMatrix));
		glUniformMatrix4fv(lProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(lProjectionMatrix));
		glUniformMatrix4fv(lModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(lModelMatrix));

		glBindVertexArray(lVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(lWindow);
	}

	// Clear any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

bool gKeysPressed[1024];
void mpKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			gKeysPressed[key] = true;
		else if (action == GLFW_RELEASE)
			gKeysPressed[key] = false;
	}
}

void mpHandleInput()
{
	if (gKeysPressed[GLFW_KEY_W])
		gCamera.ProcessKeyboard(FORWARD, gDeltaTime);
	if (gKeysPressed[GLFW_KEY_S])
		gCamera.ProcessKeyboard(BACKWARD, gDeltaTime);
	if (gKeysPressed[GLFW_KEY_A])
		gCamera.ProcessKeyboard(LEFT, gDeltaTime);
	if (gKeysPressed[GLFW_KEY_D])
		gCamera.ProcessKeyboard(RIGHT, gDeltaTime);
}

bool gFirstMouseMovement = true;
GLfloat gMouseDeltaX;
GLfloat gMouseDeltaY;
GLfloat gLastMouseX = WIDTH / 2.0;
GLfloat gLastMouseY = HEIGHT / 2.0;

void mpMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (gFirstMouseMovement)
	{
		gLastMouseX = xpos;
		gLastMouseY = ypos;
		gFirstMouseMovement = false;
	}

	gMouseDeltaX = xpos - gLastMouseX;
	gMouseDeltaY = gLastMouseY - ypos;  

	gLastMouseX = xpos;
	gLastMouseY = ypos;

	gCamera.ProcessMouseMovement(gMouseDeltaX, gMouseDeltaY);
}

void mpScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	gCamera.ProcessMouseScroll(yoffset);
}