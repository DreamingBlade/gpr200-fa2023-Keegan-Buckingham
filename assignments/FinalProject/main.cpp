#include <stdio.h>
#include <math.h>
#include <iostream>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <kmb/shader.h>
#include <kmb/texture.h>

struct Vertex {
	float x, y, z;
	float u, v;
};

struct Sprite {
	float x, y, width, height;
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;
const float SPRITE_WIDTH = 31;
const float SPRITE_HEIGHT = 31;
const float CHARACTER_SPRITES_X = 3;
const float CHARACTER_SPRITES_Y = 4;
const float NUM_CHARACTERS_ROW = 4;
const float NUM_ROWS = 2;
//size of the overall spritesheet
const float IMG_SPRITES_X = CHARACTER_SPRITES_X * NUM_CHARACTERS_ROW;
const float IMG_SPRITES_Y = CHARACTER_SPRITES_Y * NUM_ROWS;
const float IMG_WIDTH = IMG_SPRITES_X * SPRITE_WIDTH; // 381;
const float IMG_HEIGHT = IMG_SPRITES_Y * SPRITE_HEIGHT; // 253;

Vertex vertices[4] =
{
	{-1.0, -1.0, 0.0, 0.0, 0.0},
	{1.0, -1.0, 0.0, 1.0, 0.0},
	{1.0, 1.0, 0.0, 1.0, 1.0},
	{-1.0, 1.0, 0.0, 0.0, 1.0}
};

unsigned short indices[6] =
{
	0, 1, 2,
	2, 3, 0
};

int main()
{
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Textures", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glEnable(GL_BLEND);

	kmb::Shader backgroundShader("assets/background.vert", "assets/background.frag");
	kmb::Shader characterShader("assets/character.vert", "assets/character.frag");

	unsigned int backgroundQuadVAO = createVAO(vertices, 4, indices, 6);

	glBindVertexArray(backgroundQuadVAO);

	unsigned int grassTexture = loadTexture("assets/grass.jpg", GL_REPEAT, GL_LINEAR);

	unsigned int characterTexture = loadTexture("assets/element_cat_sprites.png", GL_CLAMP_TO_EDGE, GL_LINEAR);

	
	//Place grass in unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture);

	backgroundShader.setInt("_BrickTexture", 0);

	float pixelX = 1.0f / IMG_WIDTH;
	float halfPixelX = pixelX / 2.0f;
	float pixelY = 1.0f / IMG_HEIGHT;
	float halfPixelY = pixelY / 2.0f;

	
	Sprite catSprites[8][12];

	for (int i = 0; i < NUM_CHARACTERS_ROW * NUM_ROWS; i++)
	{
		int spritePlacement = 0;
		for (int j = 0; j < CHARACTER_SPRITES_Y; j++)
		{
			for (int k = 0; k < CHARACTER_SPRITES_X; k++)
			{
				if (i < NUM_CHARACTERS_ROW)
				{
					catSprites[i][spritePlacement].x = (k * pixelX * SPRITE_WIDTH) + (SPRITE_WIDTH / 2 * pixelX) + (i * CHARACTER_SPRITES_X * SPRITE_WIDTH * pixelX);
					catSprites[i][spritePlacement].y = (j * pixelY * SPRITE_HEIGHT) + (SPRITE_HEIGHT / 2 * pixelY) - pixelY;
					catSprites[i][spritePlacement].height = SPRITE_HEIGHT;
					catSprites[i][spritePlacement].width = SPRITE_WIDTH;
				}
				else
				{
					catSprites[i][spritePlacement].x = (k * pixelX * SPRITE_WIDTH) + (SPRITE_WIDTH / 2 * pixelX) + (i * CHARACTER_SPRITES_X * SPRITE_WIDTH * pixelX) - (NUM_CHARACTERS_ROW * CHARACTER_SPRITES_X * SPRITE_WIDTH * pixelX);
					catSprites[i][spritePlacement].y = (j * pixelY * SPRITE_HEIGHT) + (SPRITE_HEIGHT / 2 * pixelY) + (CHARACTER_SPRITES_Y * SPRITE_HEIGHT * pixelY) - pixelY;
					catSprites[i][spritePlacement].height = SPRITE_HEIGHT;
					catSprites[i][spritePlacement].width = SPRITE_WIDTH;
				}
				spritePlacement++;
			}
		}
	}
	

	//Place character in unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, characterTexture);
	//Make sampler2D _CharacterTexture sample from unit 1
	characterShader.setInt("_CharacterTexture", 1);

	ew::Vec2 spriteIndex = ew::Vec2(catSprites[0][11].x, catSprites[0][11].y);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Both use same quad mesh
		glBindVertexArray(backgroundQuadVAO);

		float timePassed = (float)glfwGetTime();

		//Draw background
		backgroundShader.use();
		backgroundShader.setFloat("iTime", timePassed);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		//setBackgroundShaderUniforms
		//Make sampler2D _BrickTexture sample from unit 
		backgroundShader.setInt("_GrassTexture", 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);

		//glBindVertexArray(catQuadVAO);

		characterShader.use();
		characterShader.setFloat("iTime", timePassed);
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		characterShader.setInt("_CharacterTexture", 1);
		characterShader.setFloat("_Scale", 1);
		characterShader.setFloat("_Opacity", 1);
		characterShader.setVec2("_CharacterPosition", 0.5,0.5);
		characterShader.setVec2("_SpriteUVCoordinates", spriteIndex.x, spriteIndex.y);
		characterShader.setVec2("_SpriteUVSize", SPRITE_WIDTH/IMG_WIDTH/2, SPRITE_HEIGHT/IMG_HEIGHT/2);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned short* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Buffer Object 
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, vertexData, GL_STATIC_DRAW);

	//Element Buffer Object
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numIndices, indicesData, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


