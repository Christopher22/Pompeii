#include "luafile.h"

#include <map>
#include <algorithm>
#include <cassert>

namespace World {

	LuaFile::LuaFile(lua_State* state): m_pLua(state) {
		assert(state);
	}

	LuaFile::~LuaFile() {
		lua_close(m_pLua);
	}

	LuaFile* LuaFile::LoadFile(const Path path, const int results) {
		lua_State* tmp = luaL_newstate();
		luaL_openlibs(tmp);
		if (luaL_loadfile(tmp, path.c_str()) || lua_pcall(tmp, 0, results, 0)) {
			lua_close(tmp);
			return nullptr;
		}
		return new LuaFile(tmp);
	}

	int LuaFile::moveToLevel(const IdentifierPath path, const bool ignoreLast, const bool createIfNotExisting) {
		int currentLevel = 0;
		std::for_each(path.begin(), (!ignoreLast ? path.end() : path.end() - 1), [&](Identifier part) {
			if (currentLevel == -1) {
				return;
			} else if (currentLevel == 0) {
				lua_getglobal(m_pLua, part);
			} else {
				lua_getfield(m_pLua, -1, part);
			}

			if (lua_isnil(m_pLua, -1)) {
				if (!createIfNotExisting) {
					lua_pop(m_pLua, currentLevel);
					currentLevel = -1;
				} else {
					lua_pop(m_pLua, 1); // Remove nil
					if (currentLevel == 0) {
						lua_newtable(m_pLua);
						lua_setglobal(m_pLua, part);
						lua_getglobal(m_pLua, part);
					}
					else {
						lua_pushstring(m_pLua, part);
						lua_newtable(m_pLua);
						lua_settable(m_pLua, -3);
						lua_getfield(m_pLua, -1, part);
					}
					
					currentLevel++;
				}
			} else {
				currentLevel++;
			}
		});
		return currentLevel;
	}

	void LuaFile::UnregisterFunction(const Identifier path) {
		Set(path, Nil());
	}

	void LuaFile::UnregisterFunction(const IdentifierPath path) {
		Set(path, Nil());
	}
}