#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>

#include <kmb/transformations.h>
#include <kmb/shader.h>
#include <kmb/texture.h>
#include <kmb/camera.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void moveCamera(GLFWwindow* window, kmb::Camera* camera, kmb::CameraControls* controls, float deltaTime);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	kmb::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	kmb::Camera camera;
	camera.position = ew::Vec3(0, 0, -5); //Looking down the -Z axis
	camera.target = ew::Vec3(0, 0, 0);
	camera.fov = 60;
	camera.orthoSize = 6;
	camera.nearPlane = 0.1;
	camera.farPlane = 100;
	camera.orthographic = false;
	camera.aspectRatio = static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT);

	ew::Vec3 startPosition = camera.position;
	ew::Vec3 targetStartPosition = camera.target;
	

	kmb::CameraControls cameraControls;
	//main.cpp
	float prevTime =0; //Timestamp of previous frame
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//Calculate deltaTime
		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		//Pass deltaTime into moveCamera. Update this function to include a 4th parameter.
		moveCamera(window, &camera, &cameraControls, deltaTime);

		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			shader.setMat4("_View", camera.ViewMatrix());
			shader.setMat4("_Projection", camera.ProjectionMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Camera");
			ImGui::DragFloat3("Position", &camera.position.x, 0.05f);
			ImGui::DragFloat3("Target", &camera.target.x, 0.05f);
			ImGui::Checkbox("Orthographic", &camera.orthographic);
			ImGui::DragFloat("Field of view", &camera.fov, 0.05f);
			ImGui::DragFloat("Orthographic Height", &camera.orthoSize, 0.05f);
			ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.05f);
			ImGui::DragFloat("Far Plane", &camera.farPlane, 0.05f);
			if (ImGui::Button("Reset"))
			{
				camera.position = startPosition;
				camera.target = targetStartPosition;
				cameraControls.yaw = cameraControls.startYaw;
				cameraControls.pitch = cameraControls.startPitch;
			}

			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void moveCamera(GLFWwindow* window, kmb::Camera* camera, kmb::CameraControls* controls, float deltaTime)
{
	//If right mouse is not held, release cursor and return early.
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just started right clicking, set prevMouse values to current position.
	//This prevents a bug where the camera moves as soon as we click.
	if (controls->firstMouse)
	{
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	//TODO: Get mouse position delta for this frame
	double mouseDeltaX = mouseX - controls->prevMouseX;
	double mouseDeltaY = mouseY - controls->prevMouseY;
	//TODO: Add to yaw and pitch
	//controls->pitch += sin(mouseDeltaY)/cos(mouseDeltaY);
	//controls->yaw += sin(mouseDeltaX)/cos(mouseDeltaX);

	controls->pitch += (mouseY - controls->prevMouseY) * controls->mouseSensitivity;
	controls->yaw += (mouseX - controls->prevMouseX) * controls->mouseSensitivity;
	//TODO: Clamp pitch between -89 and 89 degrees
	if (controls->pitch >= 89)
	{
		controls->pitch = 89;
	}
	else if (controls->pitch <= -89)
	{
		controls->pitch = -89;
	}

	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;
	
	//Construct forward vector using yaw and pitch. Don't forget to convert to radians!
	//ew::Vec3 forward = ew::Vec3((cos(controls->yaw) * cos(controls->pitch)) * 180 / 3.14, (sin(controls->pitch )) * 180 / 3.14, (sin(controls->yaw )*cos(controls->pitch)) * 180 / 3.14);
	ew::Vec3 forward = ew::Vec3(controls->yaw * 180 / 3.14, controls->pitch * 180 / 3.14, -camera->target.z);
	forward = ew::Normalize(forward);
	//By setting target to a point in front of the camera along its forward direction, our LookAt will be updated accordingly when rendering.
	

    //TODO: Using camera forward and world up (0,1,0), construct camera right and up vectors. Graham-schmidt process!
	ew::Vec3 right = ew::Vec3(1, 0, 0);
	ew::Vec3 up = ew::Vec3(0, 1, 0);

	ew::Vec3 f = ew::Normalize(ew::Vec3((cos(controls->yaw) * cos(controls->pitch)) * 180 / 3.14, (sin(controls->pitch)) * 180 / 3.14, (sin(controls->yaw) * cos(controls->pitch)) * 180 / 3.14));
	ew::Vec3 r = ew::Normalize(ew::Cross(f, up));
	ew::Vec3 u = ew::Normalize(ew::Cross(r,f));
	//TODO: Keyboard controls for moving along forward, back, right, left, up, and down. See Requirements for key mappings.
	//EXAMPLE: Moving along forward axis if W is held.
	//Note that this is framerate dependent, and will be very fast until you scale by deltaTime. See the next section.
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera->position += r * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera->position -= r * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera->position += f * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera->position -= f * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		camera->position += u * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		camera->position -= u * controls->moveSpeed * deltaTime;
	}

	//Setting camera.target should be done after changing position. Otherwise, it will use camera.position from the previous frame and lag behind
	camera->target = camera->position + forward;
	
}
