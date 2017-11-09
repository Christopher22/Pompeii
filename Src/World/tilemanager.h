#pragma once

#include "types.h"
#include "tile.h"
#include "luafile.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>
#include <string>

namespace World {
	template<typename Texture>
	class TileManager
	{
	public:
		TileManager() : m_pTiles(new Tile<Texture>*[std::numeric_limits<Id>::max() + 1]) {
			std::fill(m_pTiles, m_pTiles + std::numeric_limits<Id>::max() + 1, nullptr);
		}

		~TileManager() {
			ForEach([](Tile<Texture>* tile) {
				delete tile;
				tile = nullptr;
			});

			delete[] m_pTiles;
			m_pTiles = nullptr;
		}

		template<typename Functor>
		void ForEach(const Functor functor) {
			std::for_each(m_pTiles, m_pTiles + std::numeric_limits<Id>::max() + 1, [&](Tile<Texture>* tile) {
				if (tile) {
					functor(tile);
				}
			});
		}

		void Process(const float deltaTime) {
			ForEach([&](Tile<Texture>* tile) {
				tile->Process(deltaTime);
			});
		}

		template<typename CustomData>
		bool Load(const TextureLoader<Texture, CustomData> loader, const Path path, CustomData customData) {
			std::map<int, std::string> tiles = LuaFile::LoadTable<int, std::string>(path);
			if (tiles.size() == 0) {
				return false;
			}

			for (auto tileName : tiles) {
				Tile<Texture>* tile = Tile<Texture>::LoadFromFile(loader, tileName.second, customData);
				if (tile) {
					this->m_pTiles[static_cast<Id>(tileName.first)] = tile;
				} else {
					ForEach([](Tile<Texture>* tile) {
						delete tile;
						tile = nullptr;
					});
					return false;
				}
			}

			return true;
		}

		inline Tile<Texture>* operator[] (const Id id) const {
			return m_pTiles[id];
		}

	private:
		TileManager(const TileManager& copy);
		Tile<Texture>** m_pTiles;
	};
}