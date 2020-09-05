#ifndef ELEMENT_CREATOR_H
#define ELEMENT_CREATOR_H

// Include GLFW
//#include <glfw3.h>

#include <vector>
#include <string>
#include <algorithm>
#include "Global.h"
#include "VertecesHandler.h"
#include "Object.h"

using std::vector;
using std::string;

// Bottom bar
void generate_GUI_Bottom_Bar(Objects& ob, VertecesHandler*& vh);
void generate_GUI_Bottom_Bar_text(Objects& ob, VertecesHandler*& vh, std::string xText, std::string yText, std::string zText);

// Text
void generate_GUI_Text(Objects& ob, VertecesHandler*& vh, double x, double y, std::string text);
void generate_GUI_Text(std::vector<Object*>& vec, VertecesHandler*& vh, double x, double y, std::string text);
void generate_GUI_Text(Objects& ob, VertecesHandler*& vh, double x, double y, std::string text, int rowLength);

// left panel
void generate_GUI_Left_Panel(Objects& ob, VertecesHandler*& vh, size_t displayState);
void generate_GUI_Left_Panel_Text_(Objects& ob, VertecesHandler*& vh);
void generate_GUI_Left_Panel_Selector(Objects & ob);

// Preview tiles
void generate_GUI_Preview_Tiles(Objects& ob);

// ItemInfo panel
void generate_GUI_Item_Info_Panel(Objects & ob, VertecesHandler*& vh);

// Palette modifier
void generate_Palette_Modifier(Objects & ob, VertecesHandler*& vh);

void generate_Palette_Modifier_Fill_Palette(Objects & ob, Palette& p);

#endif

