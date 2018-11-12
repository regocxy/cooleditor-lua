local layout = {}
local gui = require "editor.gui"
local director = require "cc.director"
local macro = require "dock.dock_macro"
local dockspace = require "dock.dockspace"

local assets = require "layout.assets"
local scene_dock = require "layout.scene_dock"
local game_dock = require "layout.game_dock"
local hierarchy_dock = require "layout.hierarchy_dock"
local inspector_dock = require "layout.inspector_dock"
local console_dock = require "layout.console_dock"
local assets_dock = require "layout.assets_dock"

function layout:init()
    self.scene_dock = scene_dock.new(self, "SCENE", false, {x = 200, y = 400})
    self.game_dock = game_dock.new(self, "GAME", true, {x = 300, y = 400})
    self.inspector_dock = inspector_dock.new(self, "INSPECTOR", false, {x = 250, y = 400})
    self.hierarchy_dock = hierarchy_dock.new(self, "HIERARCHY", false, {x = 250, y = 400})
    self.console_dock = console_dock.new(self, "CONSOLE", false, {x = 200, y = 200})
    self.assets_dock = assets_dock.new(self, "ASSETS", false, {x = 200, y = 200})
    self.dockspace = dockspace.new()
    self.dockspace:dock_to(self.scene_dock, macro.slot_type.TAB, 200, true)
    self.dockspace:dock_with(self.game_dock, self.scene_dock, macro.slot_type.TAB, 300, false)
    self.dockspace:dock_with(self.inspector_dock, self.scene_dock, macro.slot_type.RIGHT, 250, true)
    self.dockspace:dock_with(self.hierarchy_dock, self.scene_dock, macro.slot_type.LEFT, 250, true)
    self.dockspace:dock_to(self.console_dock, macro.slot_type.BOTTOM, 200, true)
    self.dockspace:dock_with(self.assets_dock, self.console_dock, macro.slot_type.TAB, 200, false)
end

function layout:draw_menu()
    if gui.begin_main_menubar() then
        if gui.begin_menu("File") then
            if gui.menu_item("NEW SCENE", "CTRL+N", false) then
                
            end
            gui.end_menu()
        end
        local offset = gui.get_window_height()
        gui.end_main_menubar()
        gui.set_cursor_posy(gui.get_cursor_posy() + offset)
    end
end

function layout:draw_toolbar()
    local icons = assets.icons
    local width = gui.get_content_region_avail_width()
    if gui.tool_bar_button(icons.translate.id, "TRANSLATE", false) then
    end
    gui.same_line(0)
    if gui.tool_bar_button(icons.rotate.id, "ROTATE", true) then
    end
    gui.same_line(0)
    if gui.tool_bar_button(icons.scale.id, "SCALE", true) then
    end
    gui.same_line(0, 50)
    if gui.tool_bar_button(icons.loc.id, "LOCAL COORDINATE SYSTEM", true) then
    end
    gui.same_line(0)
    if gui.tool_bar_button(icons.global.id, "GLOBAL COORDINATE SYSTEM", false) then
    end
    gui.same_line(0)
    if gui.tool_bar_button(icons.grid.id, "SHOW GRID", false) then
    end
    gui.same_line(0)
    if gui.tool_bar_button(icons.wireframe.id, "WIREFRAME SELECTION", false) then
    end
    gui.same_line(width / 2 - 36)
    if gui.tool_bar_button(icons.play.id, "PLAY", false) then
    end
    gui.same_line(0)
    if gui.tool_bar_button(icons.pause.id, "PAUSE", false) then
    end
    gui.same_line(0)
    if gui.tool_bar_button(icons.step.id, "STEP", false) then
    end
end

function layout:draw_dockspace()
    local offset = gui.get_frame_height_with_spacing()
    local region = gui.get_content_region_avail()
    self.dockspace:update_and_draw({x = region.x, y = region.y - offset})
end

function layout:draw_header()
    self:draw_menu()
    self:draw_toolbar()
end

function layout:render()
    gui.draw_begin()
    self:draw_header()
    self:draw_dockspace()
    gui.draw_end()
end

return layout