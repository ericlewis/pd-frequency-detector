import "CoreLibs/graphics"

local Display <const> = playdate.display
local Graphics <const> = playdate.graphics
local CenterAlignment <const> = kTextAlignment.center

Display.setScale(2)
Display.setRefreshRate(50)

local x, y = Display:getWidth()/2, Display:getHeight()/2
function playdate.update()
	Graphics.clear()
	Graphics.drawTextAligned(currentFrequency().."Hz", x, y, CenterAlignment)
end
