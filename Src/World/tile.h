#pragma once

#include "types.h"
#include "luafile.h"
#include "coordinate.h"

#include <string>
#include <cassert>
#include <vector>

namespace World {
	template<typename Texture>
	class Tile {
	public:
		~Tile() {
			delete m_pLua;
			m_pLua = nullptr;

			std::for_each(m_pTexture.begin(), m_pTexture.end(), [](Texture* texture) {
				delete texture;
				texture = nullptr;
			});
		}

		void Process(const float deltaTime) {
			if (m_pTexture.size() > 1) {
				m_timeElapsed += deltaTime;
				if (m_timeElapsed > m_frameSpeed) {
					m_timeElapsed = 0;

					++m_currentFrame;
					if (m_currentFrame >= m_pTexture.size()) {
						m_currentFrame = 0;
					}
				}
			}
		}

		inline LuaFile* GetLua() {
			return m_pLua;
		}

		template<typename ErrorHandler>
		inline bool Update(const Coordinate position, const ErrorHandler luaErrorHandler) {
			int positionArray[2] { {(int)position.GetX()}, { (int)position.GetY() }};
			return m_pLua->CallFunction<int>({ "tile", "process" }, positionArray, luaErrorHandler);
		}

		template<typename CustomData>
		inline void Draw(const TextureDrawer<Texture, CustomData> drawer, const Coordinate coordinate, CustomData customData) {
			(drawer)(coordinate, m_pTexture[m_currentFrame], customData);
		}

		template<typename Texture, typename CustomData>
		static Tile<Texture>* LoadFromFile(const TextureLoader<Texture, CustomData> loader, const Path path, CustomData customData) {
			LuaFile* file = LuaFile::LoadFile(path);
			if (file == nullptr) {
				return nullptr;
			}

			std::map<int, std::string> texture_paths;
			if (!file->Get<int, std::string>({ "tile", "images" }, texture_paths)) {
				delete file;
				file = nullptr;
				return nullptr;
			}

			std::vector<Texture*> textures;
			for (const auto path : texture_paths) {
				Texture* texture = (loader)(path.second, customData);
				
				if (texture) {
					textures.push_back(texture);
				} else {
					delete file;
					file = nullptr;
					std::for_each(textures.begin(), textures.end(), [](Texture* texture) {
						delete texture;
						texture = nullptr;
					});
					return nullptr;
				}
			}

			return new Tile(file, textures);
		}

	private:
		Tile(LuaFile* state, std::vector<Texture*> textures) : m_pLua(state), m_pTexture(textures), m_currentFrame(0), m_timeElapsed(0), m_frameSpeed(1 / float(textures.size())) {
			assert(state);
			assert(textures.size() > 0);
		}
		
		LuaFile* m_pLua;
		std::vector<Texture*> m_pTexture;
		unsigned short m_currentFrame;
		float m_frameSpeed, m_timeElapsed;
	};
}