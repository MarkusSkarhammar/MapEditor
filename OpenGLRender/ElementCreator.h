#ifndef ELEMENT_CREATOR_H
#define ELEMENT_CREATOR_H

// Include GLFW
//#include <glfw3.h>

#include <vector>
#include <string>
#include <algorithm>
#include "Global.h"
#include "Vertices.h"
#include "DrawObject.h"
#include "ElementCreatorFunctions.h"

using std::vector;
using std::string;

// Bottom bar
void generate_GUI_Bottom_Bar();

// Text
void generate_GUI_Text(TextDrawObject* ob, double x, double y, int textStartOffset, int rowLength, int height, std::string text, string textType, bool ellipsis, int centeredText, RendToText* rtt = nullptr);

// left panel
void generate_GUI_Left_Panel(size_t displayState);
void generate_Left_Panel_Tiles(Palette& p);

// Preview tiles
void generate_GUI_Preview_Tiles();

// ItemInfo panel
void generate_GUI_Item_Info_Panel();

// Palette modifier
void generate_Palette_Modifier();

// Render to text stuff
void generate_Left_Panel_Rend_To_Text();

// Vertices Creation
void generate_Vertices_Creation_Panel();

#endif

