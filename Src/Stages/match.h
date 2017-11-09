#pragma once

#include "stage.h"
#include "../World/types.h"

#include <string>

struct lua_State;

namespace Engine {
	class BackBuffer;
	class Texture;
}

namespace World {
	template<typename Texture>
	class Map;

	template<typename Texture>
	class TileManager;
	class Coordinate;
}

class Label;

class Match : public Stage {
public:
	const static unsigned short TILE_SIZE = 64;
	const static unsigned short RESCUE_BONUS = 3;

	enum Tiles : World::Id {
		Player = 1,
		Lava = 2,
		Goal = 3,
		Roman = 4
	};

	enum Status {
		Running,
		Win,
		Lose
	};

	virtual ~Match();
	void Draw(Engine::BackBuffer& backBuffer);
	void Input(SDL_Event* input);
	void Process(float deltaTime);
	unsigned short GetRound() const;
	Status GetStatus() const;
	World::Path GetPath() const;
	static Match* LoadMatch(Font* font, const char* path);

private:
	Match(Font* font, const char* path, World::Map<Engine::Texture>* map, World::Map<Engine::Texture>* background, World::TileManager<Engine::Texture>* tiles);
	Match(const Match& match);
	void showResultMenu();
	static int win(lua_State* lua);
	static int lose(lua_State* lua);
	static int rescueRoman(lua_State* lua);
	static int getMovement(lua_State* lua);
	static int getTileId(lua_State* lua);
	static int log(lua_State* lua);

	World::Map<Engine::Texture> *m_pMap, *m_pBackground;
	World::TileManager<Engine::Texture>* m_pTiles;
	char m_moveX, m_moveY;
	char m_movementModifier;
	Status m_status;
	unsigned short m_round;
	Label* m_pGui;
	std::string m_mapPath;
};