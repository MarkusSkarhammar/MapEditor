#include "Palette.h"

std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>& Palette::getItemAtLocation(int x, int y)
{
	std::pair<std::pair<int, bool>, std::pair<size_t, size_t>> item = std::pair<std::pair<int, bool>, std::pair<size_t, size_t>>(std::pair<int, bool>(-1, false), std::pair<size_t, size_t>(-1, -1));
	for (auto& e : items) {
		if (e.second.first == x && e.second.second == y) {
			return e;
		}
	}
	return item;
}

void Palette::removeItemAtLocation(int x, int y)
{
	int pos = 0;
	for (auto& e : items) {
		if (e.second.first == x && e.second.second == y) {
			items.erase(items.begin()+pos);
			break;
		}
		pos++;
	}
}
