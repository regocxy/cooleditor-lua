local node = class()

function node:ctor()
    self.splits = {}
    self.docks = {}
    self.vertical_split = false
    self.always_auto_resize = true
    self.size = 0
end

return node
