local assets_dock = class(dock)
local gui = require "framework.gui"
local director = require "cc.director"
local assets = require "layout.assets"
local fs = require "cc.fs"
local native = require "editor.native"
local string_len, string_byte = string.len, string.byte

local function basename(path)
    for i = string_len(path) - 1, 1, -1 do
        if string_byte(path, i) == 47 then
            return path:sub(i + 1, -1)
        end
    end
    return path
end

local function extension(path)
    if path:sub(-1, -1) == "/" then
        return ""
    end
    for i = string_len(path) - 1, 1, -1 do
        if string_byte(path, i) == 46 then
            return path:sub(i + 1, -1), path:sub(1, i - 1)
        end
    end
    return ""
end

local dir = class()

function dir:ctor(fullname)
    self.fullname = fullname
    self.basename = basename(fullname)
    self.ext, self.name = extension(self.basename)
    if assets.image_exts[self.ext] then
        self.icon = director.add_texture(self.fullname)
    else 
        self.icon = assets.ext2icon[self.ext] or assets.icons.folder
    end
end

function dir:get_children()
    if not self.children then
        self.children = {}
        for _, fullname in ipairs(fs.list_files(self.fullname)) do
            table.insert(self.children, dir.new(fullname))
        end
    end
    return self.children
end

function assets_dock:ctor(...)
    self.__super.ctor(self, ...)
    self.scale = 0.3
    self.root_dir = dir.new("data/icons/")
end

function assets_dock:render(area)
    if gui.button("IMPORT...") then
        local paths = native.open_multiple_files_dialog("", "")
        for k, v in ipairs(paths or {}) do
            print(k, v)
        end
    end
    gui.same_line()
    gui.push_item_width(80)
    self.scale = gui.slide_float("", self.scale, 0.3, 1) or self.scale
    local size = 128 * self.scale
    if gui.is_item_hovered() then
        gui.begin_tool_tip()
        gui.text_unformatted("SCALE ICONS")
        gui.end_tool_tip()
    end
    gui.pop_item_width()
    
    for k, v in ipairs({"hello", "world"}) do
        gui.same_line()
        gui.push_id(k)
        gui.button(v)
        gui.pop_id()
        
        gui.same_line()
        gui.align_text_to_frame_padding()
        gui.text_unformatted("/")
    end
    gui.separator()
    local flags = gui.ImGuiWindowFlags_NoTitleBar|gui.ImGuiWindowFlags_NoMove|gui.ImGuiWindowFlags_NoResize|gui.ImGuiWindowFlags_NoSavedSettings
    if gui.begin_child("assets_content", gui.get_content_region_avail(), false, flags) then
        -- self:draw_dir(self.root_dir, size)
        local children = self.root_dir:get_children()
        if children then
            for _, child in ipairs(children) do
                self:draw_dir(child, size)
            end
        end
        gui.end_child()
    end
end

function assets_dock:draw_dir(dir, size)
    gui.push_id(dir.basename)
    -- if gui.is_any_item_active() and gui.is_window_focused() then
    -- end
    if gui.get_content_region_avail_width() < size then
        gui.new_line()
    end
    local item_size = {x = size, y = size}
    local texture_size = item_size
    if dir.icon then
        texture_size = {x = dir.icon.width, y = dir.icon.height}
    end
    local uv0 = {x = 0, y = 0}
    local uv1 = {x = 1, y = 1}
    local col = gui.get_style_color(gui.ImGuiCol_WindowBg)
    gui.push_style_color(gui.ImGuiCol_Button, {x = col.x, y = col.y, z = col.z, w = 0.44})
    gui.push_style_color(gui.ImGuiCol_ButtonHovered, {x = col.x, y = col.y, z = col.z, w = 0.86})
    gui.push_style_color(gui.ImGuiCol_ButtonActive, {x = col.x, y = col.y, z = col.z, w = 1})
    local padding = 6
    local pos = gui.get_cursor_screen_pos()
    pos.y = pos.y + item_size.y + padding * 2
    if gui.image_button_with_aspect_and_text_down(dir.icon.id, dir.basename, texture_size, item_size, uv0, uv1, padding) then
    end
    if gui.is_item_hovered() and gui.is_mouse_double_clicked(0) then
        print("double")
    end
    gui.pop_style_color(3)
    if not self:process_drag_drop_source(dir) then
        self:process_drag_drop_target(dir)
    end
    gui.pop_id()
    gui.same_line()
end

function assets_dock:process_drag_drop_source(dir)
    if gui.begin_drag_drop_source() then
        local item_size = {x = 32, y = 32}
        local texture_size = item_size
        if dir.icon then
            texture_size = {x = dir.icon.width, y = dir.icon.height}
        end
        gui.begin_group()
        local w = gui.calc_text_size(dir.basename).x
        gui.same_line(0, (w - item_size.x) / 2)
        gui.image_with_aspect(dir.icon.id, texture_size, item_size)
        gui.text_unformatted(dir.basename)
        gui.end_group()
        gui.set_drag_drop_payload("dir", dir)
        gui.end_drag_drop_source()
        return true
    end
    return false;
end


function assets_dock:process_drag_drop_target(dir)
    if gui.begin_drag_drop_target() then

        gui.end_drag_drop_target()
    end
end

-- function assets_dock:draw_dir(dir)
--     local flags = gui.ImGuiTreeNodeFlags_AllowItemOverlap|gui.ImGuiTreeNodeFlags_OpenOnArrow
--     if gui.tree_node_ex(dir.basename, flags) then
--         local children = dir:get_children()
--         if children then
--             for _, child in ipairs(children) do
--                 self:draw_dir(child)
--             end
--         end
--         gui.tree_pop()
--     end
-- end

return assets_dock