local entity = class()
local gui = require "framework.gui"
local action = require "cc.action"
local assets = require "layout.assets"

local id = 0
function entity:ctor(name, attach, flags)
    id = id + 1
    self.id = id
    self.name = assert(name, "need name")
    self.attach = assert(attach, "need attach")
    self.flags = flags or gui.ImGuiTreeNodeFlags_DefaultOpen
    self.children = {}

    for k in pairs(self.attach.property_map) do
        self.attach:get_property(k)
    end
end

function entity:selected(enable)
    self.attach:show_layout(enable)
end

function entity:add(child)
    if child ~= self then
        table.insert(self.children, child)
        child:set_parent(self)
    end
end

function entity:append(peer)
    if self.parent then
        for k, v in ipairs(self.parent.children) do
            if v == self then
                table.insert(self.parent.children, k + 1, peer)
                peer:set_parent(self.parent)
                break
            end
        end
    else
        self:add(peer)
    end
end

function entity:set_parent(parent)
    self:remove()
    self.parent = parent
    self.attach.parent = parent.attach
end

function entity:get_compnents()
    return self.attach.compnents
end

function entity:get(k)
    return self.attach:get_property(k)
end

function entity:set(k, v)
    self.attach:set_property(k, v)
    if k == "content_size" then
        self.attach:show_layout(true)
        if self.name == "root" then
            self.attach:set_background(assets.colors.front_bg)
        end
    end
end

function entity:remove(remove_attach)
    if self.parent then
        for k, v in ipairs(self.parent.children) do
            if v == self then
                local child = table.remove(self.parent.children, k)
                if remove_attach then
                    child.attach:remove()
                end
                break
            end
        end
    end
end

return entity