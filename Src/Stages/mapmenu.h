#pragma once

#include "menu.h"

#include <map>
#include <string>

class MapMenu : public Menu {
public:
	MapMenu(Font* font, std::map<std::string, std::string>& maps);
	void addElements();
	static std::map<std::string, std::string> GetAvailableMaps();

private:
	static void loadMap(Menu* menu);

	std::map<std::string, std::string> m_maps;
};