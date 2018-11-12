local game_dock = class(dock)
local gui = require "framework.gui"

function game_dock:ctor(...)
    self.__super.ctor(self, ...)
end

function game_dock:render(area)
    gui.text("hello")
end

return game_dock