#include "Global.h"

// MVP
glm::mat4 Projection(1);
glm::mat4 Model(1);
glm::mat4 View(1);

GLuint projection(0);
GLuint model(0);
GLuint view(0);

// TextOffset
glm::vec2 textOffsetValues(0);


game_state current_state;
game_state previous_state;

// Store all the verteces
std::vector<VertecesHandler> verteces;

// Store all the objects
std::vector<Objects> objects;

// Store all the objects
// Store temp objects
std::vector<Objects> tiles;
std::vector<Objects> items;
std::vector<Object*> itemsToDraw;

std::vector<std::pair<std::string, int>> paths = { 
std::pair<std::string, int>("Tiles_1024", 19), std::pair<std::string, int>("Letters_", 256),  std::pair<std::string, int>("GUI_1", -1),
std::pair<std::string, int>("Doodads_1024", 28), std::pair<std::string, int>("Borders_1024", 32), std::pair<std::string, int>("Monsters_1024", 0),
std::pair<std::string, int>("Nature_1024", 4), std::pair<std::string, int>("Items_1024", 10) };

size_t sizeOfPaths = 7;
size_t letterPath = 0;

// Item atlas
ItemAtals itemAtlas;

// List of palettes
std::vector<Palette> palettes;
//std::vector<size_t> pappap = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };

const float OFFSET_PER_CHARACTER[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-15
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16-31
	6.0f, 0, 0, 0, 0, 0, 0, 0, 0, 8.0, 0, 0, 0, 16.0f, 0, 0, // 32-47
	11.0f, 8.0f, 11.0f, 8, 8, 8, 8, 8, 8, 7.5, 6.0f, 0, 0, 0, 0, 0, // 48-63
	5, 5, 12.0f, 12.0f, 12.0f, 11.0f, 5, 5, 12.0f, 12, 5, 5, 12.0f, 12.0f, 12, 5, // 64-79
	0, 0, 0, 0, 13.0f, 0, 0.0f, 0.0f, 14.0f, 14.0f, 12.0f, 0, 0, 0, 0, 0, // 80-95
	0, 9.0f, 7.0f, 7.0f, 9.0f, 8.0f, 8.0f, 0, 0, 5.0f, 10.0f, 12.0f, 5.0f, 14.0f, 9.0f, 9.0f, //96-111
	10.0f, 10.0f, 7.0f, 9.0f, 6.0f, 10.0f, 0, 14.0f, 0, 7.0f, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const std::string LEFFT_PANEL_DROP_DOWN_TEXT[] = { "Tiles", "Nature", "Monsters", "Humans", "Doodads", "Equipment", "Borders" };
const size_t SIZE_DROP_DOWN_TEXT = 7;


// The coordinates for a tile depending on where the mouse currently is and where the camera is positioned. 
size_t x(0), y(0), z(7);

bool changeFloor(false);
int newZ(7);

// For drawing
float widthStart(0.0f);
float heightStart(0.0f);
float width(0.0f);
float height(0.0f);
float textWidth(0.0f);
float textHeight(0.0f);
float textHeightStart(0.0f);
float textWidthStart(0.0f);

// Window size
float screenWidth(0);
float screenHeight(0);
int screenWidthPixels(0);
int screenHeightPixels(0);

//Update map
std::atomic<bool> updateMap(false);
std::atomic<bool> updateWorld(false);

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
std::pair<std::pair<int, bool>, std::pair<int, int>> selectedItemId(std::pair<int, int>(-1, false), std::pair<int, int>(-1, -1));
bool isWithinTileArea(false);

// Hash set for items to be draw
std::set<ToDraw, CustomCompare > thingsToDraw;

// x, y coordinate for drawing
double xCoord(0.0), yCoord(0.0);

// Current Map section
int currentSection(0);

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
World world("Chunje");
World worldTemp("Temp");

// Render section below
bool sectionBelow(false);
// Render section right
bool sectionRight(false);
// Render section southEast
bool sectionSouthEast(false);

// Left control held
bool lControl(false);

// Brush size
size_t brush(0);

// Fow saving/loading
Serialize serializer;


//-----------------------------------
//			Left Panel
//-----------------------------------

// lefPanel
GUIPanel leftPanel("leftPanel");

// bottom bar IDs
int fillerPixel;
int displayBar;
int displayBarButton;
int displayBarButtonHover;
int displayBarButtonPressed;
int tileAreaTop;
int tileAreaMiddle;
int tileAreaBottom;
int tileAreaLeftButton;
int tileAreaLeftButtonHover;
int tileAreaLeftButtonPressed;
int tileAreaRightButton;
int tileAreaRightButtonHover;
int tileAreaRightButtonPressed;
int leftPanelDropDownMiddleSection;
int leftPanelDropDownBottomSection;
int leftPanelDropDownHover;
int leftPanelYellowSquareSmall;
int leftPanelRedSquareSmall;
int leftPanelYellowSquareBig;
int leftPanelRedSquareBig;

// Show left panel
bool leftPanelShow(true);

//-----------------------------------
//			DONE: Left Panel
//-----------------------------------

//-----------------------------------
//			bottom bar
//-----------------------------------

// GUIPanel
GUIPanel bottomBar("bottomBar");

// bottom bar IDs
int bottomBarBasic(0);
int bottomBarLabel(0);
int bottomBarEraser(0);
int bottomBarEraserHover(0);
int bottomBarEraserPressed(0);
int bottomBarDestroyer(0);
int bottomBarDestroyerHover(0);
int bottomBarDestroyerPressed(0);
int bottomBarTileDestroyer(0);
int bottomBarTileDestroyerHover(0);
int bottomBarTileDestroyerPressed(0);
int bottomBarCut(0);
int bottomBarCutHover(0);
int bottomBarCutPressed(0);
int bottomBarCopy(0);
int bottomBarCopyHover(0);
int bottomBarCopyPressed(0);

// Show bottom bar
bool bottomBarShow(true);

// Copy buffer
std::vector<std::pair<ToDraw, tile*>> copyBuffer;


//-----------------------------------
//			DONE: bottom bar
//-----------------------------------

//-----------------------------------
//			Tile info window
//-----------------------------------

// GUIPanel
GUIPanel itemInfo("itemInfo");

bool updateItemInfo(false);

// item info panel tile
tile* itemInfoTile(nullptr);

// item info page
int itemInfoCurrentPage(0);
int itemInfoMaxPage(0);
int itemInfoSubPage(0);

// item info first page: Tile
bool descriptionButtonHover(false);
bool descriptionButtonPressed(false);

// item info panel IDs
int itemInfoPanelID(0);
int itemInfoTextSectionID(0);
int itemInfoRightArrowID(0);
int itemInfoRightArrowHoverID(0);
int itemInfoRightArrowPressedID(0);
int itemInfoLeftArrowID(0);
int itemInfoLeftArrowHoverID(0);
int itemInfoLeftArrowPressedID(0);
int itemInfoTextAreaID(0);
int itemInfoButtonID(0);
int itemInfoButtonHoverID(0);
int itemInfoButtonPressedID(0);


// Show tile info window
bool itemInfoWindow(false);

//-----------------------------------
//			DONE Tile info window
//-----------------------------------

//-----------------------------------
//			Palette modifier window
//-----------------------------------

// GUIPanel
GUIPanel paletteModifier("paletteModifier");

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

//-----------------------------------
//			DONE Palette modifier window
//-----------------------------------

size_t size = 0;

// Zoom level
float zoom = 1;
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
