local spritec = require "cc.sprite"
local nodec = require "cc.node"
local utils = require "framework.utils"
local prop_type = utils.prop_type
local methods, setter, getter = utils.inherit(spritec, nodec)
local spine_methods = spritec.spine_methods
local unpack = table.unpack

local sprite_meta = {}

function sprite_meta.__index(self, k)
    if methods[k] then
        return methods[k]
    end
    if getter[k] then
        return getter[k](self)
    end
    return debug.getuservalue(self)[k]
end

function sprite_meta.__newindex(self, k, v)
    if setter[k] then
        return setter[k](self, v)
    end
    debug.getuservalue(self)[k] = v
end


local ref = {}

function ref:create(compnents)
    local o = setmetatable({compnents = compnents}, {__index = self})
    o:ctor()
    return o
end

function ref:ctor()
    self.property_map = {}
    for _, comp in ipairs(self.compnents) do
        for _, v in ipairs(comp.property) do
            self.property_map[v.name] = v
        end
    end
end

function ref:set_property(k, v)
    local property = self.property_map[k]
    local f = self["set_"..k]
    if property and f then
        property.value = v
        if type(v) == "table" then
            f(self, unpack(v))
        else 
            f(self, v)
        end
    end
end

function ref:get_property(k)
    local property = self.property_map[k]
    local f = self["get_"..k]
    if property and f then
        local t = {f(self)}
        if #t > 1 then
            property.value = t
        else 
            property.value = t[0]
        end
        return unpack(t)
    end
end

local node_property = {
    {name = "pos", type = prop_type.float_vec2},
    {name = "content_size", type = prop_type.float_vec2},
    {name = "scale", type = prop_type.float_vec2, step = 0.1},
}

local function new_compnent(name, property)
    local t = {
        {name = "Node", property = node_property},
    }
    if name and property then
        t[#t + 1] = {name = name, property = property}
    end
    return t
end

local sprite = {}

function sprite.new(...)
    local o = debug.setmetatable(spritec.new(...), sprite_meta)
    local property = {
        {name = "texture", type = prop_type.string}
    }
    debug.setuservalue(o, ref:create(new_compnent("Sprite", property)))
    return o
end

function sprite.scene()
    local o = debug.setmetatable(spritec.scene(), sprite_meta)
    debug.setuservalue(o, ref:create(new_compnent()))
    return o
end

function sprite.spine(...)
    local o = debug.setmetatable(spritec.spine(...), {
        __index = function(self, k)
            local ret = sprite_meta.__index(self, k)
            if ret then
                return ret
            end
            if spine_methods[k] then
                return spine_methods[k]
            end
        end,
        __newindex = sprite_meta.__newindex
    })
    local property = {
        {
            title = "",
            name = "anim",
            type = prop_type.details,
            details = {
                {name = "track", type = prop_type.int, value = 0},
                {name = "name", type = prop_type.string, value = ""},
                {name = "loop", type = prop_type.check, value = true},
            },
            value = {0, "idle", true}
        },
    }
    debug.setuservalue(o, ref:create(new_compnent("Spine", property)))
    o:update()
    return o
end

return sprite