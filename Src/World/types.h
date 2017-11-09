#pragma once

#include <string>

namespace World {
	class Coordinate;

	typedef unsigned char Id;
	typedef unsigned short Index;
	typedef const std::string& Path;

	template<typename Texture, typename CustomData>
	using TextureLoader = Texture*(*)(Path, CustomData);

	template<typename Texture, typename CustomData>
	using TextureDrawer = void(*)(Coordinate, Texture*, CustomData);
}