#include "match.h"

#include "resultmenu.h"

#include "Helper/font.h"
#include "Helper/label.h"

#include "../pompeii.h"
#include "../World/map.h"
#include "../World/tile.h"
#include "../World/tilemanager.h"

#include "../Engine/backbuffer.h"
#include "../Engine/logmanager.h"
#include "../Engine/texturemanager.h"
#include "../Engine/texture.h"

#include <SDL.h>

#include <iomanip>
#include <sstream>
#include <cassert>
#include <cmath>

Match::Match(Font* font, const char* path, World::Map<Engine::Texture>* map, World::Map<Engine::Texture>* background, World::TileManager<Engine::Texture>* tiles) : Stage(font), m_pMap(map), m_mapPath(path), m_pTiles(tiles), m_pBackground(background), m_moveX(0), m_moveY(0), m_movementModifier(0), m_status(Match::Status::Running), m_round(1), m_pGui(new Label(font, "Round 01")) {
	assert(map);
	assert(tiles);
	assert(background);

	m_pGui->SetX(10);
	m_pGui->SetY(10);

	m_pTiles->ForEach([&](World::Tile<Engine::Texture>* tile) {
		auto lua = tile->GetLua();
		lua->RegisterFunction<Match>({ "Pompeii", "win" }, &Match::win, this);
		lua->RegisterFunction<Match>({ "Pompeii", "lose" }, &Match::lose, this);
		lua->RegisterFunction<Match>({ "Pompeii", "rescueRoman" }, &Match::rescueRoman, this);
		lua->RegisterFunction<Match>({ "Pompeii", "getMovement" }, &Match::getMovement, this);
		lua->RegisterFunction<Match>({ "Pompeii", "log" }, &Match::log, this);

		lua->Set<int>({ "Pompeii", "TILES", "PLAYER" }, Match::Tiles::Player);
		lua->Set<int>({ "Pompeii", "TILES", "LAVA" }, Match::Tiles::Lava);
		lua->Set<int>({ "Pompeii", "TILES", "ROMAN" }, Match::Tiles::Roman);
		lua->Set<int>({ "Pompeii", "TILES", "GOAL" }, Match::Tiles::Goal);
	});
}

Match::~Match() {
	delete m_pMap;
	m_pMap = nullptr;

	delete m_pBackground;
	m_pBackground = nullptr;

	delete m_pTiles;
	m_pTiles = nullptr;

	delete m_pGui;
	m_pGui = nullptr;
}

void Match::Process(float deltaTime) {
	m_pBackground->Process(deltaTime);
	m_pMap->Process(deltaTime);
}

void Match::Draw(Engine::BackBuffer& backBuffer) {
	m_pBackground->Draw<Engine::BackBuffer*>([](World::Coordinate coordinate, Engine::Texture* texture, Engine::BackBuffer* buffer) {
		buffer->DrawTexture(texture, coordinate.GetX() * Match::TILE_SIZE, coordinate.GetY() * Match::TILE_SIZE);
	}, &backBuffer);

	m_pMap->Draw<Engine::BackBuffer*>([](World::Coordinate coordinate, Engine::Texture* texture, Engine::BackBuffer* buffer) {
		buffer->DrawTexture(texture, coordinate.GetX() * Match::TILE_SIZE, coordinate.GetY() * Match::TILE_SIZE);
	}, &backBuffer);

	m_pGui->Draw(backBuffer);
}

Match::Status Match::GetStatus() const {
	return m_status;
}

void Match::Input(SDL_Event* input) {
	const int JOYSTICK_DEAD_ZONE = 8000;
	if (input->type == SDL_JOYAXISMOTION && input->jaxis.which == 0) {
		if (std::abs(input->jaxis.value) < JOYSTICK_DEAD_ZONE) {
			return;
		}

		Engine::BackBuffer* buffer = Pompeii::GetInstance().GetBackbuffer();
		unsigned short x = buffer->getCameraX(), y = buffer->getCameraY();
		if (input->jaxis.axis == 0) {
			if (x > 0 && input->jaxis.value < 0) {
				x -= 1;
			} else if (x + buffer->GetWidth() < m_pMap->GetWidth() * Match::TILE_SIZE) {
				x += 1;
			}
		} else if (input->jaxis.axis == 1) {
			if (y > 0 && input->jaxis.value < 0) {
				y -= 1;
			} else if (y + buffer->GetHeight() < m_pMap->GetHeight() * Match::TILE_SIZE) {
				y += 1;
			}
		}

		buffer->setCamera(x, y);
	} else if (input->type == SDL_JOYHATMOTION && m_status == Status::Running) {
		switch (input->jhat.value) {
		case SDL_HAT_UP: m_moveY = -1 - m_movementModifier; break;
		case SDL_HAT_DOWN: m_moveY = 1 + m_movementModifier; break;
		case SDL_HAT_LEFT: m_moveX = -1 - m_movementModifier; break;
		case SDL_HAT_RIGHT: m_moveX = 1 + m_movementModifier; break;
		default: return;
		}

		m_round += 1;
		Engine::LogManager::GetInstance().Log("Round %d started.", m_round);

		m_pMap->NextTurn([](const char* errorMessage) {
			Engine::LogManager::GetInstance().Log("[LUA] Error: %s", errorMessage);
		});

		m_moveX = 0;
		m_moveY = 0;
		m_movementModifier = 0;

		std::ostringstream ss;
		ss << "Round " << std::setw(2) << std::setfill('0') << m_round << "\n";
		m_pGui->GetText() = ss.str();
	} else if (input->type == SDL_KEYDOWN && m_status == Status::Running) {
		switch (input->key.keysym.sym) {
		case SDLK_g: 
			m_pMap->NextTurn([](const char* errorMessage) {
				Engine::LogManager::GetInstance().Log("[LUA] Error: %s", errorMessage);
			});
			break;
		case SDLK_c:
			++m_movementModifier;
			break;
		default: break;
		}
	}
}

Match* Match::LoadMatch(Font* font, const char* path) {
	World::TileManager<Engine::Texture>* manager = new World::TileManager<Engine::Texture>();
	manager->Load<Engine::BackBuffer*>([](World::Path path, Engine::BackBuffer* buffer) {
		return buffer->CreateUnmanagedTexture(path.c_str());
	}, "Assets\\tiles.lua", Pompeii::GetInstance().GetBackbuffer());
	
	World::Map<Engine::Texture>* map = World::Map<Engine::Texture>::Load(manager, path, false);
	World::Map<Engine::Texture>* background = World::Map<Engine::Texture>::Load(manager, std::string(path) + ".bg", true);
	if (!map || !background) {
		if (map) {
			delete map;
			map = nullptr;
		} else {
			Engine::LogManager::GetInstance().Log("Loading of foreground failed!");
		}

		if (background) {
			delete background;
			background = nullptr;
		} else {
			Engine::LogManager::GetInstance().Log("Loading of background failed!");
		}

		delete manager;
		manager = nullptr;

		return nullptr;
	}

	return new Match(font, path, map, background, manager);
}

void Match::showResultMenu() {
	auto backbuffer = Pompeii::GetInstance().GetBackbuffer();
	ResultMenu* menu = new ResultMenu(GetFont(), this);
	menu->Initialize(*backbuffer);
	Pompeii::GetInstance().PushStage(menu);

	backbuffer->setCamera(0, 0);
}

int Match::win(lua_State* lua) {
	auto match = ((Match*)lua_touserdata(lua, lua_upvalueindex(1)));
	if (match->GetStatus() == Status::Running) {
		match->m_status = Status::Win;
		match->showResultMenu();
	}
	return 0;
}

int Match::lose(lua_State* lua) {
	auto match = ((Match*)lua_touserdata(lua, lua_upvalueindex(1)));
	if (match->GetStatus() == Status::Running) {
		match->m_status = Status::Lose;
		match->showResultMenu();
	}
	return 0;
}

int Match::rescueRoman(lua_State* lua) {
	auto match = ((Match*)lua_touserdata(lua, lua_upvalueindex(1)));
	match->m_round = match->m_round > Match::RESCUE_BONUS ? (match->m_round - Match::RESCUE_BONUS) : 0;

	return 0;
}

int Match::getMovement(lua_State* lua) {
	auto match = ((Match*)lua_touserdata(lua, lua_upvalueindex(1)));
	lua_pushnumber(lua, match->m_moveX);
	lua_pushnumber(lua, match->m_moveY);
	return 2;
}

int Match::log(lua_State* lua) {
	auto message = luaL_checkstring(lua, 1);
	if (message) {
		Engine::LogManager::GetInstance().Log("[LUA] %s", message);
	}
	return 0;
}

unsigned short Match::GetRound() const {
	return m_round;
}

World::Path Match::GetPath() const {
	return m_mapPath;
}