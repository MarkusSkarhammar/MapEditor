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
#include "VertecesHandler.h"
#include "Object.h"
#include "Inputs.h"
#include <thread>
#include <mutex>


GLuint gTileArrayTexture(0);
GLuint gGUIArrayTexture(0);
GLuint gLayer(0), textOffset(0);
GLuint tilesVAO(0), vao2(0), vao3(0);
GLuint vbo(0), vbo2(0);
GLuint program(0);

// Verteces
std::vector<float> v;

void init();
void handlePlayerMovement(game_state* state, GLFWwindow* window);
void generateVBOs();
void createThingsToRender();
void generateTextures();
void mapUpdate();
void sortTileVector(std::vector<tile>& tileVector);

using namespace std::chrono_literals;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

bool handle_events(GLFWwindow* window);

void update(game_state* state, GLFWwindow* window);

void render(game_state const &, GLFWwindow* window);

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
	//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
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
			printf("%f ms/frame\n", 1000.0 /double(nbFrames));
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


	for (auto vert : verteces) {
		glDeleteBuffers(1, &vert.getVBO());
		glDeleteVertexArrays(1, &vert.getVAO());
	}

	glDeleteTextures(1, &gTileArrayTexture);
	glDeleteTextures(1, &gGUIArrayTexture);

	glfwTerminate();
	return 0;
}

void render(game_state const &interpolated_state, GLFWwindow* window) {

	// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update camera based on a interpolated state
	setCameraPosition(interpolated_state.xCameraPos, interpolated_state.yCameraPos);

	bool overTen(false);
	size_t from = 0, obj = 0;
	size_t to = z;
	if (z > 6) {
		from = 7;
		to = z;
	}
	std::string currentName = "";
	itemsToDraw.clear();
	itemsToDraw.shrink_to_fit();

	//----------------------------------------------------------
	//						Draw game stuff
	//----------------------------------------------------------
	// Draw order tiles > Borders > Entities > Doodads

	double x(0), y(0);
	for (int floorAt = from; floorAt <= to; floorAt++) {
		for (auto& t : world.getFloor(floorAt).getSection(currentSection)) {
			auto& object = t->getObject();
			x = 0.0 + (width * object->getXPosition());
			y = 0.0 - (height * object->getYPosition());
			if (x < xCameraPos + 1.0f * zoom && x >= xCameraPos - 1.1f * zoom && y <= yCameraPos + 1.2f * zoom && y > yCameraPos - 1.0f * zoom) { // Check to see if object is within viewport
				if (object->getDraw()) {
					Model = glm::mat4(1.0f);

					Model = glm::translate(Model, glm::vec3(x, y, 0.0f));

					glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));

					glUniform1i(gLayer, object->getTexturePos());

					glBindVertexArray(object->getVAO());
					glDrawArrays(GL_TRIANGLES, 0 + (4 * object->getID() % 1024), 3);
					glDrawArrays(GL_TRIANGLES, 1 + (4 * object->getID() % 1024), 3);
				}
				for (auto& i : t->getAllItems()) {
					itemsToDraw.push_back(i->getObject());
				}
			}
		}

		for (auto& object : itemsToDraw) {
			x = 0.0 + (width * object->getXPosition());
			y = 0.0 - (height * object->getYPosition());
			if (object->getDraw()) { // Check to see if object is within viewport

				Model = glm::mat4(1.0f);

				Model = glm::translate(Model, glm::vec3(x, y, 0.0f));

				glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));

				glUniform1i(gLayer, object->getTexturePos());

				glBindVertexArray(object->getVAO());
				glDrawArrays(GL_TRIANGLES, 0 + (4 * object->getID() % 1024), 3);
				glDrawArrays(GL_TRIANGLES, 1 + (4 * object->getID() % 1024), 3);
			}
		}
		itemsToDraw.clear();
	}

	//----------------------------------------------------------
	//						Draw GUI
	//----------------------------------------------------------

	for (auto& objects : objects) {
		currentName = objects.getName();
		if (!((currentName.compare("GUI_LeftPanel_DropDown_") == 0 || currentName.compare("GUI_LeftPanel_DropDown_Text_") == 0) && !clickPaletteDropDown)) {
			for (auto& object : objects.getObjects()) {

				Model = glm::mat4(1.0f);
				if (currentName.compare("GUI_Preview_Tiles_") == 0) Model = glm::translate(Model, glm::vec3(xCameraPos - 1.0f + object->getXPosition(), yCameraPos + 1.0f - object->getYPosition(), 0.0));
				else Model = glm::translate(Model, glm::vec3(xCameraPos - 1.0f + object->getXPosition(), yCameraPos + 1.0f - object->getYPosition(), (zoom - 1.0)));
				
				// Handle scaling
				if(object->getScale() != 1.0) 
					Model = glm::scale(Model, glm::vec3(object->getScale(), object->getScale(), object->getScale()));

				// Handle transformation
				glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(Model));

				if (object->getTextOffsetX() > 0 || object->getTextOffsetY() > 0) {
					textOffsetValues = { object->getTextOffsetX(), object->getTextOffsetY() };
					glUniformMatrix2fv(textOffset, 1, GL_FALSE, glm::value_ptr(textOffsetValues));
				}

				glUniform1i(gLayer, object->getTexturePos());

				glBindVertexArray(object->getVAO());
				glDrawArrays(GL_TRIANGLES, 0 + (4 * object->getID() % 1024), 3);
				glDrawArrays(GL_TRIANGLES, 1 + (4 * object->getID() % 1024), 3);

			}
		}
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
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
}

void createThingsToRender() {

	
	if (updateMap) {
		world.copyWorld(worldTemp);
		thingsToDraw.clear();
		updateMap = false;
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
		GL_RGBA16,              //Internal format
		2048, 2048,                 //width,height
		texturesAmount                  //Number of layers
	);

	//Get texture from image to texture through FreeImage API
	fipWinImage img;
	std::string path;
	for (size_t i = 0; i < texturesAmount; ++i) {
		verteces.at(i).setTextureID(i);
		path = "./resources/images/" + paths[i].first + ".png";
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
		//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 4);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


		/*glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);*/
	}

	//GLfloat Largest;

	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Largest);

	//glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY_EXT, Largest);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

}

void mapUpdate() {

	if (!lbutton_down && !updateMap && thingsToDraw.size() > 0 && !updateMapFloor) {
		updateMapFloor = true;
		std::thread(insert_Things_Form_ThingsToDraw).detach();
	}

	if (newZ != z && !updateMapFloor) {
		updateMapFloor = true;
		z = newZ;
		newFloor();
	}

	if (int((xCameraPos - 1.0) / ((50 * 64) / (float(screenWidthPixels) / 2))) != currentSection) {
		currentSection = (xCameraPos - 1) / ((50 * 64) / (float(screenWidthPixels) / 2));
	}
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


	for (auto path : paths ) {
		VertecesHandler temp = VertecesHandler(path.first, path.second);
		if (path.second > 0) {
			generateVetecesSquares(temp);

			glGenVertexArrays(1, &temp.getVAO());

			glGenBuffers(1, &temp.getVBO()); // Generate 1 buffer

											 //vector<float> *tilesVptr = &temp.getVerteces();

			glBindVertexArray(temp.getVAO());
			glBindBuffer(GL_ARRAY_BUFFER, temp.getVBO());
			glBufferData(GL_ARRAY_BUFFER, temp.getVerteces().size() * sizeof(float), &temp.getVerteces()[0], GL_STATIC_DRAW);


			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SPosition), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SPosition), (GLvoid*)8);
		}
		else if (path.second == -1) {

			generateGUI(temp, path.first);

			glGenVertexArrays(1, &temp.getVAO());

			glGenBuffers(1, &temp.getVBO()); // Generate 1 buffer

											 //vector<float> *tilesVptr = &temp.getVerteces();

			glBindVertexArray(temp.getVAO());
			glBindBuffer(GL_ARRAY_BUFFER, temp.getVBO());
			glBufferData(GL_ARRAY_BUFFER, temp.getVerteces().size() * sizeof(float), &temp.getVerteces()[0], GL_STATIC_DRAW);


			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SPosition), (GLvoid*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SPosition), (GLvoid*)8);
		}

		verteces.push_back(temp);
	}

	//printf("%s interpolation X velocity\n", "heehej");
	//printf("%i <--- is it true? \n",  VertecesHandler::findByName(verteces, "Tiles024").getName() == "Tiles_1024" );
}

void handlePlayerMovement(game_state* state, GLFWwindow* window) {
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	if (moveRight) {
		state->xGoal = xCameraPos + (2 / screenWidth) * 1;
	}
	if (moveLeft) {
		state->xGoal = xCameraPos - (2 / screenWidth) * 1;
	}
	if (moveUp) {
		state->yGoal = yCameraPos + (2 / screenHeight) * 1;
	}
	if (moveDown) {
		state->yGoal = yCameraPos - (2 / screenHeight) * 1;
	}
}


void init() {
	// Create Vertex Array Object
	generateVBOs();


	// Create and compile the vertex shader and fragment shader

	program = LoadShaders("./TransformVertexShader.vertexshader", "./ColorFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(program);


	//glUniform1i(0, 0); //Sampler refers to texture unit 0

					   //Generate Textures
	generateTextures();

	// Get a handle for our "MVP" uniform
	model = glGetUniformLocation(program, "model");
	view = glGetUniformLocation(program, "view");
	projection = glGetUniformLocation(program, "projection");

	// Get a handle for our "cameraPos" uniform
	gLayer = glGetUniformLocation(program, "layer");

	// Get a handle for our "textOffset" uniform
	textOffset = glGetUniformLocation(program, "textOffset");

	fillPalettes(palettes);

	for (size_t i = 0; i < MAX_FLOORS; i++) {
		objects.push_back(Objects("Floor_Tiles_" + std::to_string(i)));
		objects.push_back(Objects("Floor_Items_" + std::to_string(i)));
		tiles.push_back(Objects("Tiles_" + std::to_string(i)));
		items.push_back(Objects("Items_" + std::to_string(i)));
	}

	Objects temp = Objects("GUI_Preview_Tiles_");
	objects.push_back(temp);
	temp = Objects("GUI_BottomBar_");
	generate_GUI_Bottom_Bar(temp, VertecesHandler::findByName(verteces, "GUI_1"));
	objects.push_back(temp);
	temp = Objects("GUI_BottomBar_Text_");
	generate_GUI_Bottom_Bar_text(temp, VertecesHandler::findByName(verteces, "Letters_"), "X:" + std::to_string(x), "Y:" + std::to_string(y), "Z:" + std::to_string(z));
	objects.push_back(temp);
	temp = Objects("GUI_LeftPanel_");
	generate_GUI_Left_Panel(temp, VertecesHandler::findByName(verteces, "GUI_1"), 0);
	objects.push_back(temp);
	temp = Objects("GUI_LeftPanel_Text_");
	generate_GUI_Left_Panel_Text_(temp, VertecesHandler::findByName(verteces, "Letters_"));
	objects.push_back(temp);
	temp = Objects("GUI_Item_Info_Panel_");
	objects.push_back(temp);
	temp = Objects("GUI_Palette_Modifier_");
	generate_Palette_Modifier(temp, VertecesHandler::findByName(verteces, "GUI_1"));
	objects.push_back(temp);

}