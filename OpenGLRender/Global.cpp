#include "Global.h"

const std::string paths[] = { "Tiles_1024", "Letters",  "LeftPanel", "Doodads_1024", "Borders_1024", "Monsters_1024", "Nature_1024" };

size_t sizeOfPaths = 7;
size_t letterPath = 0;

ItemAtals itemAtlas;

const float OFFSET_PER_CHARACTER[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	8, 9.5f, 8, 8, 8, 8, 8, 8, 8, 7.5, 0, 0, 0, 0, 0, 0,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 12, 5, 5, 5, 5, 5, 5,
	0, 0, 0, 0, 5.0f, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 7, 7.0f, 7.0f, 7.0f, 8.0f, 7.0f, 0, 0, 12.0f, 10.0f, 0, 12.0f, 3.0f, 7.0f, 7.0f,
	7.0f, 7.0f, 9.1f, 8.0f, 8.75f, 5.0f, 0, 0, 0, 7.0f, 0, 0, 0, 0, 0, 0,
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

// For drawing
float widthStart;
float heightStart;
float width;
float height;
float textWidth;
float textHeight;
float textHeightStart;
float textWidthStart;

// Draw items/tiles
bool drawWalls(true);
bool drawTiles(true);

// The number of gui elements to be rendered ( the value represents where the respective element finds it's texture).
std::vector<size_t> GUIElements;

// The mouse coordinates.
double xPos, yPos;

// Check to see if the mouse is within the area where the tiles are rendered.
bool isWithinTileArea = false;

// Which scroll bar button to be rendered when the mouse hovers it.
size_t scrollBarHover(0);

// Where to draw a yellow border around an item that the cursor is currently hovering over.
size_t contentSelectionHover[2] = { 1000, 1000 };

// Where to draw a red border if an item was selected.
size_t contentSelectionSelected[2] = { 1000, 1000 };

// The selected item's position
size_t selectedID(100000);

// The amount of items to be rendered on the selection area.
size_t max_selected(1000);

// Make sure to only find the items from xml when the selection area is to be changed
bool getItemsToRenderOnSelectionArea;

// Where each of the items to be render on the selection area find their respective texture
std::vector<size_t> pos;

// Current selected item
std::vector<Item> item;

// Tile to be rendered as a preview before actual placement.
tile preview(0, 0, 0, 1);

// Draw at middle
bool drawMiddle(false);

// Draw at GUI
bool drawGUI(true);
bool drawLeftPanelGUI(true);

// Make sure to only add an item once
std::vector<std::pair<size_t, std::pair<size_t, size_t>>> addItem;

// Left mouse button held down
bool lbutton_down;

// Right mouse button held down
bool rbutton_down;

// FPS counter
size_t FPS;

// Make sure to only update the tiles when necessary.
bool updateTiles = false;
std::vector<float> tilesVerteces;
std::vector<float> tilesVertecesRight;
std::vector<float> tilesVertecesColor;
std::vector<std::vector<float>> tilesVerteces2;
std::vector<size_t> amountOfTilesToDraw;
//std::vector<std::vector<size_t>> amountOfTilesToDrawLevels;
std::vector<std::vector<size_t>> amountOfTilesToDrawLevelsBuffer;
std::vector<std::vector<size_t>> amountOfTilesToDrawLevelsDraw;

// Make sure to update the items on a tile only when necessary.
bool updateItems = false;
std::vector<float> itemsVertecesBuffer;
std::vector<float> itemsVertecesDraw;
std::vector<size_t> amountOfItemsToDraw;
//std::vector<std::vector<size_t>> amountOfItemsToDrawLevels;
std::vector<std::vector<size_t>> amountOfItemsToDrawLevelsBuffer;
std::vector<std::vector<size_t>> amountOfItemsToDrawLevelsDraw;

// Total drawn tiles/items/entities
size_t amountTilesBeforeGUI(0);
size_t amountItemsBeforeGUI(0);
size_t amountBeforeGUI(0);
bool updateAll(false);

// Make sure to only update the GUI when necessary.
bool updateGUI = true;
std::vector<float> GUIVerteces;

// Erase Toggle button
bool eraseToggle(false);

// Destroy Toggle button
bool destroyToggle(false);

// Hovering over Vertical scroll bar
bool verticalScrollbar(false);
bool verticalScrollBarHover(false);
double verticalScrollBarY(35);

// Hovering over Horizontal scroll bar
bool horizontalScrollbar(false);
bool horizontalScrollBarHover(false);
double horizontalScrollBarX(35);

// x, y coordinate for drawing
double xCoord(0.0), yCoord(0.0);

// Current Map section
size_t currentSection(0);

// Map section side length
const size_t SECTION_LENGTH = 50;

// First element determines if the drop down should be rendered
// Second element determines how many slots to render for the drop down list (must match size of LEFFT_PANEL_DROP_DOWN_TEXT).
// Third element determines which slot to be rendered with blue background on the drop down list.
// Fourth element determines which text to be rendered as the chosen category for the drop down list (> 0 && < size of LEFFT_PANEL_DROP_DOWN_TEXT)
size_t GUILeftPanelElement[] = { 0, SIZE_DROP_DOWN_TEXT, 1000, 1000 };


// Tiles to be rendered
World world;
std::vector<tile> tiles = world.getFloor(7).getSection(0);
// Render section below
bool sectionBelow(false);
// Render section right
bool sectionRight(false);
// Render section southEast
bool sectionSouthEast(false);

// Hold the latest made changes
std::vector<tile> commands;
const size_t maxCommands = 200;
int atCommand(0);

// Left control held
bool lControl(false);

// Brush size
size_t brush(0);

// Fow saving/loading
Serialize serializer;

//-----------------------------------
//			Tile info window
//-----------------------------------

// Show tile info window
bool tileInfoWindow(false);

// Selected to to show info about
tile* tileInfo(nullptr);

// Change article button logic
bool changeArticle(false);
std::string article = "";

// Verteces
std::vector<float> tileWindowVerteces;

// Texture positions
std::vector<size_t> tileWindowElements;

// Update only when necessary
bool tileWindowUpdate(false);

// Text verteces
std::vector<float> tileWindowTextVerteces;

// Text elements
size_t tileWindowText(0);

// Current selected item
int selectedItem = -1;

// Change item down
bool changeItemLeft(false);

// Change item up
bool changeItemRight(false);

//-----------------------------------
//			DONE Tile info window
//-----------------------------------

size_t size = 0;

// Zoom level
float zoom = 90;
bool haveZoomed = false;

float imgScale = 64.0f;

float xCameraPos = 0.0f;
float yCameraPos = 0.0f;

bool dKey;

bool moving;
bool movingLeft(false);
bool movingRight(false);
bool movingUp(false);
bool movingDown(false);
bool moveLeft(false);
bool moveRight(false);
bool moveUp(false);
bool moveDown(false);
float nextRight = 0;

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


bool isDoubleSize(size_t id) {
	std::vector<size_t> doubleScale{ 2048, 2049, 2050, 2051, 2052, 2053, 2054 ,2056 ,2057 ,2058 ,2059 ,2060 ,2061 ,2062 ,2063 ,2064 ,2066 ,2065 ,2066 ,2067
		,2068 ,2069 ,2070 ,2071, 2072 ,2073 , 2074, 5120};
	if (std::find(doubleScale.begin(), doubleScale.end(), id) != doubleScale.end()) return true;
	return false;
}