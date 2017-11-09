tile = {}
tile["images"] = { "Assets\\Graphic\\player_f1.png", "Assets\\Graphic\\player_f2.png", "Assets\\Graphic\\player_f3.png", "Assets\\Graphic\\player_f4.png", "Assets\\Graphic\\player_f5.png", "Assets\\Graphic\\player_f6.png", "Assets\\Graphic\\player_f7.png", "Assets\\Graphic\\player_f8.png" }
function tile.process(x, y)
	local moveX, moveY = Pompeii.getMovement()
	local nextTile = World.getTile(x + moveX, y + moveY)
	
	if nextTile == Pompeii.TILES.LAVA then
		Pompeii.lose()
	elseif nextTile ~= -1 then
		if nextTile == Pompeii.TILES.GOAL then
			Pompeii.win()
		elseif nextTile == Pompeii.TILES.ROMAN then
			Pompeii.rescueRoman()
		end
		
		World.setTile(x, y, 0)
		World.setTile(x + moveX, y + moveY, Pompeii.TILES.PLAYER)
		World.addNextTile(x + moveX, y + moveY)
	else
		World.addNextTile(x, y)
	end
end

