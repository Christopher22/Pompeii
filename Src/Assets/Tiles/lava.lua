tile = {}
tile["images"] = { "Assets\\Graphic\\lava.png" }
function tile.process(x, y)
	local spread = false
	for i, coord in ipairs({{0, -1}, {0, 1}, {-1, 0}, {1, 0}}) do 
		local newX = x + coord[1]
		local newY = y + coord[2]
		
		local nextTile = World.getTile(newX, newY)
		
		if nextTile == Pompeii.TILES.PLAYER or nextTile == Pompeii.TILES.GOAL then
			Pompeii.lose()
		elseif nextTile ~= -1 and ((i == 4 and spread == false) or math.random() < 0.5) then
			World.setTile(newX, newY, Pompeii.TILES.LAVA)
			World.addNextTile(newX, newY)
			spread = true
		end
	end
end
