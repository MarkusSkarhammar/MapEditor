#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

struct game_state;

#include "ItemAtlas.h"
#include "Tile.h"
#include "Serializer.h"
#include "Vertices.h"
#include "DrawObject.h"
#include "Palette.h"
#include <set>
#include "Camera.h"
#include "World.h"
#include <atomic> 
#include "GUI.h"
#include "RendToText.h"
#include "ObjectLibrary.h"
#include "Render.h"

// Include GLM
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <map>

//#include <utility>

//STATIC VALUES
static int MOUSE_RIGHT_CLICK = 1, MOUSE_LEFT_CLICK = 0;
static int MOUSE_RELEASE = 0, MOUSE_PRESS = 1;

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

//UBO
extern GLuint UBOCamera;
extern GLuint UBOTextureStuff;

// Shader programs
extern GLuint program;
extern GLuint program2;
extern GLuint outlineShader;

// Texture array
extern GLuint gTileArrayTexture;

// Texture array layer
extern GLuint gLayer;

//Instancing uniform
extern GLuint instancing;

//Instancing class
extern Instancing instanceDraw;

//Outline uniforms
extern GLuint outlineUniform;

//GUI uniform
extern GLuint GUI;

//Lightning shader stuff
extern GLuint ambientStrength;
extern float ambientStrengthValue;

//Game time stuff
extern __int64 time_stamp_world_time;
extern int worldTimeHour;

// Render to texture stuff here
extern GLuint fbo_palette_modifier_left;
extern GLuint fbo_palette_modifier_left_texture;
extern GLuint textOffset2;

//Renderbuffer
extern GLuint rbo;

// TextOffset
extern glm::vec2 textOffsetValues;

//ObjectLibraries
extern std::vector<ObjectLibrary*> objLibraries;

//GUIPanels
extern std::vector<GUIPanel*> GUIPanels;

//LetterLibrary
extern LetterLibrary letterLibrary;

// Render to texture containers
extern std::vector<RendToText*> renderToTextureContainer;

// Outlined item
extern tile* outLinedTile;
extern Item* outlinedItem;
extern bool outlineItemBool;
extern bool outlineTileBool;

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

// close game
extern bool quit_game;

struct SPosition
{
	float x, y;  //Position
	float u, v;  //Uv
};

//Store Text To Rend objects
extern RendToTextObjLibrary rendToTextLibrary;

// Offsets for instancing
extern unsigned int instanceVBO;

// Store temp objects
extern std::vector<DrawObjects> tiles;
extern std::vector<DrawObjects> items;
extern std::vector<std::pair<int, DrawObject*>> itemsToDraw;

extern std::vector<std::string> paths;
extern size_t sizeOfPaths;
extern size_t letterPath;

// Item atlas
extern ItemAtals itemAtlas;

// List of palettes
extern std::vector<Palette> palettes;
extern Palette tempPalette;
extern Palette tempPaletteLeft;

extern const std::string LEFFT_PANEL_DROP_DOWN_TEXT[];
extern const size_t SIZE_DROP_DOWN_TEXT;

// For drawing
extern float widthStart;
extern float heightStart;
extern double width;
extern double height;
extern float textWidth;
extern float textHeight;
extern float textHeightStart;
extern float textWidthStart;

// Window size
extern double screenWidth;
extern double screenHeight;
extern int screenWidthPixels;
extern int screenHeightPixels;

//Update map
extern std::atomic<bool> updateMap;
extern std::atomic<bool> updateWorld;
extern std::atomic<bool> changeWorldLock;

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
extern int x, y, z;
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
extern PaletteItem selectedItemId;
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
		bool operator()(const ToDraw& lhs, const ToDraw& rhs) const
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
extern int middleSection;
extern int northSection;
extern int southSection;
extern int westSection;
extern int eastSection;

// Width of the sections to be loaded(Think of the sections as a square)
extern int sections_Loaded_Width;
extern std::unordered_map<int, int> sectionsActive;

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
extern World* world;
extern World* worldLoadTemp;
extern World* worldTemp;
extern std::string mapFileName;

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

// Left shift held
extern bool lShift;

// Brush size
extern size_t brush;

// Fow saving/loading
extern Serialize serializer;

//-----------------------------------
//			Left panel
//-----------------------------------

// lefPanel
extern GUIPanel* leftPanel;

// left_panel tiles
extern bool leftPanelUpdate;
extern int left_panel_page;
extern int left_panel_maxPage;
extern double left_panel_Offset;
extern double left_panel_Offset_Max;
extern std::string left_panel_Selected_Palette;


//-----------------------------------
//			DONE: Left panel
//-----------------------------------

//-----------------------------------
//			bottom bar
//-----------------------------------

// GUIPanel
extern GUIPanel* bottomBar;

// Copy buffer
extern std::vector<std::pair<ToDraw, tile*>> copyBuffer;

//-----------------------------------
//			DONE: bottom bar
//-----------------------------------

//-----------------------------------
//			item info window
//-----------------------------------

// GUIPanel
extern GUIPanel* itemInfo;

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
extern GUIPanel* paletteModifier;

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
extern int palette_Modifier_Right_maxPage;
extern double palette_Modifier_Right_Offset;
extern double palette_Modifier_Right_Offset_Max;
extern std::string palette_Modifier_Right_Selected_Palette;
extern PaletteItem palette_Modifier_Right_Selected_Tile;
extern bool palette_Modifier_Right_First;
extern int palette_Modifier_Right_Shift_Start;
extern bool palette_Modifier_Right_Shift_Bool;
extern std::unordered_map<int, int> palette_Modifier_Right_Clicked;

// palette y left tiles
extern int palette_Modifier_Left_page;
extern int palette_Modifier_Left_maxPage;
extern double palette_Modifier_Left_Offset;
extern double palette_Modifier_Left_Offset_Max;
extern std::string palette_Modifier_Left_Selected_Palette;
extern int palette_Modifier_Left_Shift_Start;
extern bool palette_Modifier_Left_Shift_Bool;
extern std::unordered_map<int, int> palette_Modifier_Left_Clicked;

// Changes
extern std::vector<PaletteChange> palette_Modifier_Undo;
extern std::vector<PaletteChange> palette_Modifier_Redo;

//-----------------------------------
//			DONE Palette modifier window
//-----------------------------------

//-----------------------------------
//			Vertices creation window
//-----------------------------------

// GUIPanel
extern GUIPanel* verticesCreation;


// Update bool
extern bool verticesCreationUpdateSelectionArea;
extern double verticesCreationSelectionAreaOffset;
extern double verticesCreationSelectionAreaOffsetMax;
extern int verticesCreationSelectionAreaPage;
extern double verticesCreationSelectionAreaPageSize;
extern bool verticesCreationSelectionAreaPageChange;
extern bool verticesCreationFirstShow;

extern Object* selectedObject, * tempSelectedObject;
extern double verticesCreationPreviewZoom;
extern double verticesCreationPreviewClickX;
extern double verticesCreationPreviewClickY;

//-----------------------------------
//			DONE Palette modifier window
//-----------------------------------

extern size_t size;

// Zoom level
extern double zoomWorld;
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

template <class T>
static T& findByName(std::vector<T>& list, std::string name) {
	typename std::vector<T*>::iterator it = std::find_if(list.begin(), list.end(), [name](T verteces) {
		return (verteces.getName() == name);
		});
	if (it != list.end())
		return (*it);
	else
		return *list.begin();
}

static int findByName(std::vector<std::pair<std::string, int>>& list, std::string name) {
	typename std::vector<std::pair<std::string, int>>::iterator it = std::find_if(list.begin(), list.end(), [name](std::pair<std::string, int> verteces) {
		return (verteces.first == name);
		});
	if (it != list.end())
		return (*it).second;
	else
		return -1;
}


static int findByName(std::vector<std::string>& list, std::string name) {
	int pos = 0;
	typename std::vector<std::string>::iterator it = std::find_if(list.begin(), list.end(), [name, &pos](std::string element) {
		if (!(element == name)) pos++;
		return (element == name);
		});
	if (it != list.end())
		return pos;
	else
		return -1;
}

static std::pair<GLuint, int> getLettersByName(std::string name) {
	auto it = std::find_if(objLibraries.begin(), objLibraries.end(), [name](ObjectLibrary* element) {
		return (element->getName() == name);
		});
	if (it != objLibraries.end())
		return std::pair<GLuint, int>(-1, -1);
	else
		return std::pair<GLuint, int>(-1, -1);
}

static bool check_Sub_String_Case_Insensitive(std::string& string, std::string& subString) {
	for (auto& c : subString) {
		c = (char)tolower(c);
	}
	for (auto& c : string) {
		c = (char)tolower(c);
	}
	int pos = 0;
	if (subString.size() > 0 && string.size() >= subString.size()) {
		auto it = string.begin();
		while (it + (subString.size() - 1) != string.end()) {
			pos = 0;
			for (auto c : subString) {
				if (*(it + pos) != c)
					break;
				pos++;
			}
			if (pos == subString.size())
				return true;
			it++;
		}
	}
	return false;
}

static void Remove_SubString_From_String(std::string& str, std::string& subStr) {
	int posS = std::string::npos;;
	while ((posS = str.find(subStr)) != std::string::npos)
	{
		str.erase(posS, subStr.length());
	}
}

static int Get_Text_Size(std::string text, std::string textType) {
	int character = 0;
	int size = 0;
	for (auto& letter : text) {
		character = letter;
		auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, letter) + textType);
		if (letterInformation.size() > 0) {
			size += letterInformation[4] - letterInformation[3] + 1;
		}
	}
	return size;
}

static int Get_Text_Size_At_Pos(std::string text, int targetPos, std::string textType) {
	int character = 0;
	int size = 0;
	int pos = 0;
	if (text.size() > 0 && targetPos <= text.size())
		for (int pos = 0; pos <= targetPos; pos++) {
			character = text[pos];
			auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, character) + textType);
			if (letterInformation.size() > 0) {
				if (pos != targetPos && pos + 1 != text.size())
					size += letterInformation[4] - letterInformation[3] + 1;
				else {
					if (pos != text.size() && targetPos != text.size())
						size += -letterInformation[3] - 1;
					else
						size += letterInformation[4] - letterInformation[3] - 1;
					return size;
				}
			}
		}
	return size;
}

static int Get_Text_Pos_Until_Length(std::string text, std::string textType, int length) {
	char character = 0;
	int size = 0;
	int pos = 0;
	while (size < length) {
		character = text[pos];
		auto& letterInformation = letterLibrary.getLetterInformation(std::string(1, character) + textType);
		if (letterInformation.size() > 0) {
			size += letterInformation[4] - letterInformation[3] + 1;
		}
		pos++;
	}
	return pos-3;
}

static ObjectLibrary* Get_Object_Library(int VAO, int VBO) {
	auto it = std::find_if(objLibraries.begin(), objLibraries.end(), [VAO, VBO](ObjectLibrary*& lib) {
		return (lib->getVAO() == VAO && lib->getVBO() == VBO);
		});
	if (it != objLibraries.end())
		return (*it);
	else {
		return rendToTextLibrary.get_Specific_Library(VAO, VBO);
	}
};

#endif // !GLOBAL_VARIABLES_H
