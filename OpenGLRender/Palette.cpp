#include "Palette.h"
#include "Global.h"

void Palette::addItem(int id, bool doubleSize)
{
	int position;
	if (!doubleSize)
		position = getFirtsPossiblePosition(items);
	else
		position = getFirstPossiblePositionForDoubleSize(items);

	items.push_back(PaletteItem(id, doubleSize, position % 4, position / 4));
}

void Palette::addItems(std::vector<PaletteItem>& list)
{
	items.clear();
	int x = 0, y = 0;
	for (auto item : list) { //Insert non doublSized items first
		if (!item.isDoubleSize()) {
			item.changePositions(x, y);
			items.push_back(item);
			if (++x == 4) {
				x = 0;
				y++;
			}
		}
	}
	if (x != 0) {
		x = 0;
		y++;
	}
	for (auto item : list) { //Insert doublSized items
		if (item.isDoubleSize()) {
			item.changePositions(x, y);
			items.push_back(item);
			if ((x += 2) >= 4) {
				x = 0;
				y += 2;
			}
		}
	}
}

void Palette::replacePalette(std::vector<PaletteItem>& p)
{
	items.clear();
	items.shrink_to_fit();
	for (auto item : p) {
		items.push_back(item);
	}
}

std::pair<bool, PaletteItem&> Palette::getItemAtLocation(int x, int y)
{
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y) {
			return std::pair<bool, PaletteItem&>(true, e);
		}
	}
	PaletteItem item = PaletteItem(-1, false, -1, -1);
	return std::pair<bool, PaletteItem&>(false, item);
}

void Palette::clear()
{
	items.clear();
	items.shrink_to_fit();
	maxY = 0;
}

void Palette::removeItemAtLocation(int x, int y)
{
	int pos = 0;
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y) {
			items.erase(items.begin()+pos);
			if (y == maxY)
				findMaxY();
			break;
		}
		pos++;
	}
}

void Palette::removeItemAtLocation(int x, int y, std::vector<PaletteItem>& changes)
{
	int pos = 0;
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y) {
			changes.push_back(PaletteItem(e));
			items.erase(items.begin() + pos);
			if (y == maxY)
				findMaxY();
			break;
		}
		pos++;
	}
}

void Palette::removeItemAtLocationUnlessSame(int x, int y, PaletteItem& other)
{
	int pos = 0;
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y) {
			if (e != other) {
				items.erase(items.begin() + pos);
				if (y == maxY)
					findMaxY();
			}
			break;
		}
		pos++;
	}
}

void Palette::removeItemAtLocationIFDoubleSize(int x, int y)
{
	int pos = 0;
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y) {
			if (e.isDoubleSize()) {
				items.erase(items.begin() + pos);
				if (y == maxY)
					findMaxY();
			}
			break;
		}
		pos++;
	}
}

void Palette::removeItemAtLocationIFDoubleSizeUnlessSame(int x, int y, PaletteItem& other)
{
	int pos = 0;
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y) {
			if (e.isDoubleSize() && e != other) {
				items.erase(items.begin() + pos);
				if (y == maxY)
					findMaxY();
			}
			break;
		}
		pos++;
	}
}

void Palette::changePositionAt(int xTo, int yTo, int xSorce, int ySorce)
{
	for (auto& e : items) {
		if (e.getX() == xSorce && e.getY() == ySorce) {
			e.setX(xTo);
			e.setY(yTo);
			break;
		}
	}
}

void Palette::changePositionAt(int xTo, int yTo, PaletteItem& source)
{
	for (auto& e : items) {
		if (e == source) {
			e.setX(xTo);
			e.setY(yTo);
			break;
		}
	}
}

void Palette::changePositionAtUnlessSame(int xTo, int yTo, int xSorce, int ySorce, PaletteItem& other)
{
	for (auto& e : items) {
		if (e.getX() == xSorce && e.getY() == ySorce && e != other) {
			e.setX(xTo);
			e.setY(yTo);
			break;
		}
	}
}

void Palette::change_ID_And_Size_At(int id, bool size, int x, int y)
{
	for (auto& e: items) {
		if (e.getX() == x && e.getY() == y) {
			e.setID(id);
			e.setDoubleSize(size);
			break;
		}
	}
}


void Palette::fillPalettes(std::vector<Palette>& palettes)
{
	{

		Palette p("All");
		int pos = 0, doubleSizeIncrement = 0;
		bool change = false, currentItemSizeCheck;
		for (auto& i : itemAtlas.getEveryItem()) {
			if (i.first->getID() != -1 && !i.first->getAnimationOnly()) {
				currentItemSizeCheck = itemAtlas.checkIfDouleSize(i.first->getID());
				if (change != currentItemSizeCheck) {
					change = currentItemSizeCheck;
					if (pos % 4 > 0)
						pos += (4 - pos % 4);
				}
				p.addItem(i.first->getID(), currentItemSizeCheck, pos % 4, pos / 4);
				if (currentItemSizeCheck) {
					pos += 2;
					doubleSizeIncrement++;
					if (doubleSizeIncrement == 2) {
						doubleSizeIncrement = 0;
						pos += 4;
					}
				}
				else pos++;
			}
		}
		palettes.push_back(p);

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
}

PaletteItem Palette::getItemFromID(int ID)
{
	for (auto& i : items) {
		if (i.getID() == ID)
			return i;
	}
	return PaletteItem();
}

float Palette::getMaxYAsFloat(int maxPage, float maxOffset)
{
	return std::fmod((maxY + maxPage + 1) * 64.0, maxOffset);
}

void Palette::findMaxY()
{
	maxY = 0;
	for (auto& i : items) {
		if (i.getY() > maxY)
			maxY = i.getY();
	}
}

/**
** Check if placing a double sized item at the specified location is blocked by another double sized item.
**
** @param xDst The x value of the targeted location to check.
** @param yDst The y value of the targeted location to check.
** @param xSrc The x value of the source location.
** @param ySrc The y value of the source location.
**
** @return a 0 if there's no problem in placing a double sized item, or a 1 if something is blocking the placement.
**/
int Palette::check_If_Double_Size_Is_Blocked_At_Location(int xDst, int yDst, int xSrc, int ySrc)
{
	int b = is_Double_Size(xDst, yDst);
	if (b == 1)
		b = yDst * 4 + xDst;
	if (!b && xDst > 0 && !(xDst - 1 == xSrc && yDst == ySrc)) {
		b = is_Double_Size(xDst - 1, yDst);
	}
	if (!b && y > 0 && !(xDst == xSrc && yDst - 1 == ySrc)) {
		b = is_Double_Size(xDst, yDst - 1);
	}
	if (!b && xDst < 3 && !(xDst + 1 == xSrc && yDst == ySrc)) {
		b = is_Double_Size(xDst + 1, yDst);
	}
	if (!b && !(xDst == xSrc && yDst + 1 == ySrc)) {
		b = is_Double_Size(xDst, yDst + 1);
	}
	if (!b && xDst > 0 && yDst > 0 && !(xDst == xSrc && yDst - 1 == ySrc)) {
		b = is_Double_Size(xDst - 1, yDst - 1);
	}
	if (!b && xDst < 3 && !(xDst + 1 == xSrc && yDst + 1 == ySrc)) {
		b = is_Double_Size(xDst + 1, yDst + 1);
	}
	if (!b && xDst < 3 && yDst > 0 && !(xDst + 1 == xSrc && yDst - 1 == ySrc)) {
		b = is_Double_Size(xDst + 1, yDst - 1);
	}
	if (!b && xDst > 0 && !(xDst - 1 == xSrc && yDst + 1 == ySrc)) {
		b = is_Double_Size(xDst - 1, yDst + 1);
	}
	return b;
}

/**
** Check if it's possible to place a double sized item at the specified location.
**
** @param x The x value of the location to check.
** @param y The y value of the location to check.
**
** @return a 0 if there's no problem in placing a double sized item, or the position of the item preventing the placement of a double sized item.
**/
int Palette::check_If_Double_Size_Is_Possible_At_Location(int xDst, int yDst, int xSrc, int ySrc)
{
	int b = is_Double_Size(xDst, yDst);
	if (b == 1)
		b = yDst * 4 + xDst;
	if (!b && xDst > 0 && !(xDst - 1 == xSrc && yDst == ySrc)) {
		b = is_Double_Size(xDst - 1, yDst);
		if (b == 1)
			b = yDst * 4 + (xDst - 1);
	}
	if (!b && yDst > 0 && !(xDst == xSrc && yDst - 1 == ySrc)) {
		b = is_Double_Size(xDst, yDst - 1);
		if (b == 1)
			b = (yDst - 1) * 4 + xDst;
	}
	if (!b && xDst > 0 && yDst > 0 && !(xDst - 1 == xSrc && yDst - 1 == ySrc)) {
		b = is_Double_Size(xDst - 1, yDst - 1);
		if (b == 1)
			b = (yDst - 1) * 4 + (xDst - 1);
	}
	if (!b) {
		b = item_Exists_at(xDst, yDst);
		if (b == 1) {
			b = yDst * 4 + xDst;
			if (b == 0)
				b = -1;
		}
	}
	if (!b && xDst < 3 && !(xDst + 1 == xSrc && yDst == ySrc)) {
		b = item_Exists_at(xDst + 1, yDst);
		if (b == 1)
			b = yDst * 4 + (xDst + 1);
	}
	if (!b && !(xDst == xSrc && yDst + 1 == ySrc)) {
		b = item_Exists_at(xDst, yDst + 1);
		if (b == 1)
			b = (yDst + 1) * 4 + xDst;
	}
	if (!b && xDst < 3 && !(xDst + 1 == xSrc && yDst + 1 == ySrc)) {
		b = item_Exists_at(xDst + 1, yDst + 1);
		if (b == 1)
			b = (yDst + 1) * 4 + (xDst + 1);
	}
	if (b == 0)
		return -1;
	else if (b == -1)
		return 0;
	else
		return b;
}

/**
** Check if it's possible to place a normal sized item at the specified location.
**
** @param x The x value of the location to check.
** @param y The y value of the location to check.
**
** @return a 0 if there's no problem in placing a double sized item, or the position of the item preventing the placement of a double sized item.
**/
int Palette::check_If_Normal_Size_Is_Possible_At_Location(int x, int y)
{
	int b = false;
	if (!b && x > 0) {
		b = is_Double_Size(x - 1, y);
		if (b == 1)
			b = y * 4 + (x - 1);
	}
	if (!b && y > 0) {
		b = is_Double_Size(x, y - 1);
		if (b == 1)
			b = (y - 1) * 4 + x;
	}
	if (!b && x > 0 && y > 0) {
		b = is_Double_Size(x - 1, y - 1);
		if (b == 1)
			b = (y - 1) * 4 + (x - 1);
	}
	return b;
}

/**
** Check if the specified location is of an item with a double size.
**
** @param x The x value of the location to check.
** @param y The y value of the location to check.
**
** @return bool of whether the specified location is of a double sized item.
**/
bool Palette::is_Double_Size(int x, int y)
{
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y)
			return e.isDoubleSize();
	}
	return false;
}

/**
** Check if an item exists at specified location.
**
** @param x The x value of the location to check.
** @param y The y value of the location to check.
**
** @return bool of whether the specified location contains an item.
**/
bool Palette::item_Exists_at(int x, int y)
{
	for (auto& e : items) {
		if (e.getX() == x && e.getY() == y)
			return true;
	}
	return false;
}

void Palette::sortPalette()
{
	std::sort(items.begin(), items.end(), [](PaletteItem& lhs, PaletteItem& rhs) {

		// a=b for primary condition, go to secondary
		if (lhs.getY() < rhs.getY()) return true;
		if (rhs.getY() < lhs.getY()) return false;

		if (lhs.getX() < rhs.getX()) return true;
		if (rhs.getX() < lhs.getX()) return false;

		return false;
		});
	findMaxY();
}


void PaletteItem::change_ID_And_Size(PaletteItem& other)
{
	this->doubleSize = other.doubleSize; this->id = other.id;
}

PaletteItem& PaletteItem::operator=(PaletteItem other)
{
	if (this == &other)
		return *this;
	id = other.getID(); doubleSize = other.doubleSize; x = other.x; y = other.y;
	return *this;
}



bool PaletteItem::operator==(const PaletteItem& other)
{
	return ((id == other.id) && (this->doubleSize == other.doubleSize) && (this->x == other.x) && (this->y == other.y));
}

bool PaletteItem::operator!=(const PaletteItem& other)
{
	return !((id == other.id) && (this->doubleSize == other.doubleSize) && (this->x == other.x) && (this->y == other.y));
}

int PaletteItem::operator>(const PaletteItem& other)
{
	int posThis = (this->y * 4 + this->x), posOther = (other.y * 4 + other.x);
	return posThis-posOther;
}

int getFirstPossiblePositionForDoubleSize(std::vector<PaletteItem> items)
{
	return 0;
}

int getFirtsPossiblePosition(std::vector<PaletteItem> items)
{
	if (items.size() > 0) {
		int x = 0; y = 0;
		for (auto e : items) {
			if (e.getY() > y) {
				y = e.getY();
				x = e.getX();
			}
			else if (e.getY() == y && e.getX() > x) {
				x = e.getX();
			}
			if (++x == 4) {
				x = 0;
				y++;
			}
		}
		return y * 4 + x;
	}
	else
		return 0;
}
