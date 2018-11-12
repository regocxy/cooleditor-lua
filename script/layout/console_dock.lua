local console_dock = class(dock)
local gui = require "framework.gui"
local text_filter = require "editor.text_filter"

function console_dock:ctor(...)
    self.__super.ctor(self, ...)
    self.text_filter = text_filter.new()
    self.log = {}
end

function console_dock:render(area)
    gui.push_style_value(gui.ImGuiStyleVar_FramePadding, {x = 0, y = 0})
    self.text_filter:draw("FILTER", 180)
    gui.pop_style_value()
    gui.same_line()
    if gui.small_button("CLEAR") then
    end
    local _, spacing_y = gui.get_style_item_spacing()
    gui.begin_child("ScrollingRegion", {x = 0, y = -spacing_y - gui.get_frame_height_with_spacing()}, false, gui.ImGuiWindowFlags_HorizontalScrollbar)
    if gui.begin_popup_context_window() then
        if gui.selectable("Clear") then
            
        end
        gui.end_popup()
    end
    gui.push_style_value(gui.ImGuiStyleVar_ItemSpacing, {x = 4, y = 1})
    for k, v in ipairs(self.log) do
        if self.text_filter:pass_filter(v) then
            gui.text_wrapped(v)
        end
    end
    gui.pop_style_value()
    gui.end_child()
    gui.separator()
    gui.push_item_width(gui.get_content_region_avail_width())
    local cmd = gui.input_text("", gui.ImGuiInputTextFlags_EnterReturnsTrue|gui.ImGuiInputTextFlags_AutoSelectAll)
    if cmd then
        self.log[#self.log + 1] = cmd
        if gui.is_item_hovered() then
            gui.set_keyboard_focus_here(-1)
        end
    end
    gui.pop_item_width()
end

return console_dock