import "CoreLibs/graphics"

playdate.display.setScale(2)
playdate.display.setRefreshRate(50)

local x, y = playdate.display:getWidth()/2, playdate.display:getHeight()/2
function playdate.update()
	playdate.graphics.clear()
	playdate.graphics.drawTextAligned(currentFrequency().."Hz", x, y, kTextAlignment.center)
end
