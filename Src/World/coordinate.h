#pragma once

#include "types.h"

#include <functional>

namespace World {
	class Coordinate {
	public:
		Coordinate(const Index x, const Index y);
		Index GetX() const;
		Index GetY() const;
		bool operator==(const Coordinate &anotherCoord) const;
	private:
		const Index m_x, m_y;
	};
}

namespace std {
	template <>
	struct hash<World::Coordinate> {
		std::size_t operator()(const World::Coordinate& coord) const {
			size_t res = 17;
			res *= 31 + hash<World::Index>()(coord.GetX());
			res *= 31 + hash<World::Index>()(coord.GetY());
			return res;
		}
	};
}