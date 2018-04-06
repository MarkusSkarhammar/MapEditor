#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "ItemAtlas.h"
#include "Tile.h"
#include "World.h"
#include "Serializer.h"

struct SVertex2D
{
	float x, y;  //Position
	float u, v;  //Uv
};

extern const std::string paths[];
extern size_t sizeOfPaths;
extern size_t letterPath;

extern ItemAtals itemAtlas;

extern const float OFFSET_PER_CHARACTER[256];

extern const std::string LEFFT_PANEL_DROP_DOWN_TEXT[];
extern const size_t SIZE_DROP_DOWN_TEXT;

// For drawing
extern float widthStart;
extern float heightStart;
extern float width;
extern float height;
extern float textWidth;
extern float textHeight;
extern float textHeightStart;
extern float textWidthStart;

// Draw items/tiles
extern bool drawWalls;
extern bool drawTiles;

// Draw at middle
extern bool drawMiddle;

// Draw at GUI
extern bool drawGUI;
extern bool drawLeftPanelGUI;

// The number of gui elements to be rendered ( the value represents where the respective element finds it's texture).
extern std::vector<size_t> GUIElements;

// The mouse coordinates.
extern double xPos, yPos;

// The coordinates for a tile depending on where the mouse currently is or where it last were. 
extern size_t x, y, z;

// Check to see if the mouse is within the area where the tiles are rendered.
extern bool isWithinTileArea;

// Which scroll bar button to be rendered when the mouse hovers it.
extern size_t scrollBarHover;

// Where to draw a yellow border around an item that the cursor is currently hovering over.
extern size_t contentSelectionHover[];

// Where to draw a red border if an item was selected.
extern size_t contentSelectionSelected[];

// The selected item's position
extern size_t selectedID;

// The amount of items to be rendered on the selection area.
extern size_t max_selected;

// Make sure to only find the items from xml when the selection area is to be changed
extern bool getItemsToRenderOnSelectionArea;

// Where each of the items to be render on the selection area find their respective texture
extern std::vector<size_t> pos;

// Current selected item
extern std::vector<Item> item;

// Tile to be rendered as a preview before actual placement.
extern tile preview;

// Make sure to only add an item once
extern std::vector<std::pair<size_t, std::pair<size_t, size_t>>> addItem;

// Left mouse button held down
extern bool lbutton_down;

// Right mouse button held down
extern bool rbutton_down;

// FPS counter
extern size_t FPS;

// Make sure to only update the tiles when necessary.
extern bool updateTiles;
extern std::vector<float> tilesVerteces;
extern std::vector<float> tilesVertecesRight;
extern std::vector<float> tilesVertecesColor;
extern std::vector<std::vector<float>> tilesVerteces2;
extern std::vector<size_t> amountOfTilesToDraw;
extern std::vector<std::vector<size_t>> amountOfTilesToDrawLevelsBuffer;
extern std::vector<std::vector<size_t>> amountOfTilesToDrawLevelsDraw;

// Make sure to update the items on a tile only when necessary.
extern bool updateItems;
extern std::vector<float> itemsVertecesBuffer;
extern std::vector<float> itemsVertecesDraw;
extern std::vector<size_t> amountOfItemsToDraw;
extern std::vector<std::vector<size_t>> amountOfItemsToDrawLevelsBuffer;
extern std::vector<std::vector<size_t>> amountOfItemsToDrawLevelsDraw;

// Total drawn tiles/items/entities
extern size_t amountTilesBeforeGUI;
extern size_t amountItemsBeforeGUI;
extern size_t amountBeforeGUI;
extern bool updateAll;

// Make sure to only update the GUI when necessary.
extern bool updateGUI;
extern std::vector<float> GUIVerteces;

// Erase Toggle button
extern bool eraseToggle;

// Destroy Toggle button
extern bool destroyToggle;

// Hovering over Vertical scroll bar
extern bool verticalScrollbar;
extern bool verticalScrollBarHover;
extern double verticalScrollBarY;

// Hovering over Horizontal scroll bar
extern bool horizontalScrollbar;
extern bool horizontalScrollBarHover;
extern double horizontalScrollBarX;

// x, y coordinate for drawing
extern double xCoord, yCoord;

// Current Map section
extern size_t currentSection;

// Map section side length
extern const size_t SECTION_LENGTH;

// First element determines if the drop down should be rendered
// Second element determines how many slots to render for the drop down list (must match size of LEFFT_PANEL_DROP_DOWN_TEXT).
// Third element determines which slot to be rendered with blue background on the drop down list.
// Fourth element determines which text to be rendered as the chosen category for the drop down list (> 0 && < size of LEFFT_PANEL_DROP_DOWN_TEXT)
extern size_t GUILeftPanelElement[];


// Tiles to be rendered
extern World world;
extern std::vector<tile> tiles;
// Render section below
extern bool sectionBelow;
// Render section right
extern bool sectionRight;
// Render section southEast
extern bool sectionSouthEast;

// Hold the latest made changes
extern std::vector<tile> commands;
extern const size_t maxCommands;
extern int atCommand;

// Left control held
extern bool lControl;

// Brush size
extern size_t brush;

// Fow saving/loading
extern Serialize serializer;

//-----------------------------------
//			Tile info window
//-----------------------------------

// Show tile info window
extern bool tileInfoWindow;

// Selected to to show info about
extern tile* tileInfo;

// Change article button logic
extern bool changeArticle;
extern std::string article;

// Verteces
extern std::vector<float> tileWindowVerteces;

// Texture positions
extern std::vector<size_t> tileWindowElements;

// Update only when necessary
extern bool tileWindowUpdate;

// Text verteces
extern std::vector<float> tileWindowTextVerteces;

// Text elements
extern size_t tileWindowText;

// Current selected item
extern int selectedItem;

// Change item down
extern bool changeItemLeft;

// Change item up
extern bool changeItemRight;

//-----------------------------------
//			DONE Tile info window
//-----------------------------------

extern size_t size;

// Zoom level
extern float zoom;
extern bool haveZoomed;

bool isDoubleSize(size_t id);

extern float imgScale;

extern float xCameraPos;
extern float yCameraPos;

extern bool dKey;

extern bool moving;
extern bool movingLeft;
extern bool movingRight;
extern bool movingUp;
extern bool movingDown;
extern bool moveLeft;
extern bool moveRight;
extern bool moveUp;
extern bool moveDown;
extern float nextRight;

//-----------------------------------
//			Character stuff
//-----------------------------------

extern float speed;
extern size_t lastCheckedX;
extern size_t lastCheckedY;
extern size_t targetMoveX;
extern size_t targetMoveY;

//-----------------------------------
//			DONE Character stuff
//-----------------------------------



#endif // !GLOBAL_VARIABLES_H
