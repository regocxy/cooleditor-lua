local scene_dock = class(dock)
local gui = require "framework.gui"
local prop_type = require "framework.utils".prop_type
local director = require "cc.director"

function scene_dock:ctor(...)
    self.__super.ctor(self, ...)
    self.wheel_delta = 0
    local w, h = director.get_width_heigth()
    self.aspect_ratio = h / w
end

function scene_dock:render(area)
    if not self.content_size then
        local w = gui.get_content_region_avail_width()
        self.content_size = {x = w, y = w * self.aspect_ratio}
        self.image_size = {x = self.content_size.x, y = self.content_size.y}
    end
    if not self.cursor_pos then
        self.cursor_pos = gui.get_cursor_pos()
    end
    local hierarchy_dock = self.layout.hierarchy_dock
    if gui.get_key_super() then
        self.wheel_delta = self.wheel_delta + gui.get_mouse_wheel()
        local scale = 1 + self.wheel_delta / 100
        hierarchy_dock.root:set("scale", {scale, scale})
    end
    if gui.is_mouse_dragging(1) then
        local delta = gui.get_mouse_delta()
        local x, y = hierarchy_dock.root:get("pos")
        hierarchy_dock.root:set("pos", {x + delta.x, y - delta.y})
    end
    gui.image(director.get_texture_id(), self.image_size)
    local entity = hierarchy_dock.selected_entity
    if entity then
        local dx, dy = 0, 0
        if gui.is_key_pressed(gui.ImGuiKey_UpArrow) then
            dy = dy - 1
        end
        if gui.is_key_pressed(gui.ImGuiKey_DownArrow) then
            dy = dy + 1
        end
        if gui.is_key_pressed(gui.ImGuiKey_LeftArrow) then
            dx = dx - 1
        end
        if gui.is_key_pressed(gui.ImGuiKey_RightArrow) then
            dx = dx + 1
        end
        if dx ~= 0 or dy ~= 0 then
            local x, y = entity:get("pos")
            entity:set("pos", {x + dx, y - dy})
        end
        if gui.is_item_hovered() then
            if gui.is_mouse_dragging(0) then
                local delta = gui.get_mouse_delta()
                local x, y = entity:get("pos")
                entity:set("pos", {x + delta.x, y - delta.y})
            end
        end
    end
end

return scene_dock