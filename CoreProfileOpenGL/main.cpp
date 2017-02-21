#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <soil\SOIL.h>
#include <time.h>

#include "shader.hpp"
#include "Camera.h"
#include "Spotlight.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void mpKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mpMouseCallback(GLFWwindow* window, double xpos, double ypos);
void mpScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mpHandleInput();
float mfGetRandomFloat();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera gCamera(glm::vec3(0.0f, 0.0f, 3.0f));

// Materials
Material gCubeMaterial(glm::vec3(1.0, 1.0, 1.0), 100.0f);
Material gFloorMaterial(glm::vec3(1.0, 1.0, 1.0), 10.0f);

// Spotlights
Spotlight gSpotlightA(glm::vec3(0.0, 3.0, 0.0), 
					  glm::vec3(0.0, 0.0, 0.0), 
					  glm::cos(glm::radians(20.0f)), 
					  glm::cos(glm::radians(25.0f)), 
					  1.0f, 0.09f, 0.032);

Spotlight gSpotlightB(glm::vec3(2.0, 3.0, 3.0), 
					  glm::vec3(2.0, 0.0, 0.0), 
					  glm::cos(glm::radians(8.0f)), 
					  glm::cos(glm::radians(10.0f)), 
					  1.0f, 0.09f, 0.032);

GLfloat gDeltaTime = 0.0f;	
GLfloat gLastFrame = 0.0f;  	

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

	GLuint lVAO;
	glGenVertexArrays(1, &lVAO);
	glBindVertexArray(lVAO);

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

	GLuint lFloorVAO;
	glGenVertexArrays(1, &lFloorVAO);
	glBindVertexArray(lFloorVAO);

	//Use the same VBO, the floor is also a 3D cube.
	glBindBuffer(GL_ARRAY_BUFFER, lVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glUseProgram(lLightingProgramID);

	// Set material properties

	GLint lMatDiffuseLoc = glGetUniformLocation(lLightingProgramID, "material.diffuse");
	GLint lMatSpecularLoc = glGetUniformLocation(lLightingProgramID, "material.specular");
	GLint lMatShininessLoc = glGetUniformLocation(lLightingProgramID, "material.shininess");

	//Get PROPERTIES for SPOTLIGHT A
	gSpotlightA.mpSetColor(mfGetRandomFloat(), mfGetRandomFloat(), mfGetRandomFloat());

	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.position"),  gSpotlightA.aPosition.x,  gSpotlightA.aPosition.y,  gSpotlightA.aPosition.z);
	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.direction"), gSpotlightA.aDirection.x, gSpotlightA.aDirection.y, gSpotlightA.aDirection.z);

	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.cutOff"),      gSpotlightA.aCutoff);
	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.outerCutOff"), gSpotlightA.aOuterCutoff);

	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.ambient"),  gSpotlightA.aAmbient.r,  gSpotlightA.aAmbient.g,  gSpotlightA.aAmbient.b);
	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.diffuse"),  gSpotlightA.aDiffuse.r,  gSpotlightA.aDiffuse.g,  gSpotlightA.aDiffuse.b);
	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightA.specular"), gSpotlightA.aSpecular.r, gSpotlightA.aSpecular.g, gSpotlightA.aSpecular.b);
	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.constant"), gSpotlightA.aConstant);

	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.linear"),    gSpotlightA.aLinear);
	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightA.quadratic"), gSpotlightA.aQuadratic);

	//Get PROPERTIES for SPOTLIGHT B
	gSpotlightB.mpSetColor(mfGetRandomFloat(), mfGetRandomFloat(), mfGetRandomFloat());

	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.position"),  gSpotlightB.aPosition.x,  gSpotlightB.aPosition.y,  gSpotlightB.aPosition.z);
	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.direction"), gSpotlightB.aDirection.x, gSpotlightB.aDirection.y, gSpotlightB.aDirection.z);

	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.cutOff"),      gSpotlightB.aCutoff);
	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.outerCutOff"), gSpotlightB.aOuterCutoff);

	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.ambient"),  gSpotlightB.aAmbient.r,  gSpotlightB.aAmbient.g,  gSpotlightB.aAmbient.b);
	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.diffuse"),  gSpotlightB.aDiffuse.r,  gSpotlightB.aDiffuse.g,  gSpotlightB.aDiffuse.b);
	glUniform3f(glGetUniformLocation(lLightingProgramID, "lightB.specular"), gSpotlightB.aSpecular.r, gSpotlightB.aSpecular.g, gSpotlightB.aSpecular.b);
	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.constant"), gSpotlightB.aConstant);

	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.linear"),    gSpotlightB.aLinear);
	glUniform1f(glGetUniformLocation(lLightingProgramID, "lightB.quadratic"), gSpotlightB.aQuadratic);

	GLint lViewPosLoc = glGetUniformLocation(lLightingProgramID, "viewPos");
	GLint lModelMatrixLoc = glGetUniformLocation(lLightingProgramID, "model");
	GLint lViewMatrixLoc = glGetUniformLocation(lLightingProgramID, "view");
	GLint lProjectionMatrixLoc = glGetUniformLocation(lLightingProgramID, "projection");

	glm::mat4 lViewMatrix = gCamera.GetViewMatrix();
	glm::mat4 lProjectionMatrix = glm::perspective(gCamera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	glm::mat4 lModelMatrix;

	GLfloat lCurrentFrame = 0.0f;

	while (!glfwWindowShouldClose(lWindow))
	{
		// Clear the colorbuffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate deltatime of current frame
		lCurrentFrame = glfwGetTime();
		gDeltaTime = lCurrentFrame - gLastFrame;
		gLastFrame = lCurrentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		mpHandleInput();

		// Use cooresponding shader when setting uniforms/drawing objects
		glUseProgram(lLightingProgramID);

		// Update camera transformations
		lViewMatrix  = gCamera.GetViewMatrix();
		lProjectionMatrix = glm::perspective(gCamera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Update view position uniform
		glUniform3f(lViewPosLoc, gCamera.Position.x, gCamera.Position.y, gCamera.Position.z);

		// Update matrices uniforms
		glUniformMatrix4fv(lViewMatrixLoc,		 1, GL_FALSE, glm::value_ptr(lViewMatrix));
		glUniformMatrix4fv(lProjectionMatrixLoc, 1, GL_FALSE, glm::value_ptr(lProjectionMatrix));

		// Update cube transformations
		lModelMatrix = glm::mat4();
		glUniformMatrix4fv(lModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(lModelMatrix));

		glUniform3f(lMatDiffuseLoc, gCubeMaterial.aDiffuse.r, gCubeMaterial.aDiffuse.g, gCubeMaterial.aDiffuse.b);
		glUniform3f(lMatSpecularLoc, gCubeMaterial.aSpecular.r, gCubeMaterial.aSpecular.g, gCubeMaterial.aSpecular.b);
		glUniform1f(lMatShininessLoc, gCubeMaterial.aShininess);

		//Draw cube
		glBindVertexArray(lVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glUniform3f(lMatDiffuseLoc, gFloorMaterial.aDiffuse.r, gFloorMaterial.aDiffuse.g, gFloorMaterial.aDiffuse.b);
		glUniform3f(lMatSpecularLoc, gFloorMaterial.aSpecular.r, gFloorMaterial.aSpecular.g, gFloorMaterial.aSpecular.b);
		glUniform1f(lMatShininessLoc, gFloorMaterial.aShininess);

		// Update floor transformations
		lModelMatrix = glm::mat4();
		lModelMatrix = glm::translate(lModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
		lModelMatrix = glm::scale(lModelMatrix, glm::vec3(5.0f, 0.01f, 5.0f));
		glUniformMatrix4fv(lModelMatrixLoc, 1, GL_FALSE, glm::value_ptr(lModelMatrix));

		// Draw floor
		glBindVertexArray(lFloorVAO);
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

float mfGetRandomFloat()
{
	return (float)rand() / (float)RAND_MAX;
}