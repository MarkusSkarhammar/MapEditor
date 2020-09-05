#ifndef PALETTE_WORK_H
#define PALETTE_WORK_H


#include <vector>
#include <string>
#include "../pugixml/pugixml.hpp"

using namespace pugi;


class Palette {
public:
	Palette(std::string name): name(name) {};
	std::string getName() { return name; };
	void addItem(int id, bool doubleSize, size_t xPos, size_t yPos) { items.push_back(std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>(std::pair<int, bool>(id, doubleSize), std::pair<size_t, size_t> (xPos, yPos))); };
	std::vector<std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>>& getPalette() { return items; };
	int getMaxPage(int& paletteMaxY) { return items.at(items.size() - 1).second.second / paletteMaxY; };
private:
	std::vector<std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>> items;
	std::string name{""};
};

static void fillPalettes(std::vector<Palette>& palettes) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("./resources/Palettes/Palettes.xml");
	
	std::string typeS;
	for (pugi::xml_node palette = doc.child("Palettes").child("Palette"); palette; palette = palette.next_sibling("Palette"))
	{
		typeS = palette.attribute("id").as_string();
		Palette pal(typeS);
		for (pugi::xml_node attribute = palette.child("attribute"); attribute; attribute = attribute.next_sibling("attribute"))
		{
			std::pair<int, bool> info(attribute.attribute("id").as_int(), attribute.attribute("doubleSize").as_bool());
			std::pair<int, int> pos(attribute.attribute("x").as_int(), attribute.attribute("y").as_int());
			pal.addItem(info.first, info.second, pos.first, pos.second);
		}
		palettes.push_back(pal);
	}

}
#endif
