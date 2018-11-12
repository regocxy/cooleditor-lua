local dock = class()
local macro = require "./dock/dock_macro"
local node = require "./dock/node"

local index = 0
function dock:ctor(layout, title, close_button, min_size)
    index = index + 1
    print("create dock:", title, index)
    self.layout = assert(layout, "need layout")
    self.title = title.."###"..index or "title"
    self.close_button = close_button
    self.min_size = min_size or {x = 100, y = 100}
    self.container = node.new()

    self.redock_slot = macro.slot_type.NONE
    self.undockable = false
    self.draging = false
end

function dock:draw_function(size)
    self:render(size)
end

function dock:render(size)
    
end

return dock