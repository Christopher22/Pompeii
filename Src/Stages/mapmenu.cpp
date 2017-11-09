#include "mapmenu.h"

#include "Helper/label.h"

#include "../World/luafile.h"

#include "../pompeii.h"

#include <algorithm>
#include <cassert>

MapMenu::MapMenu(Font* font, std::map<std::string, std::string>& maps): Menu(font), m_maps(maps) {
	assert(maps.size() > 0);
}

void MapMenu::addElements() {
	this->addElement(new MenuEntry(new Label(GetFont(), "Available maps:", 1.4f), nullptr));

	std::for_each(m_maps.begin(), m_maps.end(), [&](const std::pair<const std::string, const std::string>& map) {
		this->addElement(new MenuEntry(new Label(GetFont(), map.first.c_str()), &MapMenu::loadMap));
	});

	this->addElement(new MenuEntry(new Label(GetFont(), "Back to menu"), [](Menu*) {
		assert(Pompeii::GetInstance().GetStages() == 2);
		Pompeii::GetInstance().PopStage();
	}));
}

std::map<std::string, std::string> MapMenu::GetAvailableMaps() {
	return World::LuaFile::LoadTable<std::string, std::string>("Assets\\maps.lua");
}

void MapMenu::loadMap(Menu* menu) {
	auto mapMenu = dynamic_cast<MapMenu*>(menu);
	assert(mapMenu);
	assert(mapMenu->GetCurrentSelection());

	std::string mapPath(mapMenu->m_maps[mapMenu->GetCurrentSelection()->GetLabel()->GetText()]);
	Pompeii::GetInstance().PopStage();
	Pompeii::GetInstance().StartGame(mapPath.c_str());
}