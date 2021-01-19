#ifndef ELEMENT_CREATOR_FUNCTIONS_H
#define ELEMENT_CREATOR_FUNCTIONS_H

#include <string>
#include "GUI.h"
#include "Global.h"
#include "ElementCreator.h"

/**
* Reset left palette modifier's RTT.
*
*/
void reset_Left_Palette_Modifier_RTT();

/**
* Reset right palette modifier's RTT.
*
*/
void reset_Right_Palette_Modifier_RTT();

/**
* Make sure that empty buttons aren't considered clicked.
*
*@param p, the palette which is to be used as a reference for valid clicks.
*@param clicked, the map of button clicks which is to be checked for redundant button clicks.
*/
void palette_Modifier_Check_Clicked_Buttons(Palette &p, std::unordered_map<int, int> &clicked);

/**
* The lambda that is to be triggered on a mouseclick on the left palette modifier.
*
*/
std::function<void(int& mousebutton, int& mouseState)> get_Palette_Modifier_Left_Mouse_Lambda();

/**
* The lambda that is to be triggered on a mouseclick on the right palette modifier.
*
*@return the generated lambda function.
*/
std::function<void(int& mousebutton, int& mouseState)> get_Palette_Modifier_Right_Mouse_Lambda();

/**
* The lambda function that is to be triggered on a scroll event.
*
*@param panel, the GUI panel which contains the element that contains triggered the scroll event.
*@param tbgName, the name of the toggle button group element which is the element that triggered the scroll event.
*@param sbName, the name of the scrollbar element which needs to be updated.
*@param rttName, the name of the render to texture element which needs to be updated.
*@param offset, the offset value of the rtt.
*@param offset_Max, the max offset before a new page is needed.
*@param page, the current page.
*@param selected_Palette, the selected palette.
*@map, the map of clicked buttons.
*
*@return the generated lambda function.
*/
std::function<void(double& v)> get_Scroll_Lambda_Tbg(GUIPanel* panel, std::string tbgName, std::string sbName, std::string rttName, double &offset, double &offset_Max, int &page, std::string &selected_Palette, std::unordered_map<int, int>& map);

/**
* The lambda function that is to be triggered on a scroll event.
*
*@param tbg, the toggle button group that triggers the hover event.
*
*@return the generated lambda function.
*/
std::function<void(double& xPos, double& yPos)> get_Hover_Lambda_Palette_Modifier_Right(ToggleButtonGroup* tbg);

/**
* The lambda function that is to be triggered on a scrollbar scroll event.
*
*@param sb, the scrollbar.
*@param rtt, the element containing the texture that is rendered to.
*@param elementWithRTT, the element that is used when drawing to the texture.
*@param offset, the offset value of the rtt.
*@param offset_Max, the max offset before a new page is needed.
*@param page, the current page.
*@param Page_Max, the max amount of pages.
*@param selected_Palette, the selected palette.
*
*@return the generated lambda function.
*/
std::function<void()> get_Palette_Modifier_Scrollbar_Scroll_Lambda(ScrollbarVertical* sb, GUIElement* rttElement, GUIElement* elementforRTT, GUIPanel* panel, RendToText* rtt, double& offset, double& offset_Max, int& page, int& Page_Max, std::string& selected_Palette, std::unordered_map<int, int>& map);


void prepare_Palette_Modifier_Rend_To_Text(GUIPanel* panel, RendToText* rtt, GUIElement* rttElement, GUIElement* tbg, Palette& p, int& page, double& offset_Max, std::unordered_map<int, int>& map);

void generate_Palette_Modifier_Rend_To_Text();

void generate_Vertices_Creation_Rend_To_Text();

void setup_Vertices_Generation_Selection_Area();
void update_Vertices_Generation_Selection_Area(std::string state);

std::function<void()> get_Vertices_Creation_Selection_Area_Scrollbar_Scroll_Lambda(ScrollbarVertical* sb, GUIElement* rttElement, GUIElement* elementforRTT, GUIPanel* panel, RendToText* rtt, double& offset);

void generate_Vertices_Creation_Preview_Rend_To_Text();

void update_Vertices_Creation_Info(Object* ob);

#endif
