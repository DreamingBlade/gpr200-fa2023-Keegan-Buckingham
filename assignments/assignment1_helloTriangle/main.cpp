#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

int main() {

	float vertices[9] = {
		//x   //y   //z
		-0.5, -0.5, 0.0, //Bottom left
		 0.5, -0.5, 0.0, //Bottom right
		 0.0,  0.5, 0.0, //Top center
	};

	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	//Tell vao to pull vertex data from vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	createVAO(vertices, 3);
	
	//Vertex shader source
	const char* vertexShaderSource = R"(
	#version 450
	layout(location = 0) in vec3 vPos;
	void main(){
		gl_Position = vec4(vPos, 1.0);
	}
	)";

	//Fragment shader source
	const char* fragmentShaderSource = R"(
	#version 450
	out vec4 FragColor;
	void main(){
		FragColor = vec4(1.0,0.0,0.0,1.0);
	}
	)";
	
	createShader(GL_VERTEX_SHADER, vertexShaderSource);

	createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);


	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(float* vertexData, int numVertices)
{
	unsigned int vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Define position attribute (3 floats)
	glVertexAttribPointer(0, numVertices, GL_FLOAT, GL_FALSE, sizeof(vertexData) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	return vao;
}

// Creates a new shader of a given type.
// Possible types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc
// Returns id of the shader object
unsigned int createShader(GLenum shaderType, const char* sourceCode)
{
	//Create a new vertex shader object
	unsigned int shader = glCreateShader(shaderType);
	//Supply the shader object with source code
	glShaderSource(shader, 1, &sourceCode, NULL);
	//Compile the shader object
	glCompileShader(shader);

	//glCompileShader(vertexShader)
	//...
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		//512 is an arbitrary length, but should be plenty of characters for our error message.
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("Failed to compile shader: %s", infoLog);
	}

	return shader;
}