// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <stack>
#include "Camera.h"
#include "Serializer.h"
#include "FreeImagePlus.h"
#include "ElementCreator.h"
#include "ItemAtlas.h"
#include "Tile.h"
#include "World.h"
#include "Global.h"
#include "Vertices.h"
#include "DrawObject.h"
#include "Inputs.h"
#include <thread>
#include <mutex>
#include "ObjectLibrary.h"
#include "Render.h"

GLuint gGUIArrayTexture(0);
GLuint tilesVAO(0), vao2(0), vao3(0);
GLuint vbo(0), vbo2(0);

// Verteces
std::vector<float> v;

void prepareDraw();
void init();
void handlePlayerMovement(game_state* state, GLFWwindow* window);
void generateVBOs();
void createThingsToRender();
void generateTextures();
void mapUpdate();
void sortTileVector(std::vector<tile>& tileVector);
void Setup_Render_To_Texture();

using namespace std::chrono_literals;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

bool handle_events(GLFWwindow* window);

void update(game_state* state, GLFWwindow* window);

game_state interpolate(game_state const & current, game_state const & previous, float alpha) {
	game_state interpolated_state;
	interpolated_state.xCameraPos = xCameraPos;
	interpolated_state.yCameraPos = yCameraPos;

	// interpolate between previous and current by alpha here
	if (current.xGoal != xCameraPos) {
		interpolated_state.velocity = (current.xGoal - previous.xCameraPos) * alpha;
		double newPos = interpolated_state.velocity + xCameraPos;
		if (interpolated_state.velocity > 0.0  && newPos <= current.xGoal) {
			//printf("0. A Pixel: %f \n", double((2 / screenWidth)));
			//printf("1. Rest of a pixel %f \n", double(std::fmod(newPos, (2 / screenWidth))));
			//printf("2.Before: %f \n", double(newPos));
			if (std::fmod(newPos, (2 / screenWidth)) != 0) newPos -= std::fmod(newPos, (2 / screenWidth));
			//printf("3.After: %f \n", double(newPos));
			interpolated_state.xCameraPos = newPos;
			//current.xCameraPos = newPos;
		}
		else if (interpolated_state.velocity > 0.0  && newPos >= current.xGoal) {
			interpolated_state.xCameraPos = current.xGoal;
			//printf("0. A Pixel: %f \n", double((2 / screenWidth)));
			//printf("1. Rest of a pixel %f \n", double(std::fmod(interpolated_state.xCameraPos, (2 / screenWidth))));
			//printf("2.Before: %f \n", double(interpolated_state.xCameraPos));
			if (std::fmod(newPos, (2 / screenWidth)) != 0) interpolated_state.xCameraPos -= std::fmod(interpolated_state.xCameraPos, (2 / screenWidth));
			//printf("3.After: %f \n", double(interpolated_state.xCameraPos));
		}
	}
	if (current.yGoal != yCameraPos) {
		interpolated_state.velocity = (current.yGoal + previous.yCameraPos) * alpha;

		float newPos = interpolated_state.velocity + yCameraPos;
		if (interpolated_state.velocity < 0.0  && newPos >= current.yGoal) {
			//printf("%f interpolation Y velocity\n", float(interpolated_state.velocity));
			if (std::fmod(newPos, (2 / float(screenHeightPixels))) != 0) newPos -= std::fmod(newPos, (2.0 / float(screenHeightPixels)));
			//printf("3.After: %f \n", double(newPos));
			interpolated_state.yCameraPos = newPos;
		}
		else if (interpolated_state.velocity < 0.0  && newPos <= current.yGoal) {
			//printf("%f interpolation Y velocity\n", float(interpolated_state.velocity));
			interpolated_state.yCameraPos = current.yGoal;
			if (std::fmod(newPos, (2.0 / float(screenHeightPixels))) != 0) interpolated_state.yCameraPos -= std::fmod(interpolated_state.yCameraPos, (2.0 / float(screenHeightPixels)));
		}
	}

	return interpolated_state;
}

int main(int argc, wchar_t *argv[])
{
	if (!glfwInit())
	{
		std::cout << "glfwInit" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	//glfwWindowHint(GLFW_REFRESH_RATE, 60);
	//glfwWindowHint(GLFW_SAMPLES, 3);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//screenHeight = 1440.0f;
	//screenWidth = 2560.0f;
	screenHeight = 1080.0f;
	screenWidth = 1920.0f;

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Map Editor", nullptr, nullptr); // Windowed
	//GLFWwindow* window =
		//= glfwCreateWindow(screenWidth, screenHeight, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	// Get pixels
	glfwGetFramebufferSize(window, &screenWidthPixels, &screenHeightPixels);
	glViewport(0, 0, screenWidthPixels, screenHeightPixels);

	// Set width depending on the pixels
	width = (imgScale / (screenWidthPixels / 2));
	height = (imgScale / (screenHeightPixels / 2));


	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetKeyCallback(window, keyboard_button_callback);
	
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//glDepthRange(0.0f, 1.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	init();

	using clock = std::chrono::high_resolution_clock;


	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();
	bool quit_game = false;

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	long int times(0);

	while (!quit_game || !glfwWindowShouldClose(window)) {
		
		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			printf("%f ms/frame, %f fps\n", 1000.0 /double(nbFrames), double(nbFrames));
			//printf("%f fps\n", double(nbFrames));
			FPS = double(nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}
		
		auto delta_time = clock::now() - time_start;
		time_start = clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		quit_game = handle_events(window);
		times = 0;
		// update game logic as lag permits
		while (lag >= timestep) {
			lag -= timestep;
			times++;
			if (times == 1) {
				previous_state = current_state;
				update(&current_state, window); // update at a fixed rate each time
			}
		}
		//if(times > 1)
			//printf("Updated amount of times: %d\n", int(times));

		// calculate how close or far we are from the next timestep
		auto alpha = (float)lag.count() / timestep.count();
		auto interpolated_state = interpolate(current_state, previous_state, alpha);

		render(interpolated_state, window);
	}

	glDeleteProgram(program);


	for (auto objLib : objLibraries) {
		delete objLib;
	}

	glDeleteTextures(1, &gTileArrayTexture);
	glDeleteTextures(1, &gGUIArrayTexture);
	glDeleteFramebuffers(1, &fbo_palette_modifier_left);

	for (RendToText* rtt : renderToTextureContainer) {
		delete rtt;
	}

	for (auto panel : GUIPanels)
		delete panel;

	glDeleteBuffers(1, &instanceVBO);

	glfwTerminate();
	return 0;
}

bool handle_events(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return true;
	}

	//glfwSetWindowShouldClose(window, GL_TRUE);

	return false; // true if the user wants to quit the game
}

void update(game_state* state, GLFWwindow* window) {

	if (state->xGoal <= 1.0f) {
		state->xGoal = 1.0f;
		xCameraPos = 1.0f;
	}
	else {

		xCameraPos = state->xGoal;
	}
	if (state->yGoal >= -1.0f) {
		state->yGoal = -1.0f;
		yCameraPos = -1.0f;
	}
	else if (yCameraPos != state->yGoal) {

		yCameraPos = state->yGoal;
	}

	//generate_Palette_Modifier_Left_Texture();

	/*
	__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	if (time_stamp_world_time == 0 || now - time_stamp_world_time > 1 * 2 * 1000) {
		time_stamp_world_time = now;
		if ((worldTimeHour += 10) > 240) worldTimeHour = 0;
		float ambient = 0.0f;
		if (worldTimeHour > 0 && worldTimeHour < 120)
			ambientStrengthValue = (worldTimeHour / 120.0);
		else if (worldTimeHour > 190)
			ambientStrengthValue = (190.0 / worldTimeHour);
		else
			ambientStrengthValue = 1.0;
	}
	*/
	ambientStrengthValue = 1.0;

	//Update animations
	checkAnimations();

	// Do logic player movement logic here
	handlePlayerMovement(state, window);

	// Add/remove tile/item
	mapUpdate();

	// Update what to render onto the screen here
	createThingsToRender();

	//glfwPollEvents();
	glfwGetCursorPos(window, &xPos, &yPos);

	// Check hovering and update
	handelHover();

	//Prepare to draw
	prepareDraw();

	//Do GUI stuff
	for (auto panel : GUIPanels) {
		panel->checkUpdates();
	}
}

void createThingsToRender() {

	if (updateWorld) {
		__int64 then = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		world->clear_World();
		world->insertWorld(worldLoadTemp);
		delete worldLoadTemp;
		thingsToDraw.clear();
		worldLoadTemp = nullptr;
		updateWorld = false;
		changeWorldLock = false;
		outlinedItem = nullptr;
		__int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		printf("The time to swap the world took: %i ms\n", int(now - then));
	}

}

void generateTextures() {

	glActiveTexture(GL_TEXTURE0 + 0);

	//Generate an array texture
	glGenTextures(1, &gTileArrayTexture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, gTileArrayTexture);

	size_t texturesAmount = paths.size();

	//Create storage for the texture. (100 layers of 1x1 texels)
	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		1,                    //No mipmaps as textures are 1x1
		GL_RGBA16F,              //Internal format
		2048, 2048,                 //width,height
		texturesAmount                  //Number of layers
	);

	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Get texture from image to texture through FreeImage API
	fipWinImage img;
	std::string path;
	for (size_t i = 0; i < texturesAmount; ++i) {
		//verteces.at(i+1)->setTextureID(i);
		path = "./resources/images/" + paths[i] + ".png";
		bool read(false);
		read = img.load(&path[0]);
		printf("Loaded image size: %i \n", img.getImageSize());
		if(read) printf("Loaded image: %s \n", &path[0]);

		img.flipVertical();
		img.convertTo32Bits();
		//img.convertTo24Bits();
		//Specify i-essim image
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,                     //Mipmap number
			0, 0, i,                 //xoffset, yoffset, zoffset
			2048, 2048, 1,                 //width, height, depth
			GL_BGRA,                //format
			GL_UNSIGNED_BYTE,      //type
			(void*)img.accessPixels());                //pointer to data
		img.clear();
		//GL_BGRA
		/*glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
	}
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	//GLfloat Largest;

	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Largest);

	//glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, Largest);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

}

void mapUpdate() {

	if (!lbutton_down && thingsToDraw.size() > 0 && !changeWorldLock) {
		changeWorldLock = true;
		if (worldLoadTemp)
			worldLoadTemp->copyWorld(world);
		else {
			worldLoadTemp = new World(world->getName(), world->get_Floor_Amount(), world->get_Width_Section(), world->get_Height_Section(), world->get_Width_Section_Tiles());
			worldLoadTemp->insert_Sections_From_Other(world, z, sectionsActive);
		}
		std::thread(insert_Things_From_ThingsToDraw, std::ref(worldLoadTemp)).detach();
	}

	if (newZ != z && !updateMapFloor) {
		updateMapFloor = true;
		z = newZ;
		newFloor();
	}

	int section = -int((yCameraPos + 1.0) / ((world->get_Height_Section() * 64) / (float(screenHeightPixels) / 2))) * world->get_Width_Section() + int((xCameraPos - 1.0) / ((world->get_Width_Section_Tiles() * 64) / (float(screenWidthPixels) / 2)));
	if (section != currentSection) {
		currentSection = section;
		sectionsActive = get_Currently_Rendered_Sections();
	}

	check_If_Load_New_Sections();
}

void sortTileVector(std::vector<tile>& tileVector) {
	std::sort(tileVector.begin(), tileVector.end(), [](tile& lhs, tile& rhs) {
		if (lhs.getX() < rhs.getX()) return true;
		if (rhs.getX() < lhs.getX()) return false;

		// a=b for primary condition, go to secondary
		if (lhs.getY() < rhs.getY()) return true;
		if (rhs.getY() < lhs.getY()) return false;

		return false;
	});
}

void generateVBOs() {
	setupObjectLibraries();
}

void handlePlayerMovement(game_state* state, GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (moveRight) {
		state->xGoal = xCameraPos + (2 / screenWidth) * 1;
	}
	if (moveLeft) {
		state->xGoal = xCameraPos - (2.f / screenWidth) * 1.f;
	}
	if (moveUp) {
		state->yGoal = yCameraPos + (2 / screenHeight) * 1;
	}
	if (moveDown) {
		state->yGoal = yCameraPos - (2 / screenHeight) * 1;
	}
}

void Setup_Render_To_Texture() {
	renderToTextureContainer.push_back(new RendToText("paletteModifier", 2048, 2048));
	renderToTextureContainer.push_back(new RendToText("leftPanelTiles", 2048, 2048));
}

void init() {

	glGenBuffers(1, &instanceVBO);

	// Create and compile the vertex shader and fragment shader
	program = LoadShaders("./resources/Shaders/TransformVertexShader.vertexshader", "./resources/Shaders/ColorFragmentShader.fragmentshader");

	// Create and compile the vertex shader and fragment shader for rend to text
	program2 = LoadShaders("./resources/Shaders/RendToTextVertexShader.vertexshader", "./resources/Shaders/RendToTextFragmentShader.fragmentshader");

	// Create and compile the vertex shader and fragment shader for object outline
	outlineShader = LoadShaders("./resources/Shaders/outlineVertexShader.vertexshader", "./resources/Shaders/outlineFragmentShader.fragmentshader");

	{ // Setup normal shaders
		// Use our shader
		glUseProgram(program);

		GLuint uniformBlockIndex1 = glGetUniformBlockIndex(program, "Matrices");

		glUniformBlockBinding(program, uniformBlockIndex1, 0);

		GLuint uniformBlockIndexTextureStuff = glGetUniformBlockIndex(program, "textStuff");

		glUniformBlockBinding(program, uniformBlockIndexTextureStuff, 1);

		// Get a handle for our "cameraPos" uniform
		gLayer = glGetUniformLocation(program, "layer");

		outlineUniform = glGetUniformLocation(program, "outline");

		GUI = glGetUniformLocation(program, "GUI");

		ambientStrength = glGetUniformLocation(program, "ambientStrength");

		instancing = glGetUniformLocation(program, "instancing");
	}

	{ // Setup rend to text shaders
		// Use our shader
		glUseProgram(program2);

		GLuint uniformBlockIndex2 = glGetUniformBlockIndex(program2, "Matrices");

		glUniformBlockBinding(program2, uniformBlockIndex2, 0);

		GLuint uniformBlockIndexTextureStuff2 = glGetUniformBlockIndex(program2, "textStuff");

		glUniformBlockBinding(program2, uniformBlockIndexTextureStuff2, 1);
	}

	{ // Setup rend to text shaders
		// Use our shader
		glUseProgram(outlineShader);

		GLuint uniformBlockIndex3 = glGetUniformBlockIndex(outlineShader, "Matrices");

		glUniformBlockBinding(outlineShader, uniformBlockIndex3, 0);

		GLuint uniformBlockIndexTextureStuff3 = glGetUniformBlockIndex(outlineShader, "textStuff");

		glUniformBlockBinding(outlineShader, uniformBlockIndexTextureStuff3, 1);
	}

	{ // Setup Matrices
		glGenBuffers(1, &UBOCamera);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
		glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOCamera, 0, 3 * sizeof(glm::mat4));

		glViewport(0, 0, screenWidthPixels, screenHeightPixels);
		float aspect = screenWidth / screenHeight;
		glm::mat4 projection = //glm::ortho(-1.0f, 1.f, -1.0f, 1.f);
		glm::perspective(glm::radians(FOV), 1.0f, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOCamera);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	{ // Setup textStuff
		glGenBuffers(1, &UBOTextureStuff);
		glBindBuffer(GL_UNIFORM_BUFFER, UBOTextureStuff);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 1);

		glBindBufferRange(GL_UNIFORM_BUFFER, 1, UBOTextureStuff, 0, sizeof(glm::mat4));
	}

	// Create Vertex Array Object
	generateVBOs();

	// Generate itemAtlas
	itemAtlas.generateAtlas();

	Setup_Render_To_Texture();

	//Generate Textures
	generateTextures();

	// Get a handle for our "MVP" uniform
	//model = glGetUniformLocation(program, "model");

	Palette::fillPalettes(palettes);


	DrawObjects *temp = new DrawObjects("GUI_Preview_Tiles_");

	objects.push_back(temp);

	temp = new DrawObjects("GUI_BottomBar_");
	bottomBar->setObjects(temp);
	objects.push_back(temp);

	temp = new DrawObjects("GUI_LeftPanel_");
	leftPanel->setObjects(temp);
	objects.push_back(temp);

	temp = new DrawObjects("GUI_LeftPanel_Text_");
	objects.push_back(temp);

	temp = new DrawObjects("GUI_Item_Info_Panel_");
	itemInfo->setObjects(temp);
	objects.push_back(temp);

	temp = new DrawObjects("GUI_Palette_Modifier_");
	paletteModifier->setObjects(temp);
	objects.push_back(temp);

	generate_GUI_Bottom_Bar();

	generate_GUI_Left_Panel(0);
	
	//generate_GUI_Left_Panel_Text_(temp, VertecesHandler::findByName(verteces, "Letters_"));
	generate_Palette_Modifier();


	world = new World("Chunje", 14, 50, 50, 50);
}