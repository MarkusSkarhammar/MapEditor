#include "Global.h"

// MVP
glm::mat4 Projection(1);
glm::mat4 Model(1);
glm::mat4 View(1);

GLuint projection(0);
GLuint model(0);
GLuint view(0);

GLuint projection2(0);
GLuint model2(0);
GLuint view2(0);

//UBO
GLuint UBOCamera(0);
GLuint UBOTextureStuff(0);

// Shader programs
GLuint program(0);
GLuint program2(0);
GLuint outlineShader(0);

// Texture array
GLuint gTileArrayTexture(0);

// Texture array layer
GLuint gLayer(0);

//Instancing uniform
GLuint instancing(0);

//Instancing class
Instancing instanceDraw;

//Outline uniforms
GLuint outlineUniform(0);

//GUI uniform
GLuint GUI(0);

//Lightning shader stuff
GLuint ambientStrength(0);
float ambientStrengthValue(0.7f);

//Game time stuff
extern __int64 time_stamp_world_time(0);
extern int worldTimeHour(120);

// Render to texture stuff here
GLuint fbo_palette_modifier_left(0);
GLuint fbo_palette_modifier_left_texture(0);
GLuint textOffset2(0);

//Renderbuffer
GLuint rbo(0);

// TextOffset
glm::vec2 textOffsetValues(0);


game_state current_state;
game_state previous_state;

// close game
bool quit_game{ 0 };

//ObjectLibraries
std::vector<ObjectLibrary*> objLibraries;

//GUIPanels
std::vector<GUIPanel*> GUIPanels;

//LetterLibrary
LetterLibrary letterLibrary("Letters");

// Render to texture containers
std::vector<RendToText*> renderToTextureContainer = {};

// Outlined item
tile* outLinedTile = nullptr;
Item* outlinedItem = nullptr;
bool outlineItemBool(true);
bool outlineTileBool(false);

//Store Text To Rend objects
RendToTextObjLibrary rendToTextLibrary;

// Offsets for instancing
unsigned int instanceVBO;

// Store all the objects
// Store temp objects
std::vector<DrawObjects> tiles;
std::vector<DrawObjects> items;
std::vector<std::pair<int, DrawObject*>> itemsToDraw;


std::vector<std::string> paths = { 
std::string("Tiles_1"), std::string("Letters_"), std::string("Letters_White_"),  std::string("GUI_1"), std::string("GUI_2"),
std::string("Doodads_1"), std::string("Borders_1"), std::string("Monsters_1"),
std::string("Nature_1"), std::string("Items_1") };

size_t sizeOfPaths = 7;
size_t letterPath = 0;

// Item atlas
ItemAtals itemAtlas;

// List of palettes
std::vector<Palette> palettes;
Palette tempPalette("empty");
Palette tempPaletteLeft("empty");
//std::vector<size_t> pappap = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };

const std::string LEFFT_PANEL_DROP_DOWN_TEXT[] = { "Tiles", "Nature", "Monsters", "Humans", "Doodads", "Equipment", "Borders" };
const size_t SIZE_DROP_DOWN_TEXT = 7;


// The coordinates for a tile depending on where the mouse currently is and where the camera is positioned. 
int x(0), y(0), z(7);
std::string xText{"0"}, yText{ "0" }, zText{ "7" };

bool changeFloor(false);
int newZ(7);

// For drawing
float widthStart(0.0f);
float heightStart(0.0f);
double width(0.0);
double height(0.0);
float textWidth(0.0f);
float textHeight(0.0f);
float textHeightStart(0.0f);
float textWidthStart(0.0f);

// Window size
double screenWidth(0);
double screenHeight(0);
int screenWidthPixels(0);
int screenHeightPixels(0);

//Update map
std::atomic<bool> updateMap(false);
std::atomic<bool> updateWorld(false);
std::atomic<bool> changeWorldLock(false);

// Draw items/tiles
bool drawWalls(true);
bool drawTiles(true);

// The number of gui elements to be rendered ( the value represents where the respective element finds it's texture).
std::vector<size_t> GUIElements;

// The mouse coordinates.
double xPos(0.0f), yPos(0.0f);

// Draw at middle
bool drawMiddle(false);

// Draw at GUI
bool drawGUI(true);
bool drawLeftPanelGUI(true);

// Left mouse button held down
bool lbutton_down(false);

// Right mouse button held down
bool rbutton_down(false);

// FPS counter
size_t FPS(0);

// Erase Toggle button
bool eraseToggle(false);
int eraseToggleID(false);

// Destroy Toggle button
bool destroyToggle(false);
int destroyToggleID(false);

// Destroy tileDestroy toggle button
bool destroyTileToggle(false);
int destroyTileToggleID(false);

// Cut toggle button
bool cutToggle(false);
int cutToggleID(false);

// Copy toggle button
bool copyToggle(false);
int copyToggleID(false);

// left panel state
int leftPanelState(0);

// Hovering tile selection palette dropdown
bool hoveringPaletteDropDown(false);
bool clickPaletteDropDown(false);
size_t startDropDown(0);

// Selected palette
int paletteID(-1);
int palettePage(0);
int paletteMaxPage(0);
int paletteMaxY(15);
bool paletteLeftHover(false);
bool paletteRightHover(false);
bool paletteLeftPressed(false);
bool paletteRightPressed(false);

// Selected item from palette
PaletteItem selectedItemId(-1, false, -1, -1);
bool isWithinTileArea(false);

// Hash set for items to be draw
std::set<ToDraw, CustomCompare > thingsToDraw;

// x, y coordinate for drawing
double xCoord(0.0), yCoord(0.0);

// Current Map section
int currentSection(-1);
int middleSection(51);
int northSection(1);
int southSection(101);
int westSection(50);
int eastSection(52);

// Width of the sections to be loaded(Think of the sections as a square)
int sections_Loaded_Width(1);
std::unordered_map<int, int> sectionsActive;

// Map section side length
const size_t SECTION_LENGTH = 50;
// Map sections width
const size_t SECTIONS_WIDTH = 40;

// Map floors
const size_t MAX_FLOORS = 14;

//  Atomic lock for updating a specific floor after drawing new tiles
std::atomic<bool> holdFloor(false);
std::atomic<bool> updateMapFloor(false);
int updatingFloor(-1);

// Atomic lock for copyBuffer access
std::atomic<bool> copyBufferLock(false);


// Tiles to be rendered
World* world;
World* worldLoadTemp;
World* worldTemp;
std::string mapFileName = "Chunje";

// Render section below
bool sectionBelow(false);
// Render section right
bool sectionRight(false);
// Render section southEast
bool sectionSouthEast(false);

// Left control held
bool lControl(false);

// Left shift held
bool lShift(false);

// Brush size
size_t brush(0);

// Fow saving/loading
Serialize serializer;


//-----------------------------------
//			Left Panel
//-----------------------------------

// lefPanel
GUIPanel* leftPanel = new GUIPanel("leftPanel");

// left_panel tiles
bool leftPanelUpdate(false);
int left_panel_page;
int left_panel_maxPage;
double left_panel_Offset;
double left_panel_Offset_Max;
std::string left_panel_Selected_Palette;


//-----------------------------------
//			DONE: Left Panel
//-----------------------------------

//-----------------------------------
//			bottom bar
//-----------------------------------

// GUIPanel
GUIPanel* bottomBar = new GUIPanel("bottomBar");

// Copy buffer
std::vector<std::pair<ToDraw, tile*>> copyBuffer;


//-----------------------------------
//			DONE: bottom bar
//-----------------------------------

//-----------------------------------
//			Tile info window
//-----------------------------------

// GUIPanel
GUIPanel* itemInfo = new GUIPanel("itemInfo");

bool updateItemInfo(false);

// item info panel tile
tile* itemInfoTile(nullptr);

// item info page
int itemInfoCurrentPage(0);
int itemInfoMaxPage(0);
int itemInfoSubPage(0);


// Show tile info window
bool itemInfoWindow(false);

//-----------------------------------
//			DONE Tile info window
//-----------------------------------

//-----------------------------------
//			Palette modifier window
//-----------------------------------

// GUIPanel
GUIPanel* paletteModifier = new GUIPanel("paletteModifier");

// palette modifier IDs
int paletteModifierPanel(0);
int paletteModifierSearchIcon(0);
int paletteModifierSearchIconHover(0);
int paletteModifierSearchIconPressed(0);
int paletteModifierDropDownUnPressed(0);
int paletteModifierDropDownPressed(0);
int paletteModifierDropDownElement(0);
int paletteModifierDropDownElementHover(0);
int paletteModifierEmptyTileMarker(0);

// palette right tiles
int palette_Modifier_Right_page(0);
int palette_Modifier_Right_maxPage(0);
double palette_Modifier_Right_Offset(0);
double palette_Modifier_Right_Offset_Max(0);
std::string palette_Modifier_Right_Selected_Palette = "";
PaletteItem palette_Modifier_Right_Selected_Tile;
bool palette_Modifier_Right_First{ true };
int palette_Modifier_Right_Shift_Start(-1);
bool palette_Modifier_Right_Shift_Bool(0);
std::unordered_map<int, int> palette_Modifier_Right_Clicked;

// palette left tiles
int palette_Modifier_Left_page(0);
int palette_Modifier_Left_maxPage(0);
double palette_Modifier_Left_Offset(0);
double palette_Modifier_Left_Offset_Max(0);
std::string palette_Modifier_Left_Selected_Palette = "";
int palette_Modifier_Left_Shift_Start(-1);
bool palette_Modifier_Left_Shift_Bool(0);
std::unordered_map<int, int> palette_Modifier_Left_Clicked;

// Changes
std::vector<PaletteChange> palette_Modifier_Undo = {};
std::vector<PaletteChange> palette_Modifier_Redo = {};

//-----------------------------------
//			DONE Palette modifier window
//-----------------------------------

//-----------------------------------
//			Vertices creation window
//-----------------------------------

// GUIPanel
GUIPanel* verticesCreation = new GUIPanel("verticesCreation");



// Update bool
bool verticesCreationUpdateSelectionArea{0};
double verticesCreationSelectionAreaOffset{0};
double verticesCreationSelectionAreaOffsetMax{0};
int verticesCreationSelectionAreaPage{0};
double verticesCreationSelectionAreaPageSize{2000};
bool verticesCreationSelectionAreaPageChange{0};
bool verticesCreationFirstShow{1};

Object* selectedObject = nullptr;
double verticesCreationPreviewZoom{1.0};
double verticesCreationPreviewClickX{ 0. };
double verticesCreationPreviewClickY{ 0. };

//-----------------------------------
//			DONE Palette modifier window
//-----------------------------------

size_t size = 0;

// Zoom level
double zoomWorld = 1.0;
bool haveZoomed = false;

float FOV = 90;

float imgScale = 64.0f;

double xCameraPos = 1.0;
double yCameraPos = -1.0;

bool dKey(0.0f);

bool moving(false);
bool movingLeft(false);
bool movingRight(false);
bool movingUp(false);
bool movingDown(false);
bool moveLeft(false);
bool moveRight(false);
bool moveUp(false);
bool moveDown(false);
float nextRight = 0;

double horizontalSpeed(0.0f);

//-----------------------------------
//			Character stuff
//-----------------------------------

float speed(200);
size_t lastCheckedX(0);
size_t lastCheckedY(0);
size_t targetMoveX(10000);
size_t targetMoveY(10000);

//-----------------------------------
//			DONE Character stuff
//-----------------------------------

std::string getTextFromID(int ID, bool & x124)
{
	std::string s = "";
	if (ID / 1024 == 0) {
		s = "Tiles_1024_" + std::to_string(ID);
	}
	else if (ID / 1024 == 1) {
		s = "Items_1024_" + std::to_string(ID % 1024);
	}
	else if (ID / 1024 == 2) {
		s = "Doodads_1024_" + std::to_string(ID % 1024);
		x124 = true;
	}
	else if (ID / 1024 == 3) {
		s = "Borders_1024_" + std::to_string(ID % 1024);
	}
	else if (ID / 1024 == 5) {
		s = "Nature_1024_" + std::to_string(ID % 1024);
	}
	return s;
}
