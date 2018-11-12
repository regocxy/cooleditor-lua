package.path = package.path..";../script/?.lua"

function class(super)
    local cls = {__super = super}
    setmetatable(cls, {__index = super})
    if not cls.ctor then
        cls.ctor = function() end
    end
    cls.new = function(...)
        local o = {}
        setmetatable(o, {__index = cls})
        o:ctor(...)
        return o
    end
    cls.create = function(_, ...)
        return cls.new(...)
    end
    return cls
end

dock = require "dock.dock"
local assets = require "layout.assets"
local layout = require "layout.layout"
local cocos = require "cocos.cocos"
local fw = require "editor.framework"

fw.EDITOR_INIT = function()
    print("EDITOR_INIT")
    cocos:init()
    layout:init()
end

fw.EDITOR_UPDATE = function()
    -- print("EDITOR_UPDATE")
    layout:render()
end

fw.inject()

