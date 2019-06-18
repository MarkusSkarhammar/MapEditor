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
#include "RendToText.h"

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

extern GLuint projection2;
extern GLuint model2;
extern GLuint view2;

// Texture array layer
extern GLuint gLayer;

// Render to texture stuff here
extern GLuint fbo_palette_modifier_left;
extern GLuint fbo_palette_modifier_left_texture;
extern GLuint textOffset2;

//Renderbuffer
extern GLuint rbo;

// TextOffset
extern glm::vec2 textOffsetValues;

// Vertices container
extern std::vector<Vertices*> verticesContainer;
extern std::vector<Vertices*> verticesContainer64xTiles;
extern std::vector<Vertices*> verticesContainer128xTiles;
extern std::vector<Vertices*> verticesContainerLetters;

// Render to texture containers
extern std::vector<RendToText*> renderToTextureContainer;


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
extern std::vector<VertecesHandler*> verteces;

// Store all the objects
extern std::vector<Objects> objects;

// Store temp objects
extern std::vector<Objects> tiles;
extern std::vector<Objects> items;
extern std::vector<Object*> itemsToDraw;

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
extern std::string xText, yText, zText;

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
extern int eraseToggleID;

// Destroy Toggle button
extern bool destroyToggle;
extern int destroyToggleID;


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
extern World worldTemp;
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
//			Left panel
//-----------------------------------

// lefPanel
extern GUIPanel leftPanel;


//-----------------------------------
//			DONE: Left panel
//-----------------------------------

//-----------------------------------
//			bottom bar
//-----------------------------------

// GUIPanel
extern GUIPanel bottomBar;

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

// Show item info window
extern bool itemInfoWindow;

//-----------------------------------
//			DONE item info window
//-----------------------------------

//-----------------------------------
//			Palette modifier window
//-----------------------------------

// GUIPanel
extern GUIPanel paletteModifier;

// palette modifier IDs
extern int paletteModifierPanel;
extern int paletteModifierSearchIcon;
extern int paletteModifierSearchIconHover;
extern int paletteModifierSearchIconPressed;
extern int paletteModifierDropDownUnPressed;
extern int paletteModifierDropDownPressed;
extern int paletteModifierDropDownElement;
extern int paletteModifierDropDownElementHover;
extern int paletteModifierEmptyTileMarker;

// palette y right tiles
extern int palette_Modifier_Right_page;
extern int palette_Modifier_Right_Offset;
extern double palette_Modifier_Right_Offset_Max;
extern std::string palette_Modifier_Right_Selected_Palette;

// palette y left tiles
extern int palette_Modifier_Left_page;
extern int palette_Modifier_Left_Offset;
extern double palette_Modifier_Left_Offset_Max;
extern std::string palette_Modifier_Left_Selected_Palette;

//-----------------------------------
//			DONE Palette modifier window
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

extern std::string getTextFromID(int ID, bool& x124);


static Palette& findByName(std::vector<Palette>& list, std::string name) {
	std::vector<Palette>::iterator it = std::find_if(list.begin(), list.end(), [name](Palette verteces) {
		return (verteces.getName() == name);
	});
	if (it != list.end())
		return (*it);
	else
		return *list.begin();
};

template <class T>
static T*& findByName(std::vector<T*>& list, std::string name) {
	typename std::vector<T*>::iterator it = std::find_if(list.begin(), list.end(), [name](T* verteces) {
		return (verteces->getName() == name);
	});
	if (it != list.end())
		return (*it);
	else
		return *list.begin();
}


#endif // !GLOBAL_VARIABLES_H
