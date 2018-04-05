// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <shader.hpp>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <stack>
#include "FreeImagePlus.h"
#include "ElementCreator.h"
#include "ItemAtlas.h"
#include "Tile.h"
#include "World.h"
#include "Serializer.h"
#include "Global.h"
#include <thread>
#include <mutex>

GLuint gTileArrayTexture(0);
GLuint gGUIArrayTexture(0);
GLuint vao(0), vao2(0), vao3(0);
GLuint vbo(0), vbo2(0);
GLuint program(0);

bool firstFill(true);

// Concurrency locks
std::mutex updateTileVerteces, updateItemVerteces, updateAmountBeforeGUI, updateBuffer;

size_t previousX = 0, previousY = 0;

// Verteces
std::vector<float> v;

// VBO floors (0 = tiles, 1 = Entities, 2 = Items)
GLuint vbos[3]{0};

glm::mat4 MVP;
GLuint MatrixID;

ElementCreator ec;

bool runMapUpdater(true);
bool updateMap(false);

void setCameraPosition(float xPos, float yPos);
void handlePlayerMovement();
bool checkCollision(float speed, size_t direction);
void generateVBOs();
void updateMapContinuously();
void updateWhatToDrawOnAllCurrentFloors();
void updateWhatToDrawOnFloor(size_t floor, bool tiles, bool items);
void updateWhatToDrawOnFloors(size_t fromFloor, size_t toFloor, bool tiles, bool items);
void createThingsToRender(GLFWwindow* window, size_t& amount, std::vector<tile>& tiles);
void generateTextures();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
size_t createSquare(float widthStart, float heightStart, float width, float height, float textWidthStart, float textHeightStart, float textWidth, float textHeight, std::vector<float>& v);
size_t getTextureLocation(size_t id, bool GUI);
void mapUpdate();
void handelHover();
void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset);
void sortTileVector(std::vector<tile>& tileVector);
void insertCommand(tile tile);
void undoCommand();
void redoCommand();
void doCommand(tile& t, bool undo);
void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateCameraPosition(float xPos, float yPos);
void setCameraZoom(float scale);

using namespace std::chrono_literals;

// we use a fixed timestep of 1 / (60 fps) = 16 milliseconds
constexpr std::chrono::nanoseconds timestep(16ms);

struct game_state {
	// this contains the state of your game, such as positions and velocities
};

bool handle_events(GLFWwindow* window);

void update(game_state* state, GLFWwindow* window, size_t& amount);

void render(game_state const &, GLFWwindow* window, size_t& amount);

game_state interpolate(game_state const & current, game_state const & previous, float alpha) {
	game_state interpolated_state;

	// interpolate between previous and current by alpha here

	return interpolated_state;
}

int main(int argc, wchar_t *argv[])
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
	//glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL", nullptr, nullptr); // Windowed
	//GLFWwindow* window =
		//glfwCreateWindow(1920, 1080, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	ptrdiff_t pos = std::find_if(&paths[0], &paths[0] + sizeOfPaths, [](const std::string& s) { return (s.compare("Letters") == 0); }) - &paths[0];
	letterPath = pos;

	size_t amount = 0;

	vector<size_t> tilesToDraw;
	for (size_t i = 0; i < 14; ++i) {
		amountOfTilesToDrawLevelsBuffer.push_back(tilesToDraw);
	}
	for (size_t i = 0; i < 14; ++i) {
		amountOfItemsToDrawLevelsBuffer.push_back(tilesToDraw);
	}

	// Create Vertex Array Object
	//GLuint vao;
	generateVBOs();

	/*vector<ItemInfo> tilesInFirstTexture = itemAtlas.getTiles();
	textWidth = (64 / 2048.0f);
	textHeight = (64 / 2048.0f);
	textHeightStart = 1.0f;
	textWidthStart = 0.0f;

	size_t i = 0;
	size_t j = 0;
	float tempTextStartWidth, tempTextStarHeight, tempTextWidth, tempTextHeight;
	for (auto& t : tilesInFirstTexture) {

		size_t id = t.getPosInText();
		while (id > 32) {
			id -= 32;
			++j;
		}
		i = id - 1;

		tempTextStartWidth = textWidthStart + (textWidth * i);
		tempTextStarHeight = textHeightStart + (textHeight * j);
		//First triangle
		tilesVertecesColor.push_back(textWidthStart); tilesVertecesColor.push_back(textHeightStart); // Top-left
		tilesVertecesColor.push_back(textWidthStart + textWidth); tilesVertecesColor.push_back(textHeightStart); // Top-right
		tilesVertecesColor.push_back(textWidthStart); tilesVertecesColor.push_back(textHeightStart - textHeight); // Bottom-left

																	//Second triangle
		tilesVertecesColor.push_back(textWidthStart + textWidth); tilesVertecesColor.push_back(textHeightStart); // Top-right
		tilesVertecesColor.push_back(textWidthStart + textWidth); tilesVertecesColor.push_back(textHeightStart - textHeight); // Bottom-right
		tilesVertecesColor.push_back(textWidthStart); tilesVertecesColor.push_back(textHeightStart - textHeight); // Bottom-left

	}*/


	//glEnableVertexAttribArray(vao);

	// Create vertex buffer object
	//GLuint vbo;
	//glGenBuffers(1, &vbo); // Generate 1 buffer
							// Create Vertex Array Object
	/*//GLuint vao;
	glGenVertexArrays(1, &vao2);
	glBindVertexArray(vao2);
	glEnableVertexAttribArray(vao2);
	glGenBuffers(1, &vbo2); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, 400000 * sizeof(float), nullptr, GL_STATIC_DRAW);*/
	//createThingsToRender(window, amount, tiles);

	std::thread mapUpdate(updateMapContinuously);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glVertexAttribPointer(0, 7, GL_FLOAT, GL_FALSE, 0, NULL);


	// Create and compile the vertex shader and fragment shader
	program = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(program);
	

	glUniform1i(0, 0); //Sampler refers to texture unit 0

	//Generate Textures
	generateTextures();

	// Get a handle for our "MVP" uniform
	MatrixID = glGetUniformLocation(program, "MVP");

	updateCameraPosition(0.0f, 0.0f);

	using clock = std::chrono::high_resolution_clock;



	std::chrono::nanoseconds lag(0ns);
	auto time_start = clock::now();
	bool quit_game = false;

	game_state current_state;
	game_state previous_state;

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	while (!quit_game || !glfwWindowShouldClose(window)) {

		// Measure speed
		double currentTime = glfwGetTime();
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
											 // printf and reset timer
			//printf("%f ms/frame\n", 1000.0 /double(nbFrames));
			printf("%f fps\n", double(nbFrames));
			FPS = double(nbFrames);
			nbFrames = 0;
			lastTime += 1.0;
		}

		auto delta_time = clock::now() - time_start;
		time_start = clock::now();
		lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

		quit_game = handle_events(window);

		// update game logic as lag permits
		while (lag >= timestep) {
			lag -= timestep;
			
			previous_state = current_state;
			update(&current_state, window, amount); // update at a fixed rate each time
			
		}

		// calculate how close or far we are from the next timestep
		auto alpha = (float)lag.count() / timestep.count();
		auto interpolated_state = interpolate(current_state, previous_state, alpha);

		render(interpolated_state, window, amount);
	}

	glDeleteProgram(program);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &vbos[0]);
	glDeleteBuffers(1, &vbos[1]);
	glDeleteBuffers(1, &vbos[2]);

	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &vao2);

	glDeleteTextures(1, &gTileArrayTexture);
	glDeleteTextures(1, &gGUIArrayTexture);

	runMapUpdater = false;
	mapUpdate.join();

	glfwTerminate();
	return 0;
}

void render(game_state const &, GLFWwindow* window, size_t& amount) {
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	//glLoadIdentity();

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glLoadIdentity();
	
	//----------------------------------------------------------
	//						Draw tiles/creatures/items
	//----------------------------------------------------------
	if (amountBeforeGUI > 0) {
		size_t vertecesTilePosition = 0;
		size_t vertecesItemPosition = 0;
		float xMax = xCameraPos + 1;
		float yMax = yCameraPos - 1;
		size_t from, to, prevText;
		if (z > 6) {
			for (size_t i = 0; i < 7; ++i) {
				if (tilesVertecesRight.size() > 0) vertecesTilePosition += amountOfTilesToDrawLevelsDraw.at(i).size();
				if (itemsVertecesDraw.size() > 0) vertecesItemPosition += amountOfItemsToDrawLevelsDraw.at(i).size();
			}
			to = z;
			from = 7;
		}
		else {
			from = 0;
			to = z;
		}
		for (size_t level = from; level < to + 1; ++level) {

			// Draw the tiles of a level
			vector<size_t> *tilesAtLevel = &amountOfTilesToDrawLevelsDraw.at(level);
			size_t size = tilesAtLevel->size();
			if (drawTiles) {
				glBindVertexArray(vao);
				//float *vptrTiles = &tilesVerteces2[0][0];
				float *vptrTiles = &tilesVertecesRight[0];
				//glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
				for (size_t i = 0; i < size; ++i) {
					if (i == 0) {
						prevText = tilesAtLevel->at(i);
						glUniform1i(gTileArrayTexture, getTextureLocation(prevText, false));

					};
					float xPos = vptrTiles[(vertecesTilePosition * 24)];
					float yPos = vptrTiles[(vertecesTilePosition * 24 + 1)];
					if (xPos >= xCameraPos - 1.12f && xPos <= xMax &&
						yPos <= yCameraPos + 1.12f && yPos >= yMax) {
						//draw points 0-3 from the currently bound VAO with current in-use shader
						if (i > 0 && (size_t(tilesAtLevel->at(i) / 1024) != size_t(prevText / 1024))) {
							prevText = tilesAtLevel->at(i);
							glUniform1i(gTileArrayTexture, getTextureLocation(prevText, false));
						}
						glDrawArrays(GL_TRIANGLES, 0 + vertecesTilePosition * 6, 6);
					}
					++vertecesTilePosition;
				}
			}

			// Draw the items of a level
			if (drawWalls && (itemsVertecesDraw.size() > 0)) {
				glBindVertexArray(vao3);
				float *vptrItems = &itemsVertecesDraw[0];
				vector<size_t> itemsAtLevel = amountOfItemsToDrawLevelsDraw.at(level);
				size = itemsAtLevel.size();
				for (size_t i = 0; i < size; ++i) {
					if (i == 0) {
						prevText = itemsAtLevel[i];
						glUniform1i(gTileArrayTexture, getTextureLocation(itemsAtLevel.at(i), false));
					}
					float xPos = vptrItems[(vertecesItemPosition * 24)];
					float yPos = vptrItems[(vertecesItemPosition * 24 + 1)];
					if (xPos >= xCameraPos - 1.23f && xPos <= xMax &&
						yPos <= yCameraPos + 1.23f && yPos >= yMax) {
						//draw points 0-3 from the currently bound VAO with current in-use shader
						if (i > 0 && (size_t(itemsAtLevel[i] / 1024) != size_t(prevText / 1024))) {
							prevText = itemsAtLevel[i];
							glUniform1i(gTileArrayTexture, getTextureLocation(prevText, false));
						}
						glDrawArrays(GL_TRIANGLES, 0 + vertecesItemPosition * 6, 6);
					}
					++vertecesItemPosition;
				}
			}

		}
	}
	


	glBindVertexArray(vao2);
	size_t startAtVector;

	//----------------------------------------------------------
	//						Draw middle tile
	//----------------------------------------------------------
	if (drawMiddle) {
		startAtVector = GUIElements.size();
		//draw points 0-3 from the currently bound VAO with current in-use shader
		glUniform1i(gTileArrayTexture, getTextureLocation(1, true));
		glDrawArrays(GL_TRIANGLES, 0 + (startAtVector) * 6, 6);
	}

	//----------------------------------------------------------
	//						Draw preview tiles
	//----------------------------------------------------------
	startAtVector = GUIElements.size() + 1;
	for (size_t i = 0; i < addItem.size(); ++i) {
		//draw points 0-3 from the currently bound VAO with current in-use shader
		if (eraseToggle || destroyToggle) {
			glUniform1i(gTileArrayTexture, getTextureLocation(1, true));
		}
		else
			glUniform1i(gTileArrayTexture, getTextureLocation(preview.getID(), false));
		glDrawArrays(GL_TRIANGLES, 0 + (i + startAtVector) * 6, 6);
	}

	//----------------------------------------------------------
	//					Draw preview tile at cursor
	//----------------------------------------------------------
	if ((isWithinTileArea || !drawGUI) && (item.size() != 0 || eraseToggle || destroyToggle)) {
		size_t i = GUIElements.size() + addItem.size() + 1;
		if (brush == 0) {
			//draw points 0-3 from the currently bound VAO with current in-use shader
			if (eraseToggle || destroyToggle) {
				glUniform1i(gTileArrayTexture, getTextureLocation(1, true));
			}
			else
				glUniform1i(gTileArrayTexture, getTextureLocation(preview.getID(), false));
			glDrawArrays(GL_TRIANGLES, 0 + i * 6, 6);
		}
		else {
			size_t to = (brush + 2) * (brush + 2);
			for (size_t k = 0; k < to; ++k) {
				//draw points 0-3 from the currently bound VAO with current in-use shader
				if (eraseToggle || destroyToggle) {
					glUniform1i(gTileArrayTexture, getTextureLocation(1, true));
				}
				else
					glUniform1i(gTileArrayTexture, getTextureLocation(preview.getID(), false));
				glDrawArrays(GL_TRIANGLES, 0 + k + i * 6, 6);
			}
		}
	}

	if (drawGUI) {
		//----------------------------------------------------------
		//						Draw GUI
		//----------------------------------------------------------
		for (size_t i = 0; i < GUIElements.size(); ++i) {
			if (i == 0 || i > 0 && std::abs((int)(GUIElements.at(i - 1) - GUIElements.at(i))) > 0) {
				glUniform1i(gTileArrayTexture, getTextureLocation(GUIElements.at(i), true));
			}
			glDrawArrays(GL_TRIANGLES, 0 + (i) * 6, 6);
		}
	}
	
	//glDisableVertexAttribArray(vao);
	
	if (!updateGUI) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

bool handle_events(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		return true;
	}


	//glfwSetWindowShouldClose(window, GL_TRUE);

	return false; // true if the user wants to quit the game
}

void update(game_state* state, GLFWwindow* window, size_t& amount) {
	glfwGetCursorPos(window, &xPos, &yPos);

	// Check hovering and update
	handelHover();

	// Add/remove tile/item
	mapUpdate();

	// Do logic player movement logic here
	handlePlayerMovement();

	// Update what to render onto the screen here
	createThingsToRender(window, amount, tiles);
}

void createThingsToRender(GLFWwindow* window, size_t& amount, std::vector<tile>& tiles) {
	vector<float> *vptr = &v;
	vector<float> *tilesVptr = &tilesVerteces;


	/*if(updateTiles || updateItems || updateAll){
		vptr->clear();
		vptr->shrink_to_fit();

		vptr->insert(vptr->end(), tilesVerteces.begin(), tilesVerteces.end());
		vptr->insert(vptr->end(), itemsVerteces.begin(), itemsVerteces.end());

		updateTiles = false;
		updateItems = false;

	}*/

	ec.createGUIs(*vptr);
	//vptr->erase(vptr->begin() + tilesVerteces.size() + itemsVerteces.size(), vptr->end());
	//vptr->insert(vptr->begin() + tilesVerteces.size() + itemsVerteces.size(), GUIVerteces.begin(), GUIVerteces.end());

	if (updateTiles) {
		updateBuffer.lock();
		tilesVertecesRight.clear();
		tilesVertecesRight.insert(tilesVertecesRight.begin(), tilesVerteces.begin(), tilesVerteces.end());
		amountBeforeGUI = amountTilesBeforeGUI + amountItemsBeforeGUI;
		amountOfTilesToDrawLevelsDraw = amountOfTilesToDrawLevelsBuffer;
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, tilesVptr->size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, tilesVerteces.size() * sizeof(float), &tilesVerteces[0], GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, tilesVertecesRight.size() * sizeof(float), &tilesVertecesRight[0]);
		updateTiles = false;
		updateBuffer.unlock();
	}

	if (updateItems) {
		updateBuffer.lock();
		itemsVertecesDraw.clear();
		itemsVertecesDraw.insert(itemsVertecesDraw.begin(), itemsVertecesBuffer.begin(), itemsVertecesBuffer.end());
		amountBeforeGUI = amountTilesBeforeGUI + amountItemsBeforeGUI;
		amountOfItemsToDrawLevelsDraw = amountOfItemsToDrawLevelsBuffer;
		glBindVertexArray(vao3);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(GL_ARRAY_BUFFER, itemsVertecesBuffer.size() * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER, tilesVerteces.size() * sizeof(float), &tilesVerteces[0], GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, itemsVertecesDraw.size() * sizeof(float), &itemsVertecesDraw[0]);
		updateItems = false;
		updateBuffer.unlock();
	}

	/*if (updateTiles) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, tilesVerteces2[0].size() * sizeof(float), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, tilesVerteces2[0].size() * sizeof(float), &tilesVerteces2[0][0]);

		if(tilesVerteces2.size() < 8) updateTiles = false;
	}

	if (updateTiles && tilesVerteces2.size() > 7) {
		glBindVertexArray(vao3);
		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(GL_ARRAY_BUFFER, tilesVerteces2[7].size() * sizeof(float), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, tilesVerteces2[7].size() * sizeof(float), &tilesVerteces2[7][0]);

		updateTiles = false;
	}*/

	if (GUIVerteces.size() > 0) {
		glBindVertexArray(vao2);
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		glBufferData(GL_ARRAY_BUFFER, GUIVerteces.size() * sizeof(float), nullptr, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, GUIVerteces.size() * sizeof(float), &GUIVerteces[0]);
	}

}

void generateTextures() {

	//Generate an array texture
	glGenTextures(1, &gTileArrayTexture);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, gTileArrayTexture);

	size_t texturesAmount = sizeOfPaths;

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
		path = "./resources/images/" + paths[i] + ".png";
		img.load(&path[0]);

		img.convertTo32Bits();
		//Specify i-essim image
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
			0,                     //Mipmap number
			0, 0, i,                 //xoffset, yoffset, zoffset
			2048, 2048, 1,                 //width, height, depth
			GL_BGRA,                //format
			GL_UNSIGNED_BYTE,      //type
			(void*)img.accessPixels());                //pointer to data
		img.clear();
	}
	
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT ) {
		if (action == GLFW_PRESS) {

			if (isWithinTileArea && lControl) {
				std::vector<tile>& tiles = world.getFloor(z).getSection(currentSection);
				size_t xTile = x, yTile = y;
				auto it = std::find_if(tiles.begin(), tiles.end(), [xTile, yTile](tile& t) { return t.getX() == x && t.getY() == y; });
				if (it != tiles.end()) {
					selectedItem = -1;
					tileInfo = &(*it);
					tileInfoWindow = true;
					tileWindowUpdate = true;
					updateGUI = true;
				}
			}

			if ((xPos >= 1851.0 && xPos <= 1890.0) && (yPos >= 110.0 && yPos <= 137.0)) {
				if (GUILeftPanelElement[0] == 0) {
					GUILeftPanelElement[0] = 1;
				}
				else {
					GUILeftPanelElement[0] = 0;
				}
				updateGUI = true;
			}
			else if ((xPos >= 1471.0 && xPos <= 1550.0) && (yPos >= 40.0 && yPos <= 67.0)) {
				eraseToggle = !eraseToggle;
				destroyToggle = false;
				updateGUI = true;
			}
			else if ((xPos >= 1553.0 && xPos <= 1632.0) && (yPos >= 40.0 && yPos <= 67.0)) {
				destroyToggle = !destroyToggle;
				eraseToggle = false;
				updateGUI = true;
			}
			else if (verticalScrollBarHover) {
				verticalScrollbar = true;
			}
			else if (horizontalScrollBarHover) {
				horizontalScrollbar = true;
			}

			if (tileInfoWindow && (xPos >= 806.0 && xPos <= 847.0) && (yPos >= 595.0 && yPos <= 624.0)) {
				changeArticle = true;
				tileWindowUpdate = true;
				updateGUI = true;
			}

			if (tileInfoWindow && (xPos >= 892.0 && xPos <= 923.0) && (yPos >= 345.0 && yPos <= 378.0)) {
				changeItemLeft = true;
				tileWindowUpdate = true;
				updateGUI = true;
			}

			if (tileInfoWindow && (xPos >= 1006.0 && xPos <= 1036.0) && (yPos >= 345.0 && yPos <= 378.0)) {
				changeItemRight= true;
				tileWindowUpdate = true;
				updateGUI = true;
			}

			if (GUILeftPanelElement[0] == 1 && ((xPos >= 1477.0 && xPos <= 1892.0) && (yPos >= 145.0 && yPos <= (145.0 + GUILeftPanelElement[1] * 41)))) {
				GUILeftPanelElement[3] = (yPos - 145) / 41;
				getItemsToRenderOnSelectionArea = true;
				GUILeftPanelElement[0] = 0;
				contentSelectionSelected[0] = 1000;
				selectedID = 100000;
				item.clear();
				updateGUI = true;
			}
			else if (GUILeftPanelElement[0] != 1 && (xPos >= 1456.0 && xPos <= 1904.0) && (yPos >= 181 && yPos <= (1258.0))) {
				if (GUILeftPanelElement[3] != 1000) {
					item.clear();
					size_t x, y;
					x = (xPos - 1456) / 60;
					y = (yPos - 181) / 60;
					size_t temp = (x + y * 7) + 1;
					if (temp <= max_selected) {
						selectedID = temp;
						contentSelectionSelected[0] = x;
						contentSelectionSelected[1] = y;
						item.push_back( itemAtlas.getItem(itemAtlas.getSelectedItemID(pos.at(selectedID - 1), selectedID)) );
						//updateGUI = true;
					}
				}
			}
			lbutton_down = true;
		}
		else if (GLFW_RELEASE == action) {
			lbutton_down = false;
			if (changeArticle) {
				changeArticle = false;
				if (article.compare("") != 0) {
				tileInfo->setArticle(article);
				}
				tileWindowUpdate = true;
				updateGUI = true;
			}
			if (changeItemLeft) {
				if (selectedItem > -1) --selectedItem;
				changeItemLeft = false;
				tileWindowUpdate = true;
				updateGUI = true;
			}
			if (changeItemRight) {
				if (selectedItem < int(tileInfo->getAllItems().size()) - 1) ++selectedItem;
				//cout << tileInfo->getAllItems().size();
				changeItemRight = false;
				tileWindowUpdate = true;
				updateGUI = true;
			}
		}

	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (GUILeftPanelElement[0] == 1) {
			GUILeftPanelElement[2] = 1000;
			GUILeftPanelElement[0] = 0;
			//updateGUI = true;
		}
		if (lbutton_down) {
			addItem.clear();
			addItem.resize(0);
			lbutton_down = false;
			//updateGUI = true;
		}
		else {
			contentSelectionSelected[0] = 1000;
			selectedID = 100000;
			item.clear();
			eraseToggle = false;
			destroyToggle = false;
			//updateGUI = true;
		}
		rbutton_down = true;
	}
	else if (GLFW_RELEASE == action)
		rbutton_down = false;

	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		if (lControl) {
			imgScale = 64;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_5 && action == GLFW_PRESS) {
		if (!tileInfoWindow && z < 13) {
			z += 1;
			if( z == 7) updateMap = true;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_4 && action == GLFW_PRESS) {
		if (!tileInfoWindow && z > 0) {
			z -= 1;
			if (z == 6) updateMap = true;
		}
	}
}

void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if(action == GLFW_PRESS)
			lControl = true;
		else if (GLFW_RELEASE == action)
			lControl = false;
	}
	
	if (key == GLFW_KEY_Z) {
		if (lControl && action == GLFW_PRESS) {
			undoCommand();
		}
	}

	if (key == GLFW_KEY_Y) {
		if (lControl && action == GLFW_PRESS) {
			redoCommand();
		}
	}

	if (key == GLFW_KEY_S) {
		if (lControl && action == GLFW_PRESS) {
			serializer.saveWorld(world);
		}
	}

	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		if (lControl) {
			serializer.loadWorld(world);
			updateMap = true;
		}
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (lControl) {
			drawMiddle = !drawMiddle;
		}
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS) {
		if (lControl) {
			drawGUI = !drawGUI;
		}
		else {
			drawLeftPanelGUI = !drawLeftPanelGUI;
		}
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (lControl) {
			for (size_t z = 0; z < 14; ++z) {
				vector<tile>& section = world.getFloor(z).getSection(0);
				section.clear();
				section.shrink_to_fit();
				for (size_t y = 0; y < 50; ++y) {
					for (size_t x = 0; x < 50; ++x) {
						section.push_back(tile(x, y, z, z + 1));
					}
				}
			}
			updateWhatToDrawOnFloor(7, true, true);
		}
	}

	if (key == GLFW_KEY_R) {
		if (action == GLFW_PRESS) {
			if (lControl) {
				world.getFloor(z).getSection(currentSection).clear();
				//updateWhatToDrawOnFloor(z, true, true);
				//updateWhatToDrawOnAllCurrentFloors();
			}
			else {
				int i = 0;
				size_t maxY = (currentSection / 40) * 50 + 50;
				size_t maxX = (currentSection % 40) * 50 + 50;
				bool removeMore = false;
				do {
					i = 0;
					for (auto& t : world.getFloor(z).getSection(currentSection)) {
						if (t.getY() >= maxY || t.getX() >= maxX) {
							world.getFloor(z).getSection(currentSection).erase(world.getFloor(z).getSection(currentSection).begin() + i);
							removeMore = true;
							break;
						}
						i++;
						if (i == world.getFloor(z).getSection(currentSection).size()) removeMore = false;
					}
				} while (removeMore);
				updateWhatToDrawOnAllCurrentFloors();
			}
		}
	}

	if (key == GLFW_KEY_ESCAPE) {
		if (tileInfoWindow) tileInfoWindow = false;
	}

	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			 if (!tileInfoWindow) {
				moveRight = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveRight = false;
		}
	}

	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		if (lControl) {
			drawTiles = !drawTiles;
		}

	}

	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			if (!tileInfoWindow) {
				//updateCameraPosition(-(imgScale / (1920 / 2)), 0);
				moveLeft = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveLeft = false;
		}
	}

	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			if (lControl) {
				drawWalls = !drawWalls;
			}else if (!tileInfoWindow) {
				//updateCameraPosition(0, (imgScale / (1080 / 2)));
				moveUp = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveUp = false;
		}
	}

	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			if (!tileInfoWindow) {
				//updateCameraPosition(0, -(imgScale / (1080 / 2)));
				moveDown = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			moveDown = false;
		}
	}
	
}

void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	if (!tileInfoWindow && !lControl && !rbutton_down && isWithinTileArea && yOffset != 0) {
		updateCameraPosition(0, yOffset * (imgScale / 540));
		if (yCameraPos > 0.0f) {
			yCameraPos = 0.0f;
			updateCameraPosition(0, 0);
		}
		//updateAll = true;
	}else if (!tileInfoWindow && !lControl && isWithinTileArea && yOffset != 0) {
		updateCameraPosition(yOffset * (imgScale / 960), 0);
		if ( xCameraPos < 0.0f) {
			xCameraPos = 0.0f;
			updateCameraPosition(0, 0);
		}
		//updateAll = true;
	}

	if (lControl && yOffset != 0) {
		zoom += yOffset;
		setCameraZoom(zoom/90);
	}
}

size_t createSquare(float widthStart, float heightStart, float width, float height, float textWidthStart, float textHeightStart, float textWidth, float textHeight, std::vector<float>& v) {

	//First triangle
	v.push_back(widthStart); v.push_back(heightStart); v.push_back(textWidthStart); v.push_back(textHeightStart); // Top-left
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthStart + textWidth); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightStart - textHeight); // Bottom-left

	//Second triangle
	v.push_back(widthStart + width); v.push_back(heightStart); v.push_back(textWidthStart + textWidth); v.push_back(textHeightStart); // Top-right
	v.push_back(widthStart + width); v.push_back(heightStart  - height); v.push_back(textWidthStart + textWidth); v.push_back(textHeightStart - textHeight); // Bottom-right
	v.push_back(widthStart); v.push_back(heightStart - height); v.push_back(textWidthStart); v.push_back(textHeightStart - textHeight); // Bottom-left

	return 1;
}

size_t getTextureLocation(size_t id, bool GUI) {
	size_t location = 0;
	std::string path;
	if(!GUI) path = itemAtlas.getTextureLocation(id);
	else path = itemAtlas.getGUITextureLocation(id);
	ptrdiff_t pos = std::find_if(&paths[0], &paths[0] + sizeOfPaths, [path](const std::string& s) { return (s.compare(path) == 0); }) - &paths[0];
	location = pos;
	return location;
}

void mapUpdate() {

	//----------------------------------------
	//	Render several sections if necessary
	//----------------------------------------
	size_t xSectionRight = size_t(xCameraPos / (imgScale / (1920 / 2)));
	size_t xSectionLeft = size_t(xCameraPos / (imgScale / (1920 / 2)));
	size_t ySection = -(yCameraPos / (imgScale / (1080 / 2)));

	bool testSectionRight = (sectionRight && xSectionRight % 50 == 10);
	bool testSectionLeft = (!sectionRight && (size_t)(xSectionRight / SECTION_LENGTH) != (size_t)((xSectionRight + (1920 / imgScale)) / SECTION_LENGTH));
	bool testSectionDown = (sectionBelow && ySection % 50 == 10);
	bool testSectionUp = (!sectionBelow && (size_t)(ySection / SECTION_LENGTH) != (size_t)((ySection + (1080 / imgScale)) / SECTION_LENGTH) );
	if (testSectionRight || testSectionLeft || testSectionDown || testSectionUp) {

		currentSection = xSectionLeft / SECTION_LENGTH;
		currentSection += (ySection / SECTION_LENGTH) * 40;
		if (testSectionRight) {
			sectionRight = false;
		}
		if (testSectionDown) {
			sectionBelow = false;
		}
		if (testSectionLeft && (size_t)(ySection / SECTION_LENGTH) == (size_t)((ySection + (1080 / imgScale)) / SECTION_LENGTH)) {
			sectionBelow = false;
		}
		//if (testSectionRight && (size_t)(ySection / SECTION_LENGTH) == (size_t)((ySection + (1080 / imgScale)) / SECTION_LENGTH)) {
			//sectionBelow = false;
		//}
		updateMap = true;
	}

	if (!sectionRight) {
		size_t checkRight = ((xCameraPos / (imgScale / (1920 / 2))) + (1920 / imgScale) + 2);
		if (checkRight / SECTION_LENGTH != xSectionRight / SECTION_LENGTH) {
			sectionRight = true;
			updateMap = true;
		}
	}
	/*else{
		size_t checkRight = ((xCameraPos / (imgScale / (1920 / 2))) + (1920 / imgScale) + 10) / SECTION_LENGTH;
		if (xSection == checkRight) {
			sectionRight = false;
			updateMap = true;
		}
	}*/
	if (!sectionBelow) {
		size_t checkDown = -(yCameraPos / (imgScale / (1080 / 2))) + (1080 / imgScale);
		if (checkDown / SECTION_LENGTH != ySection / SECTION_LENGTH) {
			sectionBelow = true;
			updateMap = true;
		}
	}
	/*else {
		size_t checkDown = size_t((-(yCameraPos / (imgScale / (1080 / 2))) + (1080 / imgScale)) / SECTION_LENGTH) * 40;
		if (ySection == checkDown) {
			sectionBelow = false;
			updateMap = true;
		}
	}*/
	if (!sectionSouthEast) {
		if (sectionBelow && sectionRight) {
			sectionSouthEast = true;
			updateMap = true;
		}
	}
	else if(!(sectionBelow && sectionRight)){
		sectionSouthEast = false;
		updateMap = true;
	}

	//if (updateMap) updateWhatToDrawOnAllCurrentFloors();
		/*
	if (updateTiles || updateItems) {
		size_t xSection = size_t(((xCoord / 64) / 200));
		size_t ySection = size_t(((yCoord / 64) / 200)) * SECTION_LENGTH;
		if (ySection != currentSection || xSection != currentSection) {
			if (ySection == currentSection + SECTION_LENGTH || ySection == currentSection - SECTION_LENGTH) {
				sectionBelow = false;
				sectionSouthEast = false;
			}
			if (xSection != currentSection) {
				sectionRight = false;
				sectionSouthEast = false;
			}
			currentSection = xSection;
			currentSection += ySection;
		}

		size_t checkAboveBelow = (size_t(yCoord / 64) % 200);
		if (!sectionBelow && checkAboveBelow + 17 > 200) {
			std::vector<tile> temp = world.getFloor(z).getSection(currentSection + SECTION_LENGTH);
			if (temp.size() > 0) {
				updateTiles = true;
				updateItems = true;
			}
			sectionBelow = true;
		}
		else if (sectionBelow && !(checkAboveBelow > 183 && checkAboveBelow < 200)  ) {
			sectionBelow = false;
			sectionSouthEast = false;
		}

		size_t checkRightLeft = (size_t((xCoord / 64)) % 200);
		if (!sectionRight && checkRightLeft + 22 >= 200) {
			std::vector<tile> temp = world.getFloor(z).getSection(currentSection + 1);
			if (temp.size() > 0) {
			updateTiles = true;
			updateItems = true;
			}
			sectionRight = true;
		}
		else if (sectionRight && !(checkRightLeft > 177 && checkRightLeft < 200)) {
			sectionRight = false;
			sectionSouthEast = false;
		}

		if (sectionBelow && sectionRight) {
			sectionSouthEast = true;
		}
	}
	*/
	//---------------------------------------------------
	//			Add/remove/modify tiles/items
	//---------------------------------------------------
	if (addItem.size() > 0 && !lbutton_down) {
		std::pair<size_t, std::pair<size_t, size_t>> position;
		if (destroyToggle) {
			for (size_t i = 0; i < addItem.size(); ++i) {
				position = addItem.at(i);
				size_t x = position.second.first, y = position.second.second, floor = z;
				std::vector<tile>& tileVector = world.getFloor(z).getSection(position.first);
				auto it = std::find_if(tileVector.begin(), tileVector.end(), [x, y, floor](tile& t) { return ((t.getX() == x) && (t.getY() == y) && (t.getZ() == z)); });
				if (it != tileVector.end()) {
					insertCommand(*it);
					tileVector.erase(it);
					updateWhatToDrawOnFloor(z, true, true);
				}
			}
		}
		else if (eraseToggle) {
			for (size_t i = 0; i < addItem.size(); ++i) {
				position = addItem.at(i);
				size_t x = position.second.first, y = position.second.second, floor = z;
				std::vector<tile>& tileVector = world.getFloor(z).getSection(position.first);
				auto it = std::find_if(tileVector.begin(), tileVector.end(), [x, y, floor](tile& t) { return ((t.getX() == x) && (t.getY() == y) && (t.getZ() == z)); });
				if (it != tileVector.end()) {
					insertCommand((*it));
					(*it).clearItems();
					updateWhatToDrawOnFloor(z, false, true);
				}
			}
		}
		else if (item.at(0).getType().compare("Tiles") == 0) {
			for (size_t i = 0; i < addItem.size(); ++i) {
				position = addItem.at(i);
				size_t x = position.second.first, y = position.second.second, floor = z;
				std::vector<tile>& tileVector = world.getFloor(z).getSection(position.first);
				auto it = std::find_if(tileVector.begin(), tileVector.end(), [x, y, floor](tile& t) { return ((t.getX() == x) && (t.getY() == y) && (t.getZ() == z)); });
				Item tempItem = item.at(0);
				if (it != tileVector.end()) {
					insertCommand(tile((*it).getX(), (*it).getY(), (*it).getZ(), tempItem.getID() + 100000));
					(*it).setID(tempItem.getID());
					(*it).setBlockPathfind(tempItem.getBlockPathfind());
				}
				else {
					tile t = tile(x, y, z, item.at(0).getID());
					if (item.at(0).getBlockPathfind() == 1) t.setBlockPathfind(true);
					tile t2 = t;
					t2.setID(t.getID() + 100000);
					insertCommand(t2);
					tileVector.push_back(t);
				}
			}
			sortTileVector(world.getFloor(z).getSection(position.first));
			updateWhatToDrawOnFloor(z, true, false);
		}
		else {
			for (size_t i = 0; i < addItem.size(); ++i) {
				position = addItem.at(i);
				size_t x = position.second.first, y = position.second.second, floor = z;
				std::vector<tile>& tileVector = world.getFloor(z).getSection(position.first);
				auto it = std::find_if(tileVector.begin(), tileVector.end(), [x, y, floor](tile& t) { return ((t.getX() == x) && (t.getY() == y) && (t.getZ() == z)); });
				if (it != tileVector.end()) {
					(*it).getAllItems().push_back(item.at(0));
					tile t = (*it);
					t.setID(t.getID() + 200000);
					insertCommand(t);
				}
				else {
					tile t(x, y, z, 0);
					t.insertItem(item.at(0));
					tileVector.push_back(t);
					sortTileVector(tileVector);
					tile t2 = t;
					t2.setID(t.getID() + 100000);
					insertCommand(t2);
				}
			}
			sortTileVector(world.getFloor(z).getSection(position.first));
			updateWhatToDrawOnFloor(z, false, true);
		}
		addItem.clear();
		addItem.shrink_to_fit();
	}

	//---------------------------------------------------------
	//	   Get the position where to add/modify tiles/items
	//---------------------------------------------------------
	if (lbutton_down) {
		if ((isWithinTileArea || !drawGUI) && (item.size() != 0 || eraseToggle || destroyToggle)) {
			size_t xTile = x, yTile = y, section = 0;
			section = size_t((xCameraPos / (imgScale / 960)) + xPos / 64) / SECTION_LENGTH;
			section += size_t((-yCameraPos / (imgScale / 540) + yPos / 64) / SECTION_LENGTH) * 40;
			auto it = std::find_if(addItem.begin(), addItem.end(), [xTile, yTile](const std::pair<size_t, std::pair<size_t, size_t>>& p) { return (p.second.first == xTile && p.second.second == yTile); });
			if (it == addItem.end()) {
				addItem.push_back(std::pair<size_t, std::pair<size_t, size_t>>(section, std::pair<size_t, size_t>(xTile, yTile)));
			}
		}
	}
}

void handelHover() {

	// Drawing screen vertical scrollbar
	if (verticalScrollbar) {
		if (yPos > 34.0 && yPos < 998.0) {
			verticalScrollBarY = yPos;
			if (yPos == 36.0) {
				yCoord = 0.0;
			}
			else if (yPos == 997.0) {
				yCoord = 126956.0;
			}
			else
			{
				yCoord = (verticalScrollBarY - 35.0f) * 132.07;
				if (yCoord > 126956.0) yCoord = 126956.0;
			}
		}
		if(!lbutton_down) verticalScrollbar = false;
	}
	if ((xPos >= 1421.0 && xPos <= 1440.0) && (yPos >= (verticalScrollBarY - 20) && yPos <= (verticalScrollBarY + 20))) {
		verticalScrollBarHover = true;
	}
	else {
		verticalScrollBarHover = false;
	}
	// Drawing screen horizontal scrollbar
	if (horizontalScrollbar) {
		if (xPos > 34.0 && xPos < 1387.0) {
			horizontalScrollBarX = xPos;
			if (xPos == 36.0) {
				xCoord = 0.0;
			}
			else if (xPos == 1386.0) {
				xCoord = 126528.0;
			}
			else
			{
				xCoord = (horizontalScrollBarX - 35.0f) * 94.1;
				if (xCoord > 126528.0) xCoord = 126528.0;
			}
		}
		if (!lbutton_down) horizontalScrollbar = false;
	}
	if ((xPos >= (horizontalScrollBarX - 20) && xPos <= (horizontalScrollBarX + 20) && (yPos >= 1032.0 && yPos <= 1051.0) )) {
		horizontalScrollBarHover = true;
	}
	else {
		horizontalScrollBarHover = false;
	}

	if ((!tileInfoWindow && (xPos >= 0.0 && xPos <= 1421.0) && (yPos >= 0.0 && yPos <= (1032))) || !drawGUI) {
		isWithinTileArea = true;
		//contentSelectionHover[0] = 1000;
		scrollBarHover = 0;
		x = /*(xCoord / imgScale)*/ ( xCameraPos / (imgScale / (1920 / 2)) ) + (xPos / imgScale);
		y = -(yCameraPos / (imgScale / (1080 / 2))) + (yPos / imgScale);
		updateGUI = true;
	}
	else {
		isWithinTileArea = false;
		scrollBarHover = 0;
		//contentSelectionHover[0] = 1000;
		if (GUILeftPanelElement[0] == 1) {
			//double xPos, yPos;
			//glfwGetCursorPos(window, &xPos, &yPos);
			if ((xPos >= 1477.0 && xPos <= 1892.0) && (yPos >= 145.0 && yPos <= (145.0 + GUILeftPanelElement[1] * 41))) {
				GUILeftPanelElement[2] = (yPos - 145) / 41;
				updateGUI = true;
			}
			else {
				GUILeftPanelElement[2] = 1000;
			}
		}
	}
	if ((xPos >= 1421.0 && xPos <= 1440.0) && (yPos >= 0.0 && yPos <= (1051))) {
		if (scrollBarHover == 0 && (xPos >= 1421.0 && xPos <= 1440.0) && (yPos >= 1018.0 && yPos <= (1032.0))) {
			scrollBarHover = 2;
		}
		else if (scrollBarHover == 0 && (xPos >= 1421.0 && xPos <= 1440.0) && (yPos >= 0.0 && yPos <= (14.0))) {
			scrollBarHover = 1;
		}
		else {
			//scrollBarHover = 0;
		}
	}
	if (GUILeftPanelElement[0] != 1 && (xPos >= 1456.0 && xPos <= 1904.0) && (yPos >= 181 && yPos <= (1258.0))) {
		if (GUILeftPanelElement[3] != 1000) {
			contentSelectionHover[0] = (xPos - 1456) / 60;
			contentSelectionHover[1] = (yPos - 181) / 60;
			updateGUI = true;
		}
	}
	else {
		contentSelectionHover[0] = 1000;
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

void insertCommand(tile tile) {
	if (commands.size() > 0) {
		commands.erase(commands.begin() + atCommand, commands.end());
	}
	if (commands.size() > maxCommands ) {
		commands.erase(commands.begin());
	}
	commands.push_back(tile);
	atCommand = commands.size();
}

void undoCommand() {
	if (atCommand > 0) {
		tile t = commands.at(atCommand - 1);
		--atCommand;

		doCommand(t, true);
	}
};

void redoCommand() {
	if (atCommand < commands.size()) {
		tile t = commands.at(atCommand);
		++atCommand;

		doCommand(t, false);
	}
}

void doCommand(tile& t, bool undo) {
	size_t x = t.getX(), y = t.getY();
	size_t sect(0);
	sect += size_t(x / 200);
	sect += size_t(y / 200) * SECTION_LENGTH;

	if (undo) {
		std::vector<tile>& section = world.getFloor(t.getZ()).getSection(sect);
		auto it = std::find_if(section.begin(), section.end(), [x, y](tile t) { return t.getX() == x && t.getY() == y; });
		if (t.getID() < 100000) {
			if (it != section.end()) {
				(*it) = t;
			}
			else
			{
				section.push_back(t);
				sortTileVector(section);
			}
		}
		else if(t.getID() < 200000) {
			if (it != section.end()) {
				section.erase(it);
				sortTileVector(section);
			}
		}
		else {
			if (it != section.end()) {
				(*it).getAllItems().erase((*it).getAllItems().end() - 1);
			}
		}
	}
	else {
		std::vector<tile>& section = world.getFloor(t.getZ()).getSection(sect);
		auto it = std::find_if(section.begin(), section.end(), [x, y](tile t) { return t.getX() == x && t.getY() == y; });
		if (t.getID() < 100000) {
			if (it != section.end()) {
				section.erase(it);
			}
		}
		else if(t.getID() < 200000) {
			t.setID(t.getID() - 100000);
			if (it != section.end()) {
				(*it) = t;
			}
			else
			{
				section.push_back(t);
				sortTileVector(section);
			}
		}
		else {
			t.setID(t.getID() - 200000);
			if (it != section.end()) {
				(*it) = t;
			}
		}
	}
	
}

void updateCameraPosition(float xPos, float yPos) {
	xCameraPos += xPos;
	yCameraPos += yPos;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(zoom), 1.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(xCameraPos, yCameraPos, 1.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(xCameraPos, yCameraPos, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	/*size_t currentX = (size_t)(xCameraPos / (imgScale / (1920 / 2))), currentY = (size_t)(-(yCameraPos / (imgScale / (1080 / 2))));
	if (currentX != previousX || currentY != previousX) {
		previousX = currentX;
		previousY = currentY;
		if (z < 7) {
			updateWhatToDrawOnFloors(0, z, true, true);
		}
		else
			updateWhatToDrawOnFloors(7, z, true, true);
	}*/

	updateGUI = true;
}

void setCameraPosition(float xPos, float yPos) {
	xCameraPos = xPos;
	yCameraPos = yPos;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(zoom), 1.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(xCameraPos, yCameraPos, 1.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(xCameraPos, yCameraPos, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	updateGUI = true;
}

void setCameraZoom(float scale) {

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(zoom), 1.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(xCameraPos, yCameraPos, 1.0f), // Camera is at (4,3,-3), in World Space
		glm::vec3(xCameraPos, yCameraPos, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	updateGUI = true;
}

void updateMapContinuously() {
	while (runMapUpdater) {
		while (runMapUpdater && !updateMap) {
			std::this_thread::sleep_for(0.1s);
		}
		if (updateMap) {
			updateBuffer.lock();
			updateWhatToDrawOnAllCurrentFloors();
			updateBuffer.unlock();
			updateMap = false;
			updateTiles = true;
			updateItems = true;
		}
	}
}

void updateWhatToDrawOnAllCurrentFloors() {

	if (z < 7) {
		updateWhatToDrawOnFloors(0, 6, true, true);
	}
	else {
		updateWhatToDrawOnFloors(7, 13, true, true);
	}
}

void updateWhatToDrawOnFloors(size_t fromFloor, size_t toFloor, bool tiles, bool items) {
	/*std::thread floors[7];
	size_t offset(0);
	if (fromFloor >= 7) offset = 7;
	for (size_t i = fromFloor; i < toFloor + 1; ++i) {
		floors[i - offset] = std::thread(updateWhatToDrawOnFloor, i, tiles, items);
	}
	for (size_t i = 0; i < 7; ++i) {
		floors[i].join();
	}*/
	for (size_t i = fromFloor; i < toFloor + 1; ++i) {
		updateWhatToDrawOnFloor( i, tiles, items);
	}
}

void updateWhatToDrawOnFloor(size_t floor, bool tiles, bool items) {
	float widthStart = -1.0f;
	float heightStart = 1.0f;
	float width = (imgScale / 960.0f);
	float height = (imgScale / 540.0f);
	float textWidth = (64 / 2048.0f);
	float textHeight = (64 / 2048.0f);
	float textHeightStart = 1.0f;
	float textWidthStart = 0.0f;
	size_t startX = xCameraPos / (imgScale / (1920 / 2));
	size_t startY = -(yCameraPos / (imgScale / (1080 / 2)));
	size_t endX = startX + (1920 / 64);
	size_t endY = startY + (1080 / 64);
	
	if (tiles) {
		updateTileVerteces.lock();
		size_t id = 0;
		size_t i = 0;
		size_t j = 0;

		vector<size_t> tilesToDraw;

		std::vector <std::vector<tile>> tileSectionsToDraw;

		size_t amount(0);
		for (size_t i = 0; i < floor; ++i) {
			amount += amountOfTilesToDrawLevelsBuffer.at(i).size() * 24;
		}

		tilesVerteces.erase(tilesVerteces.begin() + amount, tilesVerteces.begin() + amount + amountOfTilesToDrawLevelsBuffer.at(floor).size() * 24);

		vector<float> temp;

		tileSectionsToDraw.clear();
		tileSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection));

		tilesToDraw.clear();
		tilesToDraw.shrink_to_fit();
		if (sectionBelow) tileSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection + 40));
		if (sectionRight) tileSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection + 1));
		if (sectionSouthEast) tileSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection + 41));
		for (auto& tileVect : tileSectionsToDraw) {
			//temp.clear();
			for (auto& t : tileVect) {
				size_t x = t.getX(), y = t.getY();
			//	if (x >= startX && x <= endX && y >= startY && y <= endY) {
				i = 0;
				j = 0;
				id = itemAtlas.getItemTexturePosition(t.getID());
				while (id > 32) {
					id -= 32;
					++j;
				}
				i = id - 1;

				if (id != 0) {
					// Draw a normal tile
					createSquare(widthStart + (width * t.getX()), heightStart - (height * t.getY()), width, height, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, temp);
					tilesToDraw.push_back(t.getID());

				}
				//}
			}
		}
		//tilesVerteces2.push_back(temp);
		tilesVerteces.insert(tilesVerteces.begin() + amount, temp.begin(), temp.end());
		amountOfTilesToDrawLevelsBuffer[floor] = tilesToDraw;
		//updateTiles = true;
		updateTileVerteces.unlock();
	}

	if (items) {
		updateItemVerteces.lock();
		size_t id = 0;
		size_t i = 0;
		size_t j = 0;

		std::vector<size_t> itemsToDraw;
		std::vector <std::vector<tile>> itemSectionsToDraw;

		size_t amount(0);
		for (size_t i = 0; i < floor; ++i) {
			amount += amountOfItemsToDrawLevelsBuffer.at(i).size() * 24;
		}
		itemsVertecesBuffer.erase(itemsVertecesBuffer.begin() + amount, itemsVertecesBuffer.begin() + amount + amountOfItemsToDrawLevelsBuffer.at(floor).size() * 24);

		vector<float> temp;

		itemsToDraw.clear();
		itemsToDraw.shrink_to_fit();

		itemSectionsToDraw.clear();
		itemSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection));
		if (sectionBelow) itemSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection + 40));
		if (sectionRight) itemSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection + 1));
		if (sectionSouthEast) itemSectionsToDraw.push_back(world.getFloor(floor).getSection(currentSection + 41));
		for (auto& itemVect : itemSectionsToDraw) {
			for (auto& t : itemVect) {
				for (auto& item : t.getAllItems()) {
					size_t x = t.getX(), y = t.getY();
					//if (x >= startX && x <= endX && y >= startY && y <= endY) {
					i = 0;
					j = 0;
					id = itemAtlas.getItemTexturePosition(item.getID());
					while (id > 32) {
						id -= 32;
						++j;
					}
					i = id - 1;
					if (isDoubleSize(item.getID())) {
						createSquare(widthStart + (width * t.getX()) - width, heightStart - (height * t.getY()) + height, width * 2, height * 2, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, temp);
					}
					else {
						// Draw a normal tile
						createSquare(widthStart + (width * t.getX()), heightStart - (height * t.getY()), width, height, textWidthStart + (textWidth * i), textHeightStart + (textHeight * j), textWidth, textHeight, temp);
					}
					itemsToDraw.push_back(item.getID());
					//}
				}
			}
		}
		itemsVertecesBuffer.insert(itemsVertecesBuffer.begin() + amount, temp.begin(), temp.end());
		amountOfItemsToDrawLevelsBuffer[floor] = itemsToDraw;
		//updateTiles = true;
		updateItemVerteces.unlock();
	}

	amountItemsBeforeGUI = 0;
	for (auto& v : amountOfItemsToDrawLevelsBuffer) {
		amountItemsBeforeGUI += v.size();
	}
	amountTilesBeforeGUI = 0;
	for (auto& v : amountOfTilesToDrawLevelsBuffer) {
		amountTilesBeforeGUI += v.size();
	}
}

void generateVBOs() {
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 0 * sizeof(float), nullptr, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)8);

	/*glGenBuffers(1, &vbos[0]); // Generate 1 buffer
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, 0 * sizeof(float), nullptr, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)8);*/


	glGenVertexArrays(1, &vao3);
	glGenBuffers(1, &vbos[0]); // Generate 1 buffer
	glBindVertexArray(vao3);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER, 0 * sizeof(float), nullptr, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)8);

	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2); // Generate 1 buffer
	glBindVertexArray(vao2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, 0 * sizeof(float), nullptr, GL_STATIC_DRAW);

	//Specify vertex attributes for the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex2D), (GLvoid*)8);
}

bool checkCollision(float speed, size_t direction) {
	size_t newPositionX, oldPositionX, oldPositionY, newPositionY;
	switch (direction)
	{
	case 0:
		newPositionX = ((xCameraPos + speed) / (imgScale / 960.0f)) + size_t((960 / imgScale) + 1.5), oldPositionX = ((xCameraPos) / (imgScale / 960.0f)) + size_t((960 / imgScale) + 1.5),
			oldPositionY = size_t(-yCameraPos / (imgScale / 540.0f)) + size_t((540 / imgScale) + 0.5), newPositionY = size_t((-yCameraPos + speed) / (imgScale / 540.0f)) + size_t((540 / imgScale) + 0.5);
		if (newPositionX == lastCheckedX && oldPositionY == lastCheckedY) return false;
		if (newPositionX > oldPositionX) {
			if (!(newPositionX / 50 > oldPositionX / 50)) {
				auto worldVect = world.getFloor(z).getSection(currentSection);
				auto it = std::find_if(worldVect.begin(), worldVect.end(), [newPositionX, oldPositionY](tile& t) {
					return (t.getX() == newPositionX && t.getY() == oldPositionY);
				});
				if (it != worldVect.end()) {
					if ((*it).getBlockPathfind()) {
						lastCheckedX = newPositionX;
						lastCheckedY = oldPositionY;
						return false;
					}
					for (Item i : (*it).getAllItems()) {
						if (i.getBlockPathfind()) {
							lastCheckedX = newPositionX;
							lastCheckedY = oldPositionY;
							return false;
						}
					}
				}
			}
		}
		break;
	case 2:
		newPositionX = ((xCameraPos + speed) / (imgScale / 960.0f)) + size_t((960 / imgScale) + 0.5), oldPositionX = ((xCameraPos) / (imgScale / 960.0f)) + size_t((960 / imgScale) + 0.5),
			oldPositionY = size_t(-yCameraPos / (imgScale / 540.0f)) + size_t((540 / imgScale) + 1.5), newPositionY = size_t(-(yCameraPos + speed) / (imgScale / 540.0f)) + size_t((540 / imgScale) + 1.5);
		if (oldPositionX == lastCheckedX && newPositionY == lastCheckedY) return false;
		if (newPositionY > oldPositionY) {
			if (!(newPositionY / 50 > oldPositionY / 50)) {
				auto worldVect = world.getFloor(z).getSection(currentSection);
				auto it = std::find_if(worldVect.begin(), worldVect.end(), [oldPositionX, newPositionY](tile& t) {
					return (t.getX() == oldPositionX && t.getY() == newPositionY);
				});
				if (it != worldVect.end()) {
					if ((*it).getBlockPathfind()) {
						lastCheckedX = newPositionX;
						lastCheckedY = oldPositionY;
						return false;
					}
					for (Item i : (*it).getAllItems()) {
						if (i.getBlockPathfind()) {
							lastCheckedX = newPositionX;
							lastCheckedY = oldPositionY;
							return false;
						}
					}
				}
			}
		}
		break;
	}
	return true;
}

void handlePlayerMovement() {
	speed = (imgScale / 960.0f) * 0.03 * (150 / 100);
	if (moveRight) {
		if (!moving) {
			moving = true;
			targetMoveX = size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale) + 1;
			targetMoveY = size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale);
		}

	}
	if (moveLeft) {
		if (!moving) {
			moving = true;
			targetMoveX = size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale) - 1;
			targetMoveY = size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale);
		}
	}
	if (moveUp) {
		if (!moving) {
			moving = true;
			targetMoveX = size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale);
			targetMoveY = size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale) - 1;
		}
	}
	if (moveDown) {
		if (!moving) {
			moving = true;
			targetMoveX = size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale);
			targetMoveY = size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale) + 1;
		}
	}
	if (moving) {

		if (size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale) < targetMoveX &&
			size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale) == targetMoveY) {
			updateCameraPosition(speed, 0);
		}
		else if (size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale) > targetMoveX &&
			size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale) == targetMoveY) {
			updateCameraPosition(-speed, 0);
		}
		else if (size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale) == targetMoveX &&
			size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale) > targetMoveY) {
			updateCameraPosition(0, speed);
		}
		else if (size_t(xCameraPos / (imgScale / 960)) + size_t(960 / imgScale) == targetMoveX &&
			size_t(-yCameraPos / (imgScale / 540)) + size_t(540 / imgScale) < targetMoveY) {
			updateCameraPosition(0, -speed);
		}
		else {
			//setCameraPosition(targetMoveX * (imgScale / 960) - 1.0f, -((targetMoveY) * (imgScale / 540)) + 1.0f);
			moving = false;
		}
	}
}

