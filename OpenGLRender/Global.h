#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "ItemAtlas.h"
#include "Tile.h"
#include "Serializer.h"
#include "VertecesHandler.h"
#include "Object.h"
#include "Palette.h"
#include <set>
#include "Camera.h"
#include "World.h"
#include <atomic> 
#include "GUI.h"

// Include GLM
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

//#include <utility>

// MVP
extern glm::mat4 Projection;
extern glm::mat4 Model;
extern glm::mat4 View;

extern GLuint projection;
extern GLuint model;
extern GLuint view;


struct game_state {
	// this contains the state of your game, such as positions and velocities
	double xCameraPos = 0.0f;
	double yCameraPos = 0.0f;
	double velocity = 0.0f;
	double xGoal = 1.0f;
	double yGoal = -1.0f;
};

extern game_state current_state;
extern game_state previous_state;

struct SPosition
{
	float x, y;  //Position
	float u, v;  //Uv
};

// Store all the verteces
extern std::vector<VertecesHandler> verteces;

// Store all the objects
extern std::vector<Objects> objects;

// Store temp objects
extern std::vector<Objects> tiles;
extern std::vector<Objects> items;

extern std::vector<std::pair<std::string, int>> paths;
extern size_t sizeOfPaths;
extern size_t letterPath;

// Item atlas
extern ItemAtals itemAtlas;

// List of palettes
extern std::vector<Palette> palettes;

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

// Window size
extern float screenWidth;
extern float screenHeight;
extern int screenWidthPixels;
extern int screenHeightPixels;

//Update map
extern std::atomic<bool> updateMap;
extern std::atomic<bool> updateWorld;

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

extern bool changeFloor;
extern int newZ;

// Left mouse button held down
extern bool lbutton_down;

// Right mouse button held down
extern bool rbutton_down;

// FPS counter
extern size_t FPS;


//-----------------------------------
//			Bottom bar toggles
//-----------------------------------

// Erase Toggle button
extern bool eraseToggle;

// Destroy Toggle button
extern bool destroyToggle;


// Destroy tileDestroy toggle button
extern bool destroyTileToggle;
extern int destroyTileToggleID;


// Cut toggle button
extern bool cutToggle;
extern int cutToggleID;


// Copy toggle button
extern bool copyToggle;
extern int copyToggleID;

//-----------------------------------
//			DONE Bottom bar toggles
//-----------------------------------

// left panel state
extern int leftPanelState;

// Hovering tile selection palette dropdown
extern bool hoveringPaletteDropDown;
extern bool clickPaletteDropDown;
extern size_t startDropDown;

// Selected palette
extern int paletteID;
extern int palettePage;
extern int paletteMaxPage;
extern int paletteMaxY;
extern bool paletteLeftHover;
extern bool paletteRightHover;
extern bool paletteLeftPressed;
extern bool paletteRightPressed;

// Selected item from palette
extern std::pair<std::pair<int, bool>, std::pair<int, int>> selectedItemId;
extern bool isWithinTileArea;


	//----------------------------------------------------------
	//						start: Store items to be drawn
	//----------------------------------------------------------

	class ToDraw {
	public:
		ToDraw(int id, int x, int y, int z, int section): id(id), x(x), y(y), z(z), section(section) {};
		int getId() const { return id; };
		int getX() const { return x; };
		int getY() const { return y; };
		int getZ() const { return z; };
		int setX(int value) { x = value; };
		int setY(int value) { y = value; };
		int getSection() const { return section; };
		int getHash() const { return (x+(y*40)+z); };

	private:
		int id, x, y, z, section;
	};

	struct CustomCompare
	{
		bool operator()(const ToDraw& lhs, const ToDraw& rhs)
		{
			return lhs.getHash() < rhs.getHash();
		}
	};

	// Hash set for items to be draw
	extern std::set<ToDraw, CustomCompare > thingsToDraw;


	//----------------------------------------------------------
	//						end: Store items to be drawn
	//----------------------------------------------------------

// x, y coordinate for drawing
extern double xCoord, yCoord;

// Current Map section
extern int currentSection;

// Map section side length
extern const size_t SECTION_LENGTH;

// Map sections width
extern const size_t SECTIONS_WIDTH;

// Map floors
extern const size_t MAX_FLOORS;

//  Atomic lock for updating a specific floor after drawing new tiles
extern std::atomic<bool> holdFloor;
extern std::atomic<bool> updateMapFloor;
extern int updatingFloor;

// Atomic lock for copyBuffer access
extern std::atomic<bool> copyBufferLock;

// Tiles to be rendered
extern World world;
//extern std::vector<tile> tiles;

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
//			bottom bar
//-----------------------------------

// GUIPanel
extern GUIPanel bottomBar;

// bottom bar IDs
extern int bottomBarBasic;
extern int bottomBarLabel;
extern int bottomBarEraser;
extern int bottomBarEraserHover;
extern int bottomBarEraserPressed;
extern int bottomBarDestroyer;
extern int bottomBarDestroyerHover;
extern int bottomBarDestroyerPressed;
extern int bottomBarTileDestroyer;
extern int bottomBarTileDestroyerHover;
extern int bottomBarTileDestroyerPressed;
extern int bottomBarCut;
extern int bottomBarCutHover;
extern int bottomBarCutPressed;
extern int bottomBarCopy;
extern int bottomBarCopyHover;
extern int bottomBarCopyPressed;

// Show bottom bar
extern bool bottomBarShow;

// Copy buffer
extern std::vector<std::pair<ToDraw, tile*>> copyBuffer;

//-----------------------------------
//			DONE: bottom bar
//-----------------------------------

//-----------------------------------
//			item info window
//-----------------------------------

// GUIPanel
extern GUIPanel itemInfo;

extern bool updateItemInfo;

// item info panel tile
extern tile* itemInfoTile;

// item info page
extern int itemInfoCurrentPage;
extern int itemInfoMaxPage;
extern int itemInfoSubPage;

// item info first page: Tile
extern bool descriptionButtonHover;
extern bool descriptionButtonPressed;

// item info panel
extern int itemInfoPanelID;
extern int itemInfoTextSectionID;
extern int itemInfoRightArrowID;
extern int itemInfoRightArrowHoverID;
extern int itemInfoRightArrowPressedID;
extern int itemInfoLeftArrowID;
extern int itemInfoLeftArrowHoverID;
extern int itemInfoLeftArrowPressedID;
extern int itemInfoTextAreaID;
extern int itemInfoButtonID;
extern int itemInfoButtonHoverID;
extern int itemInfoButtonPressedID;

// Show item info window
extern bool itemInfoWindow;

//-----------------------------------
//			DONE item info window
//-----------------------------------

extern size_t size;

// Zoom level
extern float zoom;
extern bool haveZoomed;

extern float FOV;

extern float imgScale;

extern double xCameraPos;
extern double yCameraPos;

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

extern double horizontalSpeed;

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
