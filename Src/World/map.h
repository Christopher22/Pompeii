#pragma once
#pragma warning( disable : 4996 )

#include "types.h"
#include "coordinate.h"
#include "tilemanager.h"
#include "luafile.h"

#include <queue>
#include <algorithm>
#include <string>
#include <fstream>
#include <limits>
#include <cassert>
#include <unordered_set>

struct lua_State;

namespace World {

	template<typename Texture>
	class Map {
	public:
		Map(TileManager<Texture>* tiles, const Index width, const Index height, const Id defaultTile, const bool isBackground) : m_tiles(tiles), m_width(width), m_height(height), m_pIds(new Id[width * height]), m_isBackground(isBackground), m_nextTiles() {
			std::fill(m_pIds, m_pIds + width * height - 1, defaultTile);

			for (Index x = 0; x < width; x++) {
				for (Index y = 0; y < height; y++) {
					m_nextTiles.emplace(x, y);
				}
			}

			if (!m_isBackground) {
				tiles->ForEach([&](Tile<Texture>* tile) {
					tile->GetLua()->RegisterFunction<Map<Texture>>({ "World", "setTile" }, &Map<Texture>::setTile, this);
					tile->GetLua()->RegisterFunction<Map<Texture>>({ "World", "getTile" }, &Map<Texture>::getTile, this);
					tile->GetLua()->RegisterFunction<Map<Texture>>({ "World", "addNextTile" }, &Map<Texture>::addNextTile, this);
				});
			}
		}

		~Map() {
			if (!m_isBackground) {
				m_tiles->ForEach([&](Tile<Texture>* tile) {
					tile->GetLua()->UnregisterFunction({ "World", "setTile" });
					tile->GetLua()->UnregisterFunction({ "World", "getTile" });
					tile->GetLua()->UnregisterFunction({ "World", "addNextTile" });
				});
			}
			delete[] m_pIds;
			m_pIds = nullptr;
		}

		inline Id& operator[] (const Coordinate index) {
			return m_pIds[index.GetX() + m_width * index.GetY()];
		}

		inline Index GetHeight() const {
			return m_height;
		}

		inline Index GetWidth() const {
			return m_width;
		}

		inline void Process(const float deltaTime) {
			m_tiles->Process(deltaTime);
		}

		template<typename ErrorHandler>
		void NextTurn(ErrorHandler luaErrorHandler) {
			if (!m_isBackground) {
				for (auto i = m_nextTiles.size(); i > 0; i--) {
					const auto current = m_nextTiles.front();
					if (m_ignoreTiles.find(current) == m_ignoreTiles.end()) {
						const auto id = (*this)[current];
						const auto tile = (*m_tiles)[id];
						if (tile) {
							tile->Update(current, luaErrorHandler);
						}
					}
					m_nextTiles.pop();
				}
				m_ignoreTiles.clear();
			}
		}

		inline void AddNextTile(const Coordinate position) {
			m_nextTiles.push(position);
		}

		template<typename CustomData>
		void Draw(TextureDrawer<Texture, CustomData> drawer, CustomData customData) const  {
			for (Index x = 0; x < m_width; x++) {
				for (Index y = 0; y < m_height; y++) {
					const Id id = m_pIds[x + m_width * y];
					Tile<Texture>* target = (*m_tiles)[id];
					if (target) {
						target->Draw(drawer, Coordinate(x, y), customData);
					}
				}
			}
		}

		static Map* Load(TileManager<Texture>* tiles, Path path, bool isBackground) {
			std::ifstream img(path, std::ios::binary);
			if (img.bad()) {
				return nullptr;
			}

			std::string inputLine;
			if (!(img >> inputLine) || inputLine.compare("P5") != 0) {
				return nullptr;
			}

			Index width = 0, height = 0;
			try {
				while (img >> inputLine) {
					if (inputLine.at(0) == '#') {
						// TODO: Parse metadata
						continue;
					} else if (width == 0) {
						width = (Index)std::stoi(inputLine);
					} else if (height == 0) {
						height = (Index)std::stoi(inputLine);
					} else {
						if (std::stoi(inputLine) > std::numeric_limits<Id>::max()) {
							return nullptr;
						}
						break;
					}
				}

				// Check if the while loop was breaked early
				if (height == 0) {
					return nullptr;
				}
			}
			catch (std::exception exeption) {
				return nullptr;
			}

			img.ignore(1); // Ignore whitespace

			char* buffer = new char[width * height];
			if (!img.read(buffer, width * height)) {
				delete[] buffer;
				return nullptr;
			}
			else {
				Map<Texture>* result = new Map<Texture>(tiles, width, height, 0, isBackground);
				std::transform(buffer, buffer + width * height, result->m_pIds, [](char id) {
					return static_cast<Id>(id);
				});

				delete[] buffer;
				return result;
			}
		}

	private:
		Map(const Map& map);

		static int setTile(lua_State* lua) {
			auto world = ((World::Map<Engine::Texture>*)lua_touserdata(lua, lua_upvalueindex(1)));
			int x = (int)luaL_checknumber(lua, 1), y = (int)luaL_checknumber(lua, 2);
			assert(x >= 0 && x < (int)world->GetWidth() && y >= 0 && y < (int)world->GetHeight());

			const auto id = (World::Id)luaL_checknumber(lua, 3);
			const auto target = World::Coordinate(World::Index(x), World::Index(y));

			world->operator[](target) = id;
			world->m_ignoreTiles.emplace(target);
			return 0;
		}

		static int getTile(lua_State* lua) {
			auto world = ((World::Map<Engine::Texture>*)lua_touserdata(lua, lua_upvalueindex(1)));
			const int x = (int)luaL_checknumber(lua, 1), y = (int)luaL_checknumber(lua, 2);

			if (x >= 0 && x < (int)world->GetWidth() && y >= 0 && y < (int)world->GetHeight()) {
				lua_pushnumber(lua, world->operator[](World::Coordinate((World::Index)x, (World::Index)y)));
			} else {
				lua_pushnumber(lua, -1);
			}
			return 1;
		}

		static int addNextTile(lua_State* lua) {
			auto world = ((World::Map<Engine::Texture>*)lua_touserdata(lua, lua_upvalueindex(1)));
			const int x = (int)luaL_checknumber(lua, 1), y = (int)luaL_checknumber(lua, 2);
			assert(x >= 0 && x < (int)world->GetWidth() && y >= 0 && y < (int)world->GetHeight());

			world->AddNextTile(World::Coordinate((World::Index)x, (World::Index)y));
			return 0;
		}

		TileManager<Texture>* m_tiles;
		Id* m_pIds;
		const Index m_width, m_height;
		const bool m_isBackground;
		std::queue<Coordinate> m_nextTiles;
		std::unordered_set<Coordinate> m_ignoreTiles;
	};
}